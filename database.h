#include "twitter.h"
#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStringList>


enum userparameters{
     BY_NAME =0
    ,BY_ID
    ,BY_DISPLAY_NAME
    ,BY_TWITTER_ID
};

enum queryTypes{
     NEW_USER=0
    ,USER_TO_READ
    ,VIRTUAL_TIMELINE
    ,USER_TIMELINE
};

class DataBase : public QObject
{
    Q_OBJECT
public:
    struct tweets
    {
        QString tweetID;
        QString tweetTime;
        QString username;
        QString text;
        QString twitterUserID;        
    };
     DataBase();
    void createDatabase();
    QStringList getUsers();
    Twitter::userSettings getSettings(QString userID);
    void setSettings(QString userID, QStringList settings);
    Twitter::userData getData (QString parameter, userparameters type);
    void addNewUser(QString accessToken, QString accessTokenSecret, QString displayName);
    bool checkUser(QString parameter, userparameters queryType);
    bool checkReadableUser(QString userID, QString twitterID);
    QString getLastID();
    void updateNewUserData(QString parameter, Twitter::userData *data);
    void addReadableUser(Twitter::userData *data, QString senderID, queryTypes typeQuery);
    void insertTweetsToDatabase(QList<tweets> *dataToInsert);
    QList<Twitter::tweetsData> getTimeline(QString maxTweetID, QString userID, int leftLimit, int rightLimit, queryTypes type);
    void deleteUser(QString twitterID, QString readerID);
    int countRecordsInTimeLine(QString userID, QString maxTweetID, queryTypes type);
    int countReadableUsers(QString userID);
    void updateUserData(QString twitterID, QString parameter, QVariant value);
    QList<Twitter::userData> getReadableUsers(QString userID);
    QStringList getUsersForSync(QString userID);
    QString getLastTweetID(QString twitterID);

private:    
    void connect();    
    void disconnect();
    Twitter *twitter;
    QSqlDatabase twitterDB;
private slots:
    void process();

signals:
    workFinished();
    userAdded();
};


#endif // DATABASE_H
