#include "requests.h"
#include <QDebug>
Requests::Requests()
{
    twitter = Twitter::getcls();
}

QByteArray Requests::getRequest(requestType type, std::string parameters, std::string parameters2)
{
    QEventLoop loop;
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(managerFinished()));
    QNetworkRequest request;
    switch (type) {
        case GET_USER:
        {
            request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/users/show.json",parameters+parameters2));
            break;
        }
        case GET_HOME_TIMELINE:
        {
            request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/statuses/home_timeline.json",parameters+parameters2));
            break;
        }
        case GET_USER_TIMELINE:
        {
            request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/statuses/user_timeline.json","user_id="+parameters+parameters2));
            break;
        }
        case TWEETS_SEARCH:
        {
            request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/search/tweets.json","q=%23"+QUrl::toPercentEncoding(QString::fromStdString(parameters)).toStdString()+parameters2));
        break;
        }
        case TWEETS_SEARCH_NAVIGATE:
        {
            request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/search/tweets.json",parameters+parameters2));
            break;
        }
        case GET_USER_BY_ID:
        {
            request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/users/show.json","user_id="+parameters+parameters2));
            break;
        }
    }
    QNetworkReply* reply= manager->get(request);
    connect(reply, SIGNAL(finished()),this, SLOT(replyFinished()));
    connect(reply, SIGNAL(finished()),&loop, SLOT(quit()));
    loop.exec();   
    return requestResult;

}

QByteArray Requests::getImage(QUrl requestURL)
{
    QEventLoop loop;
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(managerFinished()));
    QNetworkRequest request;
    request.setUrl(requestURL);
    QNetworkReply* reply= manager->get(request);
    connect(reply, SIGNAL(finished()),this, SLOT(imageDownloadFinished()));
    connect(reply, SIGNAL(finished()),&loop, SLOT(quit()));
    loop.exec();
    return requestResult;
}

void Requests::replyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());    
    if ((reply->error() == QNetworkReply::NoError)|| (reply->errorString().contains("ssl")))
      {
        std::string request = reply->readAll().toStdString();
        if (request[0] == '[')
        {
            requestResult = "{\"fuckingJsonFromTwitter\": ";
            requestResult.append(QString::fromStdString(request));
            requestResult.append("}");
        }
        else
            requestResult.append(QString::fromStdString(request));
      }
    else qDebug()<<reply->errorString();
    reply->deleteLater();
}

void Requests::managerFinished()
{
    QNetworkAccessManager *manager = qobject_cast<QNetworkAccessManager *>(sender());
    manager->deleteLater();

}

void Requests::imageDownloadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() == QNetworkReply::NoError)
    {
        requestResult =  reply->readAll();
    }
}
