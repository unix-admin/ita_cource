#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QScrollBar>
#include "readableusers.h"
#include "synchronization.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{    

    ui->setupUi(this);    
    ui->centralWidget->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->myTtwitterTimeline->setReadOnly(true);
    tw = Twitter::getcls();
    QPixmap myImg(":/data/splash.jpg");    
    ui->label_2->setPixmap(myImg);
    QTimer *timer = new QTimer;
    timer->start(20000);
    connect(timer,SIGNAL(timeout()), this, SLOT(networkConnection()));
    sync.setFileName(":/data/images/loading_small.gif");
    ui->syncLabel->setMovie(&sync);
    sync.start();
    ui->syncLabel->setVisible(false);
    ui->syncText->setVisible(false);
    ui->statusBar->insertPermanentWidget(0, ui->netImg ,0 );
    ui->statusBar->insertPermanentWidget(0, ui->netText ,0 );
    ui->statusBar->addWidget(ui->syncLabel);
    ui->statusBar->addWidget(ui->syncText);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(ui->myTtwitterTimeline,SIGNAL(textChanged()),this, SLOT(splashInvisible()));
    connect(ui->searchButton,SIGNAL(clicked()),SLOT(userSearch()));
    connect(ui->centralWidget,SIGNAL(destroyed(QObject*)),this,SLOT(close()));
    connect(ui->tweetSearchButton,SIGNAL(clicked()),SLOT(tweetSearch()));   
    connect(ui->settingsButton,SIGNAL(clicked(bool)),this, SLOT(settingsShow()));
    connect(ui->myUsers,SIGNAL(clicked(bool)),this,SLOT(myUserClicked()));
    connect(tw,SIGNAL(finished()),this, SLOT(userShow()));
    connect(ui->myVirtualTimeline->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(moved()));
    networkConnection();
    db = new DataBase;
    leftLimit=0;
    rightLimit=100;    
    ui->comboBox->insertItems(0,db->getUsers());
    ui->verticalLayoutWidget_3->setVisible(true);
    ui->horizontalLayoutWidget->setVisible(false);
    ui->formLayoutWidget->setVisible(false);
    ui->photo->setVisible(false);
    ui->verticalLayoutWidget->setVisible(false);
    ui->tabWidget->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete db;
    delete ui;
}

void MainWindow::buttonClicked()
{
    if (ui->comboBox->currentText() == "Новый пользователь")
    {
        autorize *newUser = new autorize;
        newUser->getAutorisation();
        connect(newUser,SIGNAL(formClosed()),this,SLOT(getNewUserData()));
        connect(newUser,SIGNAL(formClosed()),newUser,SLOT(deleteLater()));
    }
    else
    {
     userShow();
    }
}

void MainWindow::networkConnection()
{
    netError = false;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished()));
    QNetworkRequest request;
    request.setUrl(QUrl("http://abs.twimg.com/favicons/favicon.ico"));
    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");
    QNetworkReply *reply;

    reply = manager->get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),SLOT(networkError()));
    connect(reply,SIGNAL(finished()),this,SLOT(networkOk()));
}

void MainWindow::networkError()
{
   tw->setNetworkStatus(false);
}

void MainWindow::networkOk()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    tw->setNetworkStatus(true);
    if (netError)
    {
        ui->netText->setText(QString::fromStdString("Сеть недоступна"));
        ui->netImg->setPixmap(QPixmap(":/data/images/disconnected.png"));

    }
    else
    {
        ui->netText->setText(QString::fromStdString("Сеть доступна"));
        ui->netImg->setPixmap(QPixmap(":/data/images/connected.png"));

    }
    reply->deleteLater();

}

void MainWindow::replyFinished(QNetworkReply *)
{

}

void MainWindow::updateUserTimeLine()
{
    //ui->myTtwitterTimeline->setText(tw->userTimeLineText());
}

void MainWindow::splashInvisible()
{
    ui->label_2->setVisible(false);
}

void MainWindow::userSearch()
{

   UserSearch *userSearchForm = new UserSearch();
   userSearchForm->getTwitterClass();
   userSearchForm->show();
}

void MainWindow::close()
{
    QApplication::quit();
}

void MainWindow::tweetSearch()
{
    TweetsSearch *search = new TweetsSearch;
    search->prepare();

}

void MainWindow::settingsShow()
{
    Settings *settingsWindow = new Settings;
    settingsWindow->setUserID(data.id);
    settingsWindow->exec();
    //connect(settingsWindow,SIGNAL(closed()),settingsWindow,SLOT(deleteLater()));
    settingsWindow->deleteLater();

}

void MainWindow::userShow()
{

    if (data.id=="")
     data = db->getData(ui->comboBox->currentText(),BY_NAME);
    tw->setUserData(data);
    tw->setUserSettings(db->getSettings(data.id));
    Requests *request = new Requests;
    Parser *dataParser = new Parser;
    QByteArray userData = request->getRequest(GET_USER,"screen_name="+data.screen_name.toStdString(),"");
    QStringList myDada = dataParser->parseUserInfo(&userData);    
    paintElements();
    ui->myTtwitterTimeline->setText(getTimeLine(0,100,"",0));
    ui->verticalLayoutWidget_3->setVisible(false);
    ui->formLayoutWidget->setVisible(true);
    ui->tweetSearchButton->setVisible(true);
    ui->photo->setVisible(true);
    ui->verticalLayoutWidget->setVisible(true);
    ui->horizontalLayoutWidget->setVisible(true);
    ui->tabWidget->setVisible(true);
    pages = db->countRecordsInTimeLine(data.id,"",VIRTUAL_TIMELINE);
    ui->myVirtualTimeline->setText(getTimeLine(leftLimit,rightLimit,"",1));
    lastMyTweet = db->getLastTweetID(data.twitterID);
    lastMyVirtualTweet = db->getLastTweetID("");
    ui->syncLabel->setVisible(true);
    ui->syncText->setVisible(true);
    lastSynchronization = QTime::currentTime();
    tw->setLastSyncTime(lastSynchronization);
    Synchronization *sync = new Synchronization;
    connect(sync,SIGNAL(synchronizationFinished()), this, SLOT(syncFinished()));
    sync->startSynchronization(data.id, data.twitterID);
    QTimer *syncTimer = new QTimer;
    connect(syncTimer,SIGNAL(timeout()), this, SLOT(syncNeeded()));
    syncTimer->start(tw->getUserSettings()->refreshTime.toInt()*60000);
    delete request;
    delete dataParser;
}

void MainWindow::getNewUserData()
{
    QString lastID = db->getLastID();
    Requests *request = new Requests;
    Parser *dataParser = new Parser;
    data = db->getData(lastID,BY_ID);
    QByteArray userData = request->getRequest(GET_USER,"screen_name=",data.screen_name.toStdString());
    QStringList myDada = dataParser->parseUserInfo(&userData);
    data.twitterID = myDada.value(0);
    data.name = myDada.value(1);
    data.description = myDada.value(3);
    data.statuses_count = "0";
    data.friends_count = myDada.value(5);
    data.followers_count = myDada.value(6);
    data.profile_image_url = myDada.value(7);
    data.profile_image_data = request->getImage(myDada.value(7));
    connect(db, SIGNAL(workFinished()),this, SLOT(userShow()));
    db->updateNewUserData(lastID,&data);

}

void MainWindow::moved()
{
    if (ui->myVirtualTimeline->verticalScrollBar()->value()== ui->myVirtualTimeline->verticalScrollBar()->maximum())
    {   leftLimit +=100;
        rightLimit+=100;
        if (rightLimit < pages)
            ui->myVirtualTimeline->append(getTimeLine(leftLimit,rightLimit,"",1));
    }
}

void MainWindow::myUserClicked()
{
    ReadableUsers *myUsers = new ReadableUsers;
    myUsers->show();

}

void MainWindow::syncNeeded()
{
    lastMyTweet = db->getLastTweetID(data.twitterID);
    lastMyVirtualTweet = db->getLastTweetID("");
    ui->syncLabel->setVisible(true);
    ui->syncText->setVisible(true);
    Synchronization *sync = new Synchronization;
    connect(sync,SIGNAL(synchronizationFinished()), this, SLOT(syncFinished()));
    sync->startSynchronization(data.id,data.twitterID);

}

void MainWindow::syncFinished()
{

    ui->syncLabel->setVisible(false);
    ui->syncText->setVisible(false);    
    Synchronization *sync = qobject_cast<Synchronization *>(sender());
    tw->setSyncedUsers(sync->getUpdatedUsers());
    tw->setSyncedTimelines(sync->getUpdatedTimelines());
    tw->setLastSyncTime(QTime::currentTime());    
    sync->deleteLater();
    updateData();

}

void MainWindow::updateData()
{
    QStringList syncUserData;
    QStringList syncTimelines;
    if (lastSynchronization  != tw->getLastSyncTime())
    {
        syncTimelines =  tw->getSyncedTimelines();
        syncUserData = tw->getSyncedUsers();
        if (syncUserData.count() > 0)
        {
            for (int i=0; i<syncUserData.count(); i++)
            {
                if (syncUserData.at(i) == data.twitterID)
                {
                    Twitter::userData dataToSync = db->getData(data.id,BY_ID);
                    data.name = dataToSync.name;
                    data.screen_name = dataToSync.screen_name;
                    data.description = dataToSync.description;
                    data.profile_image_data = dataToSync.profile_image_data;
                    data.statuses_count = dataToSync.statuses_count;
                    data.friends_count = dataToSync.friends_count;
                    data.followers_count = dataToSync.followers_count;
                    paintElements();
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
                if (syncTimelines.at(i) == data.twitterID)
                {
                    records = db->countRecordsInTimeLine(data.twitterID,lastMyVirtualTweet,USER_TIMELINE);
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
                            scrollbarvalue = ui->myTtwitterTimeline->verticalScrollBar()->maximum()-ui->myTtwitterTimeline->verticalScrollBar()->value();
                            previousText = ui->myTtwitterTimeline->toHtml();
                            ui->myTtwitterTimeline->setText(getTimeLine(i*100,i*100+100,lastMyTweet,0)+previousText);
                            ui->myTtwitterTimeline->verticalScrollBar()->setValue(ui->myTtwitterTimeline->verticalScrollBar()->maximum()-scrollbarvalue);
                        }
                    }
                    else
                    {
                        scrollbarvalue = ui->myTtwitterTimeline->verticalScrollBar()->maximum()-ui->myTtwitterTimeline->verticalScrollBar()->value();
                        previousText = ui->myTtwitterTimeline->toHtml();
                        ui->myTtwitterTimeline->setText(getTimeLine(0,100,lastMyTweet,0)+previousText);
                        ui->myTtwitterTimeline->verticalScrollBar()->setValue(ui->myTtwitterTimeline->verticalScrollBar()->maximum()-scrollbarvalue);
                    }
                }                
            }
            records = db->countRecordsInTimeLine(data.id,lastMyVirtualTweet,VIRTUAL_TIMELINE);
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
                    scrollbarvalue = ui->myVirtualTimeline->verticalScrollBar()->maximum()-ui->myVirtualTimeline->verticalScrollBar()->value();
                    previousText = ui->myVirtualTimeline->toHtml();
                    ui->myVirtualTimeline->setText(getTimeLine(i*100,i*100+100,lastMyVirtualTweet,1)+previousText);
                    ui->myVirtualTimeline->verticalScrollBar()->setValue(ui->myVirtualTimeline->verticalScrollBar()->maximum()-scrollbarvalue);
                }
            }
            else
            {
                scrollbarvalue = ui->myVirtualTimeline->verticalScrollBar()->maximum()-ui->myTtwitterTimeline->verticalScrollBar()->value();
                previousText = ui->myVirtualTimeline->toHtml();
                ui->myVirtualTimeline->setText(getTimeLine(0,100,lastMyVirtualTweet,1)+previousText);
                ui->myVirtualTimeline->verticalScrollBar()->setValue(ui->myVirtualTimeline->verticalScrollBar()->maximum()-scrollbarvalue);
            }
        }
        lastSynchronization = tw->getLastSyncTime();
    }
}

void MainWindow::replyFinished()
{
    QNetworkAccessManager *manager = qobject_cast<QNetworkAccessManager *>(sender());
    manager->deleteLater();
}

void MainWindow::closeEvent(QCloseEvent *)
{

    QApplication::exit();
}

QString MainWindow::getTimeLine(int left,int right, QString maxTweetID, int type)
{
    QList<Twitter::tweetsData> tweets;
    if (type == 1)
        tweets = db->getTimeline(maxTweetID, data.id,left,right,VIRTUAL_TIMELINE);
    else
        tweets = db->getTimeline(maxTweetID, data.twitterID,left,right,USER_TIMELINE);
    QString result;
    result.append("<style>.select {font-weight: 600;} </style>");
    for(int i=0; i<tweets.count(); i++)
    {
        if( i<tweets.count()-1)
            result.append("<span class=\"select\">"+tweets.at(i).tweetTime+" "+tweets.at(i).username + "</span>:" +tweets.at(i).text+";<br>");
        else
            result.append("<span class=\"select\">"+tweets.at(i).tweetTime+" "+tweets.at(i).username + "</span>:" +tweets.at(i).text+";");


    }
    return result;
}

void MainWindow::paintElements()
{
    ui->username->setText( data.name);
    ui->displayname->setText( data.screen_name);
    ui->description->setText( data.description);
    ui->tweets->setText("Твиты: "+ data.statuses_count);
    ui->friends->setText("Друзья: "+ data.friends_count);
    ui->followers->setText("Читатели: "+ data.followers_count);
    userPhoto.loadFromData( data.profile_image_data);
    ui->photo->setPixmap(userPhoto);
}
