#include "twitter.h"
#include <fstream>
#include <QDebug>
#include <QString>
#include <QUrlQuery>
#include <ctime>
#include <iostream>
#include <QByteArray>
#include <QJsonDocument>
#include <QVariant>
#include <QMap>

Twitter::Twitter(QTextEdit *edit)
{
    key = "GgqXEJQD6rglvtLRnSGGPgB75"; // Key from Twitter
    secret = "hffrwdXOxsdOZ14JDPONP3VbhPGcYAuLkCAfSC6k1JJPMmjEpi"; // Secret from Twitter
    request_token_url = "https://api.twitter.com/oauth/request_token";
    request_token_query_args = "oauth_callback=oob";
    authorize_url = "https://api.twitter.com/oauth/authorize";
    access_token_url = "https://api.twitter.com/oauth/access_token";
    oauthSignature = "";
    oauthConsumer = new OAuth::Consumer(key,secret);
    oauthClient = new OAuth::Client(oauthConsumer);
    userTimeLineMap = new QMap<QString, QVariant>;
    userTimeLine = new QString;
    returnText = edit;
    userData.accessTokenKey = "";
    userData.accessTokenSecret = "";
    request_token = new OAuth::Token;
}

Twitter::Twitter()
{

}

QUrl Twitter::getRequestToken()
{
    std::string base_request_token_url = request_token_url + (request_token_query_args.empty() ? std::string("") : (std::string("?")+request_token_query_args) );
    std::string oAuthQueryString = oauthClient->getURLQueryString( OAuth::Http::Get, base_request_token_url);
    std::string url = request_token_url + "?" + oAuthQueryString;
    QString temp =  QString::fromStdString(url).replace("#", "?");
    oauthSignature = QUrlQuery(temp).queryItemValue("oauth_signature").toStdString();    
    return QString::fromStdString(url);
}

void Twitter::setOauthToken(std::string response)
{
    *request_token = OAuth::Token::extract(response);
}

QUrl Twitter::getPIN()
{
    QUrl result;
    result = QString::fromStdString( authorize_url+"?oauth_token="+request_token->key());
    return result;
}

QUrl Twitter::accessToken()
{
    QUrl result;
    *oauthClient = OAuth::Client(oauthConsumer, request_token);
    result = QString::fromStdString(access_token_url + "?"+oauthClient->getURLQueryString( OAuth::Http::Get, access_token_url, std::string( "" ), true ));
    return result;
}

void Twitter::setPin(std::string PIN)
{
 request_token->setPin(PIN);

}

QUrl Twitter::generateQueryString(std::string url, std::string parameters)
{
    OAuth::Consumer consumer(key, secret);
    OAuth::Token token(userData.accessTokenKey.toStdString(), userData.accessTokenSecret.toStdString());
    OAuth::Client oauthClient(&consumer, &token);
    std::string oAuthQueryString = oauthClient.getURLQueryString(OAuth::Http::Get, url + "?" + parameters);
    return QUrl(QString::fromStdString(url+"?"+oAuthQueryString));
}

void Twitter::setUserData(DataBase::userData data)
{
    userData = data;
}

void Twitter::setUserSettings(DataBase::userSettings settings)
{
    userSettings = settings;
}

DataBase::userData *Twitter::getUserData()
{
    return &userData;
}

DataBase::userSettings *Twitter::getUserSettings()
{
    return &userSettings;
}

void Twitter::fin()
{
    qDebug()<< "Finished";
}

QByteArray Twitter::nonce()
{
        static bool firstTime = true;
        if (firstTime) {
            firstTime = false;
            qsrand(QTime::currentTime().msec());
        }
        QString u = QString::number(QDateTime::currentDateTimeUtc().toTime_t());
        u.append(QString::number(qrand()));
        return u.toLatin1();
}



