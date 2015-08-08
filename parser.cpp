#include "parser.h"
#include <QDebug>


Parser::Parser()
{

}

QStringList Parser::parseSearchRequest(QByteArray *data)
{
    QStringList result;
    QMap<QString,QVariant> dataToParse;
    dataToParse = QJsonDocument::fromJson(*data).toVariant().toMap();
    QMap<QString, QVariant> metadata = dataToParse.begin().value().toMap();
    result.append(metadata.find("next_results").value().toString());
    result.append(metadata.find("refresh_url").value().toString());
    QString tweets;
    tweets = parseTweets(data);
    result.append(tweets);
    result.append(minTweetID);
    result.append(maxTweetID);
    return result;
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
        if (i==0)
        {
            maxTweetID =  mymap.find("id_str").value().toString();
        }
        if (i==tweetsList.count()-1)
        {
            minTweetID =  mymap.find("id_str").value().toString();
        }
        user = mymap.find("user");
        text =  mymap.find("text");
        createDate=  mymap.find("created_at");
        userMap = user.value().toMap();
        userName= userMap.find("screen_name");
        tweets.append("<span class=\"select\">"+dateFormat(createDate.value())+" "+userName.value().toString() + "</span>:" + text.value().toString()+";<br>");
     }
     return tweets;
}

void Parser::parseSearchMetadata(QByteArray *data, QStringList *listData)
{

    listData->clear();
    QMap<QString,QVariant> dataToParse;
    dataToParse = QJsonDocument::fromJson(*data).toVariant().toMap();
    QMap<QString, QVariant> metadata = dataToParse.begin().value().toMap();    
    listData->append(metadata.find("next_results").value().toString());
    listData->append(metadata.find("refresh_url").value().toString());
}

QStringList Parser::parseUserInfo(QByteArray *data)
{
    QStringList result;
    QMap<QString,QVariant> userInfo;
    userInfo = QJsonDocument::fromJson(*data).toVariant().toMap();
    result.append(userInfo.find("id").value().toString());
    result.append(userInfo.find("name").value().toString());
    result.append(userInfo.find("screen_name").value().toString());
    result.append(userInfo.find("description").value().toString());
    result.append(userInfo.find("statuses_count").value().toString());
    result.append(userInfo.find("friends_count").value().toString());
    result.append(userInfo.find("followers_count").value().toString());
    result.append(userInfo.find("profile_image_url").value().toString().replace("_normal","_reasonably_small"));
    Requests *imageDownload = new Requests;
    result.append(QString::fromStdString(imageDownload->getImage(QUrl(result.value(7))).toStdString()));
    return result;
}

void Parser::quit()
{


    qDebug() << "quit";
}

QString Parser::dateFormat(QVariant value)
{
    QString result;
    result = value.toString();
    result.replace("+0000","");
    //Replace week
    result.replace("Mon","Пн");
    result.replace("Tue","Вт");
    result.replace("Wed","Ср");
    result.replace("Thu","Чт");
    result.replace("Fri","Пт");
    result.replace("Sat","Сб");
    result.replace("Sun","Вс");
    //Replace month
    result.replace("Jan","Янв");
    result.replace("Feb","Фев");
    result.replace("Mar","Мар");
    result.replace("Apr","Апр");
    result.replace("May","Май");
    result.replace("Jun","Июн");
    result.replace("Jul","Июл");
    result.replace("Aug","Авг");
    result.replace("Sep","Сен");
    result.replace("Oct","Окт");
    result.replace("Nov","Ноя");
    result.replace("Dec","Дек");



    return result;
}


