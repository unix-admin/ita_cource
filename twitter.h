#ifndef TWITTER_H
#define TWITTER_H
#include <string>
#include "liboauthcpp.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QList>
#include <QTextDocument>
#include <QTextEdit>
#include <QEventLoop>
#include <QTime>
#include <QSqlDatabase>
#include <QFile>
class Twitter : public QObject
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
        QString refreshTime;
    };
    struct tweetsData
    {
        QString tweetID;
        QString tweetTime;
        QString username;
        QString text;
        QString twitterUserID;        
    };

    struct requestParamerers{
        QByteArray name;
        QByteArray value;
    };

private:
    static Twitter *instance;
public:

    static Twitter *getcls() {
            if(!instance)
                instance = new Twitter();
            return instance;
    }
    QUrl getRequestToken();
    void setOauthToken(std::string response);
    QUrl getPIN();
    QUrl accessToken();
    void setPin(std::string PIN);
    void setAccessToken(std::string response);    
    QUrl generateQueryString(std::string url, std::string parameters);
    void setUserData(userData data);
    void setUserSettings(userSettings settings);
    userData *getUserData();
    userSettings *getUserSettings();
    void setSyncedUsers(QStringList syncUsers);
    void setSyncedTimelines(QStringList syncTimelines);
    void setLastSyncTime(QTime time);
    QStringList getSyncedUsers();
    QStringList getSyncedTimelines();
    QTime getLastSyncTime();
    QSqlDatabase getDatabase();    
    void setNetworkStatus(bool status);
    bool getNetworkStatus();
    bool getDatabaseStatus();
    void setDatabaseStatus(bool status);

private :
          Twitter();
          Twitter( const Twitter& );
          Twitter& operator=( Twitter& );
          std::string key;
          std::string secret;
          std::string request_token_url;
          std::string request_token_query_args;
          std::string authorize_url;
          std::string access_token_url;
          std::string oauth_token;
          std::string oauth_token_secret;
          std::string oauthSignature;
          OAuth::Client *oauthClient;
          OAuth::Consumer *oauthConsumer;
          OAuth::Token *request_token;
          bool networkStatus;
          QStringList syncedUsers;
          QStringList syncedTimelines;
          QTime LastSync;
          userData twitterUserData;
          userSettings twitterUserSettings;
          QSqlDatabase twitterDB;
          bool databaseStatus;
};

#endif // TWITTER_H
