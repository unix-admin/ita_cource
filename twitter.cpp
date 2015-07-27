#include "twitter.h"
#include <fstream>
#include <QDebug>
#include <QString>
#include <QUrlQuery>
#include <ctime>
#include <iostream>
#include <QByteArray>
#include <QJsonDocument>
//#include <QJsonObject>
//#include <QJsonArray>
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
    request_token = new OAuth::Token;
    accessTokenKey = "3370490014-9Mxv22sir4OgEGYgLkVMuux9u6u7W6aRqPzFBXg";
    accessTokenSecret="mGIf3QINhI2ZaIAjy9u1klq2pnhcmrPajapMQ3P89Bx7U";
    displayName = "";
    userTimeLineMap = new QMap<QString, QVariant>;
    userTimeLine = new QString;
    returnText = edit;
    userSearchResultByPage = 5;

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
    qDebug() << oauthClient->returnSignature().c_str();
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

void Twitter::setAccessToken(std::string response)
{
     OAuth::KeyValuePairs access_token_resp_data = OAuth::ParseKeyValuePairs(response);
     OAuth::Token access_token = OAuth::Token::extract( access_token_resp_data );
     accessTokenKey = access_token.key();
     accessTokenSecret= access_token.secret();
     std::pair<OAuth::KeyValuePairs::iterator, OAuth::KeyValuePairs::iterator> screen_name_its = access_token_resp_data.equal_range("screen_name");
         for(OAuth::KeyValuePairs::iterator it = screen_name_its.first; it != screen_name_its.second; it++)
            displayName = it->second;
}

void Twitter::getUserTimeline()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    QNetworkRequest request;
    std::string oauth_protected_resource = "https://api.twitter.com/1.1/statuses/home_timeline.json";
    std::string oauth_protected_resource_params = "count=100";
    OAuth::Consumer consumer(key, secret);
    OAuth::Token token(accessTokenKey, accessTokenSecret);
    OAuth::Client oauth1(&consumer, &token);
    std::string oAuthQueryString = oauth1.getURLQueryString(OAuth::Http::Get, oauth_protected_resource + "?" + oauth_protected_resource_params);
    request.setUrl(QUrl(QString::fromStdString("https://api.twitter.com/1.1/statuses/home_timeline.json?"+oAuthQueryString)));
    QNetworkReply* reply= manager->get(request);
    connect( reply, SIGNAL(finished()),this, SLOT(replyFinished()));
    qDebug() << oauth_protected_resource.c_str()<< "?" << oAuthQueryString.c_str();
}

QString Twitter::userTimeLineText()
{
    return *userTimeLine;
}

int Twitter::getuserSearchResultByPage()
{
    return userSearchResultByPage;
}

void Twitter::userSerch(QString userName)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    QNetworkRequest request;
    std::string oauth_protected_resource = "https://api.twitter.com/1.1/users/search.json";
    std::string oauth_protected_resource_params =  "q="+QUrl::toPercentEncoding(userName).toStdString()+"&count=5";
    OAuth::Consumer consumer(key, secret);
    OAuth::Token token(accessTokenKey, accessTokenSecret);
    OAuth::Client oauth1(&consumer, &token);
    std::string oAuthQueryString = oauth1.getURLQueryString(OAuth::Http::Get, oauth_protected_resource + "?" + oauth_protected_resource_params);
    request.setUrl(QUrl(QString::fromStdString("https://api.twitter.com/1.1/users/search.json?"+oAuthQueryString)));
    QNetworkReply* reply= manager->get(request);
    connect( reply, SIGNAL(finished()),this, SLOT(userSearchFinished()));
}


void Twitter::fin()
{
    qDebug()<< "Finished";
}

void Twitter::parseUserTimelineFinished()
{

    returnText->setHtml(*userTimeLine);
    returnText->setVisible(true);
}

void Twitter::userSearchFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray qb = "{\"fuckingJsonFromTwitter\": ";
        qb.append(reply->readAll());
        qb.append("}");
        *userTimeLineMap = QJsonDocument::fromJson(qb).toVariant().toMap();
        parseUserTimeline(userTimeLineMap);
      }
    else qDebug()<<reply->errorString();
    reply->deleteLater();
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


void Twitter::replyFinished()
{
      QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
      if (reply->error() == QNetworkReply::NoError)
        {
          QByteArray qb = "{\"fuckingJsonFromTwitter\": ";
          qb.append(reply->readAll());
          qb.append("}");          
          *userTimeLineMap = QJsonDocument::fromJson(qb).toVariant().toMap();
          parseUserTimeline(userTimeLineMap);
        }
      else qDebug()<<reply->errorString();
      reply->deleteLater();
}

void Twitter::parseUserTimeline(QMap<QString, QVariant> *map)
{

   QMap<QString, QVariant>::const_iterator user;
   QMap<QString, QVariant>::const_iterator text;
   QMap<QString, QVariant>::const_iterator createDate;
   QMap<QString, QVariant>::const_iterator userName;
   QList<QVariant> tweets;
   QString timeLine;
   timeLine.append("<style>.select {font-weight: 600;} </style>");
   QMap<QString, QVariant> mymap;
   QMap<QString, QVariant> userMap;
   QMap<QString, QVariant>::const_iterator iterator = map->constBegin();
   tweets = iterator.value().toList();

   for ( int i = 0 ; i<tweets.count(); i++)
   {
      mymap = tweets.at(i).toMap();
      user = mymap.find("user");
      text =  mymap.find("text");
      createDate=  mymap.find("created_at");
      userMap = user.value().toMap();
      userName= userMap.find("screen_name");
      timeLine.append("<span class=\"select\">"+createDate.value().toString()+" "+userName.value().toString() + "</span>:" + text.value().toString()+";<br>");

   }
   *userTimeLine = timeLine;
   emit parseUserTimelineFinished();
}


