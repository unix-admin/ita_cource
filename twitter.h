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
#include "database.h"

class Twitter : public QObject
{
    Q_OBJECT
public:
    Twitter(QTextEdit *edit);
    Twitter();
    QUrl getRequestToken();
    void setOauthToken(std::string response);
    QUrl getPIN();
    QUrl accessToken();
    void setPin(std::string PIN);
    void setAccessToken(std::string response);    
    QUrl generateQueryString(std::string url, std::string parameters);
    void setUserData(DataBase::userData data);
    void setUserSettings(DataBase::userSettings settings);
    DataBase::userData *getUserData();
    DataBase::userSettings *getUserSettings();
struct requestParamerers{
    QByteArray name;
    QByteArray value;
};
public slots:
        void fin();


signals: finished();

private : std::string key;
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
          QByteArray nonce();
          QMap<QString, QVariant> *userTimeLineMap;
          QString *userTimeLine;
          QTextEdit *returnText;
          DataBase::userData userData;
          DataBase::userSettings userSettings;

};

#endif // TWITTER_H
