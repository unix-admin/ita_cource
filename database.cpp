#include "database.h"

DataBase* DataBase::p_instance = 0;

void DataBase::connect()
{
    if (!tweetsDB.open()) {
          tweetsDB.open();
    }
}

void DataBase::disconnect()
{
    if (tweetsDB.open()) {
          tweetsDB.close();
    }
}

bool DataBase::checkUser(QString displayName)
{
    bool result = false;
    connect();
    QSqlQuery checkUserQuery("SELECT userID FROM users WHERE displayName=\""+displayName+"\"");
    if (checkUserQuery.record().count() > 0 )
        result = true;
    return result;
}

QString DataBase::getLastID()
{
    QString ID;
    QSqlQuery getLastIDQuery("SELECT userID FROM users");
    ID = getLastIDQuery.lastInsertId().toString();
     disconnect();
     return ID;
}

void DataBase::updateNewUserData(QString id, userData *data )
{
    connect();
    QSqlQuery updateUserDataQuery;
    QString queryString = "UPDATE users SET userName=\""+data->name\
            +"\", twitterID="+data->twitterID\
            +", desription=\""+data->description \
            +"\", tweets="+data->statuses_count \
            +", friends="+data->friends_count \
            +", followers="+data->followers_count \
            +", image=:image" \
            +", imageUrl=\""+data->profile_image_url +"\" WHERE userID="+id;    
    updateUserDataQuery.prepare(queryString);
    updateUserDataQuery.addBindValue(data->profile_image_data);
    updateUserDataQuery.exec();
    updateUserDataQuery.finish();
    queryString = "INSERT INTO settings (userID, timelineTweetsByPage," \
                  "searchTweetsByPage, searchUsersByPage, "\
                  "searchTweetsToDatabase, refreshTime) VALUES ("+id+",5,10,5,100,10)";
    updateUserDataQuery.prepare(queryString);
    updateUserDataQuery.exec();
    disconnect();
    emit workFinished();
}

void DataBase::addReadableUser(DataBase::userData *data, QString senderID)
{
    connect();
    QSqlQuery updateUserDataQuery;
    QString queryString = "INSERT INTO users(userName,displayName,twitterID,desription,tweets,friends,followers,image,imageUrl,usertype) VALUES(\""+data->name\
                +"\",\""+data->screen_name\
                +"\","+data->twitterID\
                +", \""+data->description \
                +"\", "+data->statuses_count \
                +", "+data->friends_count \
                +", "+data->followers_count \
                +", :image"
                +", \""+data->profile_image_url +"\",0)";
     qDebug() << queryString;
     updateUserDataQuery.prepare(queryString);
     updateUserDataQuery.addBindValue(data->profile_image_data);
        updateUserDataQuery.exec();
        updateUserDataQuery.finish();
        queryString = "INSERT INTO readableUsers (userID,twitterID) VALUES("+senderID+","+data->twitterID+")" ;
        qDebug() << queryString;
        updateUserDataQuery.prepare(queryString);
        updateUserDataQuery.exec();
        disconnect();
}


QStringList DataBase::getUsers()
{
     connect();
     QStringList result;
     QSqlQuery getUsersQuery("SELECT username FROM users WHERE usertype=1");
     while (getUsersQuery.next()) {
         result.append(getUsersQuery.value(0).toString());
     }
     result.append("Новый пользователь");
     disconnect();

     return result;
}

DataBase::userSettings DataBase::getSettings(QString userID)
{
    connect();
    userSettings result;
    QSqlQuery getUsersQuery("SELECT * FROM settings WHERE userID="+userID);
    while (getUsersQuery.next()) {
        result.timelineTweetsByPage = getUsersQuery.value(1).toString();
        result.searchTweetsByPage = getUsersQuery.value(2).toString();
        result.searchUsersByPage = getUsersQuery.value(3).toString();
        result.searchTweetsToDatabase = getUsersQuery.value(4).toString();
        result.refreshTime = getUsersQuery.value(5).toString();
    }
    disconnect();
    return result;
}

void DataBase::setSettings(QString userID, QStringList settings)
{
    connect();
    QSqlQuery setSettingsQuery;    
    setSettingsQuery.exec("UPDATE settings SET timelineTweetsByPage="+settings.value(0)\
                          +", searchTweetsByPage="+settings.value(1)\
                          +", searchUsersByPage="+settings.value(2) \
                          +", searchTweetsToDatabase="+settings.value(3) \
                          + ", refreshTime="+settings.value(4)+" WHERE userID="+userID);

    disconnect();
    emit workFinished();
}

DataBase::userData DataBase::getData(QString parameter , userparameters type)
{
    DataBase::userData result;
    connect();
    QString queryString;
    switch (type) {
    case BY_NAME:
    {
        queryString = "SELECT * FROM users WHERE username='"+parameter+"'";
        break;
    }
    case BY_ID:
    {
        queryString = "SELECT * FROM users WHERE userID='"+parameter+"'";
        break;
    }
    case BY_DISPLAY_NAME:
    {
        queryString = "SELECT * FROM users WHERE displayName='"+parameter+"'";
        break;
    }
    }
    QSqlQuery getUserParametersQuery(queryString);
    while (getUserParametersQuery.next()) {
        result.id = getUserParametersQuery.value(0).toString();
        result.name = getUserParametersQuery.value(1).toString();
        result.twitterID = getUserParametersQuery.value(2).toString();
        result.accessTokenKey = getUserParametersQuery.value(3).toString();
        result.accessTokenSecret = getUserParametersQuery.value(4).toString();
        result.screen_name = getUserParametersQuery.value(5).toString();
        result.description = getUserParametersQuery.value(6).toString();                
        result.profile_image_data = getUserParametersQuery.value(7).toByteArray();
        result.profile_image_url = getUserParametersQuery.value(8).toString();
        result.statuses_count = getUserParametersQuery.value(9).toString();
        result.friends_count = getUserParametersQuery.value(10).toString();
        result.followers_count = getUserParametersQuery.value(11).toString();
    }
    disconnect();
    return result;
}

void DataBase::addNewUser(QString accessToken, QString accessTokenSecret, QString displayName)
{
    connect();
    QSqlQuery addNewUserQuery("INSERT INTO users (accessTokenKey, accessTokenSecret, displayName, usertype) VALUES(\""+accessToken+"\",\""+accessTokenSecret+"\",\""+displayName+"\",1)");
    addNewUserQuery.finish();
    emit userAdded();
}

DataBase::DataBase()
{
    tweetsDB = QSqlDatabase::addDatabase("QSQLITE");
    tweetsDB.setDatabaseName("data.db3");
}

