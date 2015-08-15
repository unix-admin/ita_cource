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
    ,GET_USER
    ,GET_USER_BY_ID
    ,GET_USER_TIMELINE
    ,TWEETS_SEARCH
    ,TWEETS_SEARCH_NAVIGATE

};

class Requests :public QObject
{

    Q_OBJECT


public:
    Requests();
    QByteArray getRequest(requestType type, std::string parameters, std::string parameters2);
    QByteArray getImage(QUrl requestURL);

private:
    QByteArray requestResult;
    Twitter *twitter;
private slots:
    void replyFinished();
    void managerFinished();
    void imageDownloadFinished();

};

#endif // REQUESTS_H
