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
    struct userData
    {
        QString id;
        QString name;
        QString twitterID;
        QString accessTokenKey;
        QString accessTokenSecret;
        QString screen_name;
        QString description;
        QString statuses_count;
        QString friends_count;
        QString followers_count;
        QString profile_image_url;
        QByteArray profile_image_data;
    };
    struct userSettings
    {
        QString timelineTweetsByPage;
        QString searchTweetsByPage;
        QString searchUsersByPage;
        QString searchTweetsToDatabase;
        QString refreshTime;
    };
    struct tweetsData
    {
        QString tweetID;
        QString tweetTime;
        QString username;
        QString text;
        QString twitterUserID;
        QString searchID;
    };
    static DataBase * getInstance() {
            if(!p_instance)
                p_instance = new DataBase();
            return p_instance;
    }
    QStringList getUsers();
    userSettings getSettings(QString userID);
    void setSettings(QString userID, QStringList settings);
    userData getData (QString parameter, userparameters type);
    void addNewUser(QString accessToken, QString accessTokenSecret, QString displayName);
    bool checkUser(QString parameter, userparameters queryType);
    bool checkReadableUser(QString userID, QString twitterID);
    QString getLastID();
    void updateNewUserData(QString parameter, userData *data);
    void addReadableUser(userData *data, QString senderID, queryTypes typeQuery);
    void insertTweetsToDatabase(tweetsData *dataToInsert);
    QList<tweetsData> getTimeline(QString maxTweetID, QString userID, int leftLimit, int rightLimit, queryTypes type);
    void deleteUser(QString twitterID, QString readerID);
    int countRecordsInVirtualTimeLine(QString userID, QString maxTweetID);
    int countReadableUsers(QString userID);
    void updateUserData(QString twitterID, QString parameter, QVariant value);
    QList<userData> getReadableUsers(QString userID);
    QStringList getUsersForSync(QString userID);
    QString getLastTweetID(QString twitterID);

private:
    static DataBase * p_instance;
    DataBase();
    DataBase( const DataBase& );
    DataBase& operator=( DataBase& );
    void connect();
    QSqlDatabase tweetsDB;
    void disconnect();

signals:
    workFinished();
    userAdded();
};


#endif // DATABASE_H
