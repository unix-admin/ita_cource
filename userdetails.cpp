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
    delete twitterRequests;
    delete parser;
    delete ui;
}

void UserDetails::prepareData(QString userid)
{
    db = DataBase::getInstance();
    id = userid;
    twitter = Twitter::getcls();
    twitterRequests = new Requests;
    parser = new Parser;
    ui->label->resize(200,200);
    QMovie* movie = new QMovie(":/data/images/loading.gif");
    ui->label->setMovie(movie);
    ui->userTimeline->setVisible(false);
    ui->horizontalLayoutWidget->setVisible(false);
    ui->gridLayoutWidget->setVisible(false);
    movie->start();
    connect(ui->buttonToDatabase,SIGNAL(clicked(bool)),this, SLOT(toDatabase()));
    connect(ui->userTimeline->verticalScrollBar(),SIGNAL(valueChanged(int)),this, SLOT(appendTimeline()));
    currentPage = 0;
    pages = 100;
    this->resize(200,200);
    this->show();
    userInDatabase = db->checkUser(userid,BY_TWITTER_ID);
    if (!userInDatabase)
    {
        getUserinfoFromTwitter(id.toStdString());
    }
    else
    {
        getUserinfoFromDatabase(id);
    }


}

void UserDetails::getUserinfoFromTwitter(std::string id)
{
    QStringList userInfo;
    requestData = twitterRequests->getRequest(GET_USER_BY_ID,id,"",twitter);
    userInfo = parser->parseUserInfo(&requestData);
    userData.twitterID = userInfo.value(0);
    userData.name = userInfo.value(1);
    userData.screen_name = userInfo.value(2);
    userData.description = userInfo.value(3);
    userData.statuses_count = userInfo.value(4);
    userData.friends_count = userInfo.value(5);
    userData.followers_count = userInfo.value(6);
    userData.profile_image_url = userInfo.value(7);
    userData.profile_image_data = twitterRequests->getImage(userInfo.value(7));
    requestData = twitterRequests->getRequest(GET_USER_TIMELINE,id,"&count="+twitter->getUserSettings()->timelineTweetsByPage.toStdString()+"&page=1",twitter);
    userTimeline = parser->parseTweets(&requestData);
    ui->userTimeline->setText(userTimeline);
    if (userData.statuses_count.toInt()%twitter->getUserSettings()->timelineTweetsByPage.toInt() > 0)
    {
        pages = userData.statuses_count.toInt()/twitter->getUserSettings()->timelineTweetsByPage.toInt()+1;
    }
    else
    {
        pages = userData.statuses_count.toInt()/twitter->getUserSettings()->timelineTweetsByPage.toInt()+1;
    }
    showResults();
}

void UserDetails::getUserinfoFromDatabase(QString id)
{
    userData = db->getData(id,BY_TWITTER_ID);
    ui->userTimeline->setText(getTimeline(0,100));
    if(db->checkReadableUser(twitter->getUserData()->id,userData.twitterID))
        ui->buttonToDatabase->setVisible(false);
    showResults();

}

QString UserDetails::getTimeline(int left, int right)
{
        QList<DataBase::tweetsData> tweets;
        tweets = db->getTimeline("", userData.twitterID,left,right,USER_TIMELINE);
        QString result;
        result.append("<style>.select {font-weight: 600;} </style>");
        for(int i=0; i<tweets.count(); i++)
        {
            result.append("<span class=\"select\">"+tweets.at(i).tweetTime+" "+tweets.at(i).username + "</span>:" +tweets.at(i).text+";<br>");
        }
        return result;
}




void UserDetails::toDatabase()
{
    if (db->checkUser(userData.screen_name,BY_DISPLAY_NAME))
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
            requestData = twitterRequests->getRequest(GET_USER_TIMELINE,id.toStdString(),"&count="+twitter->getUserSettings()->timelineTweetsByPage.toStdString() \
                                   +"&page="+QString::number(currentPage).toStdString(),twitter);
            userTimeline = parser->parseTweets(&requestData);
            ui->userTimeline->append(userTimeline);
        }
   }
}

void UserDetails::timeLineToDatabase()
{
 int pages;
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
    requestData = twitterRequests->getRequest(GET_USER_TIMELINE,userData.twitterID.toStdString(),"&count=200&exclude_replies=false"\
                                              "&page="+QString::number(i+1).toStdString(),twitter);
    tweetList = parser->parseTweetsToDatabase(&requestData);
    for ( int t = 0 ; t<tweetList.count(); t++)
    {
       tweetMap = tweetList.at(t).toMap();
       tweetsToDB.tweetID =tweetMap.find("id_str").value().toString();
       tweetsToDB.tweetTime = parser->dateFormat(tweetMap.find("created_at").value());
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

void UserDetails::showResults()
{
    ui->label_2->setText(userData.name);
    ui->label_3->setText(userData.screen_name);
    ui->label_4->setText(userData.description);
    ui->label_5->setText("Твиты: "+userData.statuses_count);
    ui->label_6->setText("Друзья: "+userData.friends_count);
    ui->label_7->setText("Читатели: "+userData.followers_count);
    photo.loadFromData(userData.profile_image_data);
    this->resize(400,580);
    ui->label->resize(128,128);
    ui->label->setPixmap(photo);
    ui->horizontalLayoutWidget->setVisible(true);
    ui->gridLayoutWidget->setVisible(true);
    ui->userTimeline->setVisible(true);
}

void UserDetails::closeEvent(QCloseEvent *)
{
    emit formClosed();
}
