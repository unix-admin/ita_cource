#include "userdetails.h"
#include "ui_userdetails.h"
#include <QMovie>
#include <QLayout>
#include <QLabel>
#include <QScrollBar>
#include <QThread>
UserDetails::UserDetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserDetails)
{
    databaseUser = false;
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
}

UserDetails::~UserDetails()
{
    delete db;
    delete twitterRequests;
    delete parser;
    delete ui;
}

void UserDetails::prepareData(QString userid)
{
    db = new DataBase;
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
    this->resize(200,200);
    this->show();
    userInDatabase = db->checkUser(userid,BY_TWITTER_ID);
    if (!userInDatabase)
    {
        getUserinfoFromTwitter(id.toStdString());
    }
    else
    {
        databaseUser = true;
        getUserinfoFromDatabase(id);
    }


}

void UserDetails::getUserinfoFromTwitter(std::string id)
{
    QStringList userInfo;
    requestData = twitterRequests->getRequest(GET_USER_BY_ID,id,"");
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
    requestData = twitterRequests->getRequest(GET_USER_TIMELINE,id,"&count="+twitter->getUserSettings()->timelineTweetsByPage.toStdString()+"&page=1");
    userTimeline = parser->parseTweets(&requestData);
    ui->userTimeline->setText(userTimeline);    
    showResults();
}

void UserDetails::getUserinfoFromDatabase(QString id)
{
    userData = db->getData(id,BY_TWITTER_ID);
    ui->userTimeline->setText(getTimeline(0,100,""));
    page=1;
    if(db->checkReadableUser(twitter->getUserData()->id,userData.twitterID))
        ui->buttonToDatabase->setVisible(false);
    syncTimer = new QTimer;
    connect(syncTimer,SIGNAL(timeout()),this,SLOT(updateData()));
    syncTimer->start(30000);
    showResults();

}

QString UserDetails::getTimeline(int left, int right, QString userLastTweet)
{
        QList<Twitter::tweetsData> tweets;
        lastTweet = db->getLastTweetID(userData.twitterID);
        tweets = db->getTimeline(userLastTweet, userData.twitterID,left,right,USER_TIMELINE);
        QString result;
        result.append("<style>.select {font-weight: 600;} </style>");
        for(int i=0; i<tweets.count(); i++)
        {
            if (i==tweets.count()-1)
            {
                result.append("<span class=\"select\">"+tweets.at(i).tweetTime+" "+tweets.at(i).username + "</span>:" +tweets.at(i).text+";");
            }
            else
            {
                result.append("<span class=\"select\">"+tweets.at(i).tweetTime+" "+tweets.at(i).username + "</span>:" +tweets.at(i).text+";<br>");
            }
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
    if (databaseUser)
    {
        if (ui->userTimeline->verticalScrollBar()->value()== ui->userTimeline->verticalScrollBar()->maximum())
        {
            page++;
            ui->userTimeline->append(getTimeline(page*100,page*100+100,""));
        }
    }
    else
    {
        QString  minTweetID;
       if (ui->userTimeline->verticalScrollBar()->value()== ui->userTimeline->verticalScrollBar()->maximum())
       {
                minTweetID = parser->minTweetID;
                requestData = twitterRequests->getRequest(GET_USER_TIMELINE,id.toStdString(),"&count="+twitter->getUserSettings()->timelineTweetsByPage.toStdString()\
                                                          +"&max_id="+minTweetID.toStdString());
                userTimeline = parser->parseTweets(&requestData);
                if (parser->minTweetID !=minTweetID)
                ui->userTimeline->append(userTimeline);
       }
    }
}

void UserDetails::updateData()
{
    QStringList syncUserData;
    QStringList syncTimelines;
    if (lastSynchronization  != twitter->getLastSyncTime())
    {
        syncTimelines =  twitter->getSyncedTimelines();
        syncUserData = twitter->getSyncedUsers();
        if (syncUserData.count() > 0)
        {
            for (int i=0; i<syncUserData.count(); i++)
            {
                if (syncUserData.at(i) == userData.twitterID)
                {
                    Twitter::userData dataToSync = db->getData(userData.twitterID,BY_TWITTER_ID);
                    userData.name = dataToSync.name;
                    userData.screen_name = dataToSync.screen_name;
                    userData.description = dataToSync.description;
                    userData.profile_image_data = dataToSync.profile_image_data;
                    userData.statuses_count = dataToSync.statuses_count;
                    userData.friends_count = dataToSync.friends_count;
                    userData.followers_count = dataToSync.followers_count;
                    showResults();
                }
            }
        }
        QString previousText;
        int scrollbarvalue = 0;
        int records = 0;
        int pages = 0;
        if (syncTimelines.count() > 0)
        {

            for (int i=0; i<syncTimelines.count(); i++)
            {
                if (syncTimelines.at(i) == userData.twitterID)
                {
                    records = db->countRecordsInTimeLine(userData.twitterID,lastTweet,USER_TIMELINE);
                    if (records > 100)
                    {
                        if (records%100 == 0)
                        {
                            pages = records/100;
                        }
                        else
                        {
                            pages = records/100+1;
                        }
                        for(int i=0; i<pages; i++)
                        {
                            scrollbarvalue = ui->userTimeline->verticalScrollBar()->maximum()-ui->userTimeline->verticalScrollBar()->value();
                            previousText = ui->userTimeline->toHtml();
                            ui->userTimeline->setText(getTimeline(i*100,i*100+100,lastTweet)+previousText);
                            ui->userTimeline->verticalScrollBar()->setValue(ui->userTimeline->verticalScrollBar()->maximum()-scrollbarvalue);
                        }
                    }
                    else
                    {
                        scrollbarvalue = ui->userTimeline->verticalScrollBar()->maximum()-ui->userTimeline->verticalScrollBar()->value();
                        previousText = ui->userTimeline->toHtml();
                        ui->userTimeline->setText(getTimeline(0,100,lastTweet)+previousText);
                        ui->userTimeline->verticalScrollBar()->setValue(ui->userTimeline->verticalScrollBar()->maximum()-scrollbarvalue);
                    }
                }
            }
        lastSynchronization = twitter->getLastSyncTime();
     }
   }
}

void UserDetails::timeLineToDatabase()
{

 int pages;
 QList<QVariant> tweetList;
 QMap<QString,QVariant> tweetMap;
 QMap<QString,QVariant> userMap;
 DataBase::tweets tweetsToDB;
 QMap<QString, QVariant>::const_iterator user;
 QList<DataBase::tweets> dataToInsert;  
 if (userData.statuses_count.toInt()%100 > 0)
    {
        pages = userData.statuses_count.toInt()/100 +1;
    }
 else
    {
         pages = userData.statuses_count.toInt()/100;
    }
ui->progressBar->setVisible(true);
 for(int i=0; i<pages; i++)
 {
    requestData = twitterRequests->getRequest(GET_USER_TIMELINE,userData.twitterID.toStdString(),"&count=100&include_rts=true"\
                                              "&page="+QString::number(i+1).toStdString());
    tweetList = parser->parseTweetsToDatabase(&requestData);
    for ( int t = 0 ; t<tweetList.count(); t++)
    {
       tweetMap = tweetList.at(t).toMap();
       tweetsToDB.tweetID =tweetMap.find("id_str").value().toString();
       tweetsToDB.tweetTime = parser->dateFormat(tweetMap.find("created_at").value());
       tweetsToDB.text = tweetMap.find("text").value().toString();
       tweetsToDB.twitterUserID = userData.twitterID;
       user = tweetMap.find("user");
       userMap = user.value().toMap();
       tweetsToDB.username= userMap.find("screen_name").value().toString();
       dataToInsert.append(tweetsToDB);       
    }
    db->insertTweetsToDatabase(&dataToInsert);
    ui->progressBar->setValue((i+1)*100/pages);
    dataToInsert.clear();
 }
  ui->progressBar->setValue(100);
  ui->progressBar->setVisible(false);
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
