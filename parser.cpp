#include "parser.h"
#include <QDebug>


Parser::Parser()
{

}

QString Parser::parseTweets(QByteArray *data)
{
     QMap<QString,QVariant> dataToParse;
     dataToParse = QJsonDocument::fromJson(*data).toVariant().toMap();
     QMap<QString, QVariant>::const_iterator user;
     QMap<QString, QVariant>::const_iterator text;
     QMap<QString, QVariant>::const_iterator createDate;
     QMap<QString, QVariant>::const_iterator userName;
     QList<QVariant> tweetsList;
     QString tweets;
     tweets.append("<style>.select {font-weight: 600;} </style>");
     QMap<QString, QVariant> mymap;
     QMap<QString, QVariant> userMap;
     QMap<QString, QVariant>::const_iterator iterator = dataToParse.constBegin();;

     if (dataToParse.count()==2)
     {
      iterator++;
     }

     tweetsList = iterator.value().toList();
     for ( int i = 0 ; i<tweetsList.count(); i++)
     {
        mymap = tweetsList.at(i).toMap();
        user = mymap.find("user");
        text =  mymap.find("text");
        createDate=  mymap.find("created_at");
        userMap = user.value().toMap();
        userName= userMap.find("screen_name");
        tweets.append("<span class=\"select\">"+createDate.value().toString()+" "+userName.value().toString() + "</span>:" + text.value().toString()+";<br>");
     }
     return tweets;
}

std::string Parser::parseSearchMetadata(QByteArray *data)
{
    QMap<QString,QVariant> dataToParse;
    dataToParse = QJsonDocument::fromJson(*data).toVariant().toMap();
    QMap<QString, QVariant> metadata = dataToParse.begin().value().toMap();
    std::string nextResults;
    nextResults = metadata.find("next_results").value().toString().toStdString();
    qDebug()<<metadata.find("next_results").value().toString();
    qDebug()<<metadata.find("refresh_url").value().toString();
    return nextResults;
}



