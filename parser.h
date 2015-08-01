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
signals:
    parseFinished();
public slots:
    void quit();
private :
    QString getmaxTweetID(QList<QVariant> tweetsList);
    QString getminTweetID(QList<QVariant> tweetsList);
};

#endif // PARSER_H
