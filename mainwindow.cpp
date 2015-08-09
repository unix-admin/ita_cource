#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{    
    ui->setupUi(this);    
    ui->centralWidget->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->myTtwitterTimeline->setReadOnly(true);
    tw = new Twitter(ui->myTtwitterTimeline);
    QPixmap myImg(":/data/splash.jpg");    
    ui->label_2->setPixmap(myImg);
    QTimer *timer = new QTimer;
    timer->start(20000);
    connect(timer,SIGNAL(timeout()), this, SLOT(networkConnection()));
    ui->statusBar->insertPermanentWidget(0, ui->netImg ,0 );
    ui->statusBar->insertPermanentWidget(0, ui->netText ,0 );
    ui->pushButton->setEnabled(false);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));    
    connect(ui->myTtwitterTimeline,SIGNAL(textChanged()),this, SLOT(splashInvisible()));
    connect(ui->searchButton,SIGNAL(clicked()),SLOT(userSearch()));
    connect(ui->centralWidget,SIGNAL(destroyed(QObject*)),this,SLOT(close()));
    connect(ui->tweetSearchButton,SIGNAL(clicked()),SLOT(tweetSearch()));   
    connect(ui->settingsButton,SIGNAL(clicked(bool)),this, SLOT(settingsShow()));
    connect(tw,SIGNAL(finished()),this, SLOT(userShow()));
    connect(ui->myVirtualTimeline->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(moved()));
    networkConnection();
    db = DataBase::getInstance();
    leftLimit=0;
    rightLimit=100;
    connect(db,SIGNAL(userAdded()),this,SLOT(getNewUserData()));
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
    delete ui;
}

void MainWindow::buttonClicked()
{
    if (ui->comboBox->currentText() == "Новый пользователь")
    {
        autorize *newUser = new autorize;
        newUser->getAutorisation(tw);
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
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    QNetworkRequest request;
    request.setUrl(QUrl("https://twitter.com/login"));
    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");
    QNetworkReply *reply = manager->get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),SLOT(networkError()));
    connect(reply,SIGNAL(finished()),this,SLOT(networkOk()));
}

void MainWindow::networkError()
{
      netError = true;
}

void MainWindow::networkOk()
{

    if (netError)
    {
        ui->netText->setText(QString::fromStdString("Сеть недоступна"));
        ui->netImg->setPixmap(QPixmap(":/data/images/disconnected.png"));
        ui->pushButton->setEnabled(false);
    }
    else
    {
        ui->netText->setText(QString::fromStdString("Сеть доступна"));
        ui->netImg->setPixmap(QPixmap(":/data/images/connected.png"));
        ui->pushButton->setEnabled(true);
    }

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
   userSearchForm->getTwitterClass(tw);
   userSearchForm->show();
}

void MainWindow::close()
{
    QApplication::quit();
}

void MainWindow::tweetSearch()
{
    TweetsSearch *searchTweet = new TweetsSearch;
    searchTweet->prepare(tw);

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
    QPixmap userPhoto;
    if (data.id=="")
     data = db->getData(ui->comboBox->currentText(),BY_NAME);
    tw->setUserData(data);
    tw->setUserSettings(db->getSettings(data.id));
    Requests *request = new Requests;
    Parser *dataParser = new Parser;
    QByteArray userData = request->getRequest(GET_USER,"screen_name="+data.screen_name.toStdString(),"",tw);
    QStringList myDada = dataParser->parseUserInfo(&userData);
    ui->username->setText( data.name);
    ui->displayname->setText( data.screen_name);
    ui->description->setText( data.description);
    ui->tweets->setText("Твиты: "+ data.statuses_count);
    ui->friends->setText("Друзья: "+ data.friends_count);
    ui->followers->setText("Читатели: "+ data.followers_count);
    userPhoto.loadFromData( data.profile_image_data);
    ui->photo->setPixmap(userPhoto);
    userData = request->getRequest(GET_HOME_TIMELINE,"","",tw);
    ui->myTtwitterTimeline->setText(dataParser->parseTweets(&userData));
    ui->verticalLayoutWidget_3->setVisible(false);
    ui->formLayoutWidget->setVisible(true);
    ui->tweetSearchButton->setVisible(true);
    ui->photo->setVisible(true);
    ui->verticalLayoutWidget->setVisible(true);
    ui->horizontalLayoutWidget->setVisible(true);
    ui->tabWidget->setVisible(true);
    pages = db->countRecordsInVirtualTimeLine(data.id);
    ui->myVirtualTimeline->setText(getVirtualTimeLine(leftLimit,rightLimit));
    delete request;
    delete dataParser;
}

void MainWindow::getNewUserData()
{
    QString lastID = db->getLastID();
    Requests *request = new Requests;
    Parser *dataParser = new Parser;
    data = db->getData(lastID,BY_ID);
    QByteArray userData = request->getRequest(GET_USER,"screen_name=",data.screen_name.toStdString(),tw);
    QStringList myDada = dataParser->parseUserInfo(&userData);
    data.twitterID = myDada.value(0);
    data.name = myDada.value(1);
    data.description = myDada.value(3);
    data.statuses_count = myDada.value(4);
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
            ui->myVirtualTimeline->append(getVirtualTimeLine(leftLimit,rightLimit));
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{

    QApplication::exit();
}

QString MainWindow::getVirtualTimeLine(int left,int right)
{
    QList<DataBase::tweetsData> tweets;
    tweets = db->getVirtualTimeline(data.id,left,right);
    QString result;
    result.append("<style>.select {font-weight: 600;} </style>");
    for(int i=0; i<tweets.count(); i++)
    {
        result.append("<span class=\"select\">"+tweets.at(i).tweetTime+" "+tweets.at(i).username + "</span>:" +tweets.at(i).text+";<br>");
    }
    return result;
}
