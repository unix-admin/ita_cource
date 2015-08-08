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
    bool checkUser(QString displayName);
    QString getLastID();
    void updateNewUserData(QString parameter, userData *data);
    void addReadableUser(userData *data, QString senderID);

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
