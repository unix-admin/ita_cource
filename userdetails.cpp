#include "userdetails.h"
#include "ui_userdetails.h"
#include <QMovie>
#include <QLayout>
#include <QLabel>

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
    ui->textBrowser->setVisible(false);
    ui->horizontalLayoutWidget->setVisible(false);
    ui->gridLayoutWidget->setVisible(false);
    movie->start();
    connect(ui->buttonToDatabase,SIGNAL(clicked(bool)),this, SLOT(toDatabase()));
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
    info = getInfo->getRequest(GET_USER_TIMELINE,id,"&count="+twitter->getUserSettings()->timelineTweetsByPage.toStdString(),twitter);
    timeLine = parseInfo->parseTweets(&info);
    ui->textBrowser->setText(timeLine);
    this->resize(400,580);
    ui->label->resize(128,128);
    ui->label->setPixmap(photo);
    ui->horizontalLayoutWidget->setVisible(true);
    ui->gridLayoutWidget->setVisible(true);
    ui->textBrowser->setVisible(true);
    if ((db->checkUser(userData.screen_name)))
            ui->buttonToDatabase->setVisible(false);
    delete getInfo;
    delete parseInfo;
}

void UserDetails::toDatabase()
{
    db->addReadableUser(&userData,twitter->getUserData()->id);
    ui->buttonToDatabase->setVisible(false);
}

