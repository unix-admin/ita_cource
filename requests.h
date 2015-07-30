#ifndef REQUESTS_H
#define REQUESTS_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QString>
#include <QObject>
#include "twitter.h"


enum requestType{
     GET_HOME_TIMELINE=0
    ,GET_USER_TIMELINE
    ,TWEETS_SEARCH
};

class Requests :public QObject
{

    Q_OBJECT


public:
    Requests();
    QByteArray getRequest(requestType type, std::string parameters, Twitter *clsTwitter);

private:
    QByteArray requestResult;

private slots:
    void replyFinished();
    void managerFinished();

};

#endif // REQUESTS_H
