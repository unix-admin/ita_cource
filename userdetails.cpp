#include "userdetails.h"
#include "ui_userdetails.h"
#include <QMovie>
#include <QLayout>
#include <QLabel>
#include <QScrollBar>

UserDetails::UserDetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserDetails)
{
    ui->setupUi(this);
}

UserDetails::~UserDetails()
{
    delete ui;
}

void UserDetails::prepareData(Twitter *clsTwitter, std::string userid)
{
    db = DataBase::getInstance();
    id = userid;
    twitter = clsTwitter;
    ui->label->resize(200,200);
    QMovie* movie = new QMovie(":/data/images/loading.gif");
    ui->label->setMovie(movie);
    ui->userTimeline->setVisible(false);
    ui->horizontalLayoutWidget->setVisible(false);
    ui->gridLayoutWidget->setVisible(false);
    movie->start();
    connect(ui->buttonToDatabase,SIGNAL(clicked(bool)),this, SLOT(toDatabase()));
    connect(ui->userTimeline->verticalScrollBar(),SIGNAL(valueChanged(int)),this, SLOT(appendTimeline()));
    currentPage = 1;
    this->resize(200,200);
    this->show();
    getUserinfo(id);

}

void UserDetails::getUserinfo(std::string id)
{
    Requests *getInfo = new Requests;

    QByteArray info;
    QStringList parsedInfo;
    info = getInfo->getRequest(GET_USER_BY_ID,id,"",twitter);
    Parser *parseInfo = new Parser;
    QString timeLine;
    parsedInfo = parseInfo->parseUserInfo(&info);
    userData.twitterID = parsedInfo.value(0);
    userData.name = parsedInfo.value(1);
    userData.screen_name = parsedInfo.value(2);
    userData.description = parsedInfo.value(3);
    userData.statuses_count = parsedInfo.value(4);
    userData.friends_count = parsedInfo.value(5);
    userData.followers_count = parsedInfo.value(6);
    userData.profile_image_url = parsedInfo.value(7);
    userData.profile_image_data = getInfo->getImage(parsedInfo.value(7));
    ui->label_2->setText(userData.name);
    ui->label_3->setText(userData.screen_name);
    ui->label_4->setText(userData.description);
    ui->label_5->setText("Твиты: "+userData.statuses_count);
    ui->label_6->setText("Друзья: "+userData.friends_count);
    ui->label_7->setText("Читатели: "+userData.followers_count);
    photo.loadFromData(userData.profile_image_data);
    info = getInfo->getRequest(GET_USER_TIMELINE,id,"&count="+twitter->getUserSettings()->timelineTweetsByPage.toStdString()+"&page=1",twitter);
    timeLine = parseInfo->parseTweets(&info);
    ui->userTimeline->setText(timeLine);
    this->resize(400,580);
    ui->label->resize(128,128);
    ui->label->setPixmap(photo);
    ui->horizontalLayoutWidget->setVisible(true);
    ui->gridLayoutWidget->setVisible(true);
    ui->userTimeline->setVisible(true);
    if (db->checkUser(userData.screen_name))
        if(db->checkReadableUser(twitter->getUserData()->id,userData.twitterID))
            ui->buttonToDatabase->setVisible(false);
    if (userData.statuses_count.toInt()%twitter->getUserSettings()->timelineTweetsByPage.toInt() > 0)
    {
        pages = userData.statuses_count.toInt()/twitter->getUserSettings()->timelineTweetsByPage.toInt()+1;
    }
    else
    {
        pages = userData.statuses_count.toInt()/twitter->getUserSettings()->timelineTweetsByPage.toInt()+1;
    }
    delete getInfo;
    delete parseInfo;
}

void UserDetails::toDatabase()
{
    if (db->checkUser(userData.screen_name))
     {
        db->addReadableUser(&userData,twitter->getUserData()->id,USER_TO_READ);
     }
     else
     {
        db->addReadableUser(&userData,twitter->getUserData()->id,NEW_USER);
        timeLineToDatabase();
     }
    ui->buttonToDatabase->setVisible(false);


}

void UserDetails::appendTimeline()
{
   if (ui->userTimeline->verticalScrollBar()->value()== ui->userTimeline->verticalScrollBar()->maximum())
   {
        if (!(currentPage > pages))
        {
            currentPage++;
            Requests *getInfo = new Requests;
            Parser *parseInfo = new Parser;
            QString timeLine;
            QByteArray info;
            info = getInfo->getRequest(GET_USER_TIMELINE,id,"&count="+twitter->getUserSettings()->timelineTweetsByPage.toStdString() \
                                   +"&page="+QString::number(currentPage).toStdString(),twitter);
            timeLine = parseInfo->parseTweets(&info);
            ui->userTimeline->append(timeLine);
            delete getInfo;
            delete parseInfo;
        }
   }
}

void UserDetails::timeLineToDatabase()
{
 int pages;
 Requests *requestTimeLine = new Requests;
 QByteArray getTimeLine;
 Parser *parserTimeLine = new Parser;
 QList<QVariant> tweetList;
 QMap<QString,QVariant> tweetMap;
 QMap<QString,QVariant> userMap;
 DataBase::tweetsData tweetsToDB;
 QMap<QString, QVariant>::const_iterator user;
 if (userData.statuses_count.toInt()%100 > 0)
    {
        pages = userData.statuses_count.toInt()/200 +1;
    }
 else
    {
         pages = userData.statuses_count.toInt()/200;
    }
 for(int i=0; i<pages; i++)
 {
    getTimeLine = requestTimeLine->getRequest(GET_USER_TIMELINE,userData.twitterID.toStdString(),"&count=200&exclude_replies=false"\
                                              "&page="+QString::number(i+1).toStdString(),twitter);
    tweetList = parserTimeLine->parseTweetsToDatabase(&getTimeLine);
    for ( int t = 0 ; t<tweetList.count(); t++)
    {
       tweetMap = tweetList.at(t).toMap();
       tweetsToDB.tweetID =tweetMap.find("id_str").value().toString();
       tweetsToDB.tweetTime = parserTimeLine->dateFormat(tweetMap.find("created_at").value());
       tweetsToDB.text = tweetMap.find("text").value().toString();
       tweetsToDB.twitterUserID = userData.twitterID;
       tweetsToDB.searchID = "0";
       user = tweetMap.find("user");
       userMap = user.value().toMap();
       tweetsToDB.username= userMap.find("screen_name").value().toString();
       db->insertTweetsToDatabase(&tweetsToDB);
    }
 }
}
