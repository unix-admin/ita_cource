#ifndef PARSER_H
#define PARSER_H
#include <QMovie>
#include <QMap>
#include <QJsonDocument>
#include <QByteArray>
#include <QString>
#include <QVariant>
//#include <QList>
#include <QStringList>
#include <QObject>
#include "requests.h"
#include "twitter.h"
class Parser : public QObject
{
    Q_OBJECT
public:
    Parser();
    QStringList parseSearchRequest(QByteArray *data);
    QString parseTweets(QByteArray *data);
    void parseSearchMetadata(QByteArray *data, QStringList *listData);
    QString maxTweetID;
    QString minTweetID;
    QStringList parseUserInfo(QByteArray *data);
    QList<QVariant> parseTweetsToDatabase(QByteArray *data);
    QString dateFormat(QVariant value);
    Twitter::userData userinfo(QByteArray *data);
    QList<Twitter::tweetsData> tweets (QByteArray *data);

signals:
    parseFinished();
public slots:
    void quit();
private :

};

#endif // PARSER_H
