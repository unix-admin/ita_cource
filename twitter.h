#ifndef TWITTER_H
#define TWITTER_H
#include <string>
#include "liboauthcpp.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>

class Twitter : public QObject
{
    Q_OBJECT
public:
    Twitter();
    QUrl getRequestToken();
    void setOauthToken(std::string response);
    QUrl getPIN();
    QUrl accessToken();
    void setPin(std::string PIN);
    void setAccesshToken(std::string response);


signals: finished();

private : std::string key;
          std::string secret;
          std::string request_token_url;
          std::string request_token_query_args;
          std::string authorize_url;
          std::string access_token_url;
          std::string oauth_token;
          std::string oauth_token_secret;
          std::string accessTokenKey;
          std::string accessTokenSecret;
          std::string displayName;
          OAuth::Client *oauthClient;
          OAuth::Consumer *oauthConsumer;
          OAuth::Token *request_token;

private slots:



};

#endif // TWITTER_H
