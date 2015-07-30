#include "requests.h"
#include <QDebug>
Requests::Requests()
{

}

QByteArray Requests::getRequest(requestType type, std::string parameters, Twitter *clsTwitter)
{
    QEventLoop loop;
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(managerFinished()));
    QNetworkRequest request;
    switch (type) {
        case GET_HOME_TIMELINE:
                break;
        case GET_USER_TIMELINE:
            break;
        case TWEETS_SEARCH:
            request.setUrl(clsTwitter->generateQueryString("https://api.twitter.com/1.1/search/tweets.json","q=%23"+QUrl::toPercentEncoding(QString::fromStdString(parameters)).toStdString()+"&count=100"));
            break;
    }
    QNetworkReply* reply= manager->get(request);
    connect(reply, SIGNAL(finished()),this, SLOT(replyFinished()));
    connect(reply, SIGNAL(finished()),&loop, SLOT(quit()));
    loop.exec();
    qDebug() << "OK";
    return requestResult;

}

void Requests::replyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() == QNetworkReply::NoError)
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
