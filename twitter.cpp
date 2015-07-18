#include "twitter.h"
#include <fstream>
#include <QDebug>
#include <QString>
#include <QUrlQuery>

Twitter::Twitter()
{
    key = "GgqXEJQD6rglvtLRnSGGPgB75"; // Key from Twitter
    secret = "hffrwdXOxsdOZ14JDPONP3VbhPGcYAuLkCAfSC6k1JJPMmjEpi"; // Secret from Twitter
    request_token_url = "https://api.twitter.com/oauth/request_token";
    request_token_query_args = "oauth_callback=oob";
    authorize_url = "https://api.twitter.com/oauth/authorize";
    access_token_url = "https://api.twitter.com/oauth/access_token";    
    oauthConsumer = new OAuth::Consumer(key,secret);
    oauthClient = new OAuth::Client(oauthConsumer);
    request_token = new OAuth::Token;
    accessTokenKey = "";
    accessTokenSecret="";
    displayName = "";


}

QUrl Twitter::getRequestToken(){

    std::string base_request_token_url = request_token_url + (request_token_query_args.empty() ? std::string("") : (std::string("?")+request_token_query_args) );
    std::string oAuthQueryString = oauthClient->getURLQueryString( OAuth::Http::Get, base_request_token_url);
    std::string url = request_token_url + "?" + oAuthQueryString;    
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

void Twitter::setAccesshToken(std::string response)
{
     OAuth::KeyValuePairs access_token_resp_data = OAuth::ParseKeyValuePairs(response);
     OAuth::Token access_token = OAuth::Token::extract( access_token_resp_data );
     accessTokenKey = access_token.key();
     accessTokenSecret= access_token.secret();
     std::pair<OAuth::KeyValuePairs::iterator, OAuth::KeyValuePairs::iterator> screen_name_its = access_token_resp_data.equal_range("screen_name");
         for(OAuth::KeyValuePairs::iterator it = screen_name_its.first; it != screen_name_its.second; it++)
            displayName = it->second;

     qDebug() << accessTokenKey.c_str();
     qDebug() << accessTokenSecret.c_str();
     qDebug() << displayName.c_str();

}


