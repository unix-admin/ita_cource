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
    id = userid;
    twitter = clsTwitter;
    ui->label->resize(200,200);
    QMovie* movie = new QMovie(":/data/images/loading.gif");
    ui->label->setMovie(movie);
    ui->textBrowser->setVisible(false);
    ui->horizontalLayoutWidget->setVisible(false);
    ui->gridLayoutWidget->setVisible(false);
    movie->start();
    this->resize(200,200);
    getUserinfo(id);
    this->show();


}

void UserDetails::getUserinfo(std::string id)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest request;
    request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/users/show.json","user_id="+id));
    QNetworkReply* reply= manager->get(request);
    connect(reply, SIGNAL(finished()),this, SLOT(getUserInfoFinished()));
}

void UserDetails::getUserTimeline(std::string id)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest request;
    request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/statuses/user_timeline.json","user_id="+id+"&count=200"));
    QNetworkReply* reply= manager->get(request);
    connect(reply, SIGNAL(finished()),this, SLOT(getUserTimelineFinished()));
}


void UserDetails::getUserInfoFinished()
{
    QMap<QString,QVariant> detailedUserInfo;

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray qb;
        qb.append(reply->readAll());
        detailedUserInfo = QJsonDocument::fromJson(qb).toVariant().toMap();
        parseUserinfoResults(detailedUserInfo);
      }
    else qDebug()<<reply->errorString();
    reply->deleteLater();
}

void UserDetails::getUserTimelineFinished()
{

    QMap<QString,QVariant> timelineMap;
    QList<QVariant> results;
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray qb = "{\"fuckingJsonFromTwitter\": ";
        qb.append(reply->readAll());
        qb.append("}");
        timelineMap = QJsonDocument::fromJson(qb).toVariant().toMap();
        results.append(timelineMap.begin().value().toList());
        parseUserTimelineResults(results);
      }
    else qDebug()<<reply->errorString();
    reply->deleteLater();

}

void UserDetails::imageDownloadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    photo.loadFromData(reply->readAll());

    reply->deleteLater();
}

void UserDetails::parseUserinfoResults(QMap<QString, QVariant> results)
{
    ui->label_2->setText(results.find("name").value().toString());
    ui->label_3->setText(results.find("screen_name").value().toString());
    ui->label_4->setText(results.find("description").value().toString());
    ui->label_5->setText("Твиты: "+results.find("statuses_count").value().toString());
    ui->label_6->setText("Друзья: "+results.find("friends_count").value().toString());
    ui->label_7->setText("Читатели: "+results.find("followers_count").value().toString());
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest request;
    QEventLoop event;
    request.setUrl(QUrl(results.find("profile_image_url").value().toString().replace("_normal","_reasonably_small")));
    manager->get(request);
    QNetworkReply* reply= manager->get(request);
    connect( reply, SIGNAL(finished()),this, SLOT(imageDownloadFinished()));
    connect( reply, SIGNAL(finished()),&event, SLOT(quit()));
    event.exec();
    getUserTimeline(id);
}

void UserDetails::parseUserTimelineResults(QList<QVariant> results)
{


    QMap<QString, QVariant>::const_iterator user;
    QMap<QString, QVariant>::const_iterator text;
    QMap<QString, QVariant>::const_iterator createDate;
    QMap<QString, QVariant>::const_iterator userName;
    //QList<QVariant> tweets;
    QString timeLine;
    timeLine.append("<style>.select {font-weight: 600;} </style>");
    QMap<QString, QVariant> mymap;
    QMap<QString, QVariant> userMap;
    for ( int i = 0 ; i<results.count(); i++)
    {
       mymap = results.at(i).toMap();
       user = mymap.find("user");
       text =  mymap.find("text");
       createDate=  mymap.find("created_at");
       userMap = user.value().toMap();
       userName= userMap.find("screen_name");
       timeLine.append("<span class=\"select\">"+createDate.value().toString()+" "+userName.value().toString() + "</span>:" + text.value().toString()+";<br>");

    }

    ui->textBrowser->setHtml(timeLine);
    this->resize(400,580);
    ui->label->resize(128,128);
    ui->label->setPixmap(photo);
    ui->horizontalLayoutWidget->setVisible(true);
    ui->gridLayoutWidget->setVisible(true);
    ui->textBrowser->setVisible(true);

}


