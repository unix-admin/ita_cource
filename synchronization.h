#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H
#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QByteArray>

#include "twitter.h"
#include "database.h"
#include "requests.h"
#include "parser.h"

class Synchronization : public QObject
{
    Q_OBJECT
public:
    Synchronization();
    ~Synchronization();
    void startSynchronization(QString userID, QString twitterID);
    QStringList getUpdatedTimelines();
    QStringList getUpdatedUsers();

private:
    Twitter *clsTwitter;
    DataBase *clsDatabase;
    Requests *requestor;
    Parser *parser;
    QByteArray requestData;
    QList<QVariant> tweets;
    QStringList updatedUsers;
    QStringList updatedTimelines;
private:
    void syncMyReadableUsers(QString userID);
    void getTimeline(QString twitterUserID);
    void tweetsToDatabase(QList<QVariant> *tweets, QString twitterUserID);    
    void checkUserInfo(QString twitterUserID);

signals:
    synchronizationFinished();
};

#endif // SYNCHRONIZATION_H
