#include "database.h"
#include <QFile>
void DataBase::connect()
{
    if (!twitterDB.open()) {
          twitterDB.open();
    }
}

void DataBase::disconnect()
{
    if (twitterDB.open()) {
          twitterDB.close();
    }
}

void DataBase::process()
{

}

bool DataBase::checkUser(QString parameter, userparameters queryType)
{
    bool result = false;
    connect();
    QString queryString;
    switch (queryType) {
    case BY_ID:
    {
        queryString = "SELECT userID FROM users WHERE userID=\""+parameter+"\"";
        break;
    }
    case BY_DISPLAY_NAME:
    {
        queryString = "SELECT userID FROM users WHERE displayName=\""+parameter+"\"";
        break;
    }
    case BY_TWITTER_ID:
    {
        queryString = "SELECT userID FROM users WHERE twitterID="+parameter;
        break;
    }
    case BY_NAME:
    {
        queryString = "SELECT userID FROM users WHERE userName=\""+parameter+"\"";
        break;
    }
    }
    QSqlQuery checkUserQuery;
    checkUserQuery.prepare(queryString);
    checkUserQuery.exec();
    checkUserQuery.first();
    if (checkUserQuery.value(0).toString() =="")
        result = false;
    else
        result = true;
    disconnect();
    return result;
}

bool DataBase::checkReadableUser(QString userID, QString twitterID)
{
    bool result = false;
    connect();
    QString queryString = "SELECT readableUsers.twitterID FROM readableUsers, users WHERE readableUsers.twitterID=users.twitterID AND readableUsers.userID="+userID+" AND users.twitterID="+twitterID;
    QSqlQuery checkReadableUserQuery;
    checkReadableUserQuery.prepare(queryString);
    checkReadableUserQuery.exec();
    checkReadableUserQuery.first();
    if (checkReadableUserQuery.value(0).toString() =="")
        result = false;
    else
        result = true;
    disconnect();
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

void DataBase::updateNewUserData(QString id, Twitter::userData *data )
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
                  "tweetsToDatabase, refreshTime) VALUES ("+id+",5,10,5,100,1)";
    updateUserDataQuery.prepare(queryString);
    updateUserDataQuery.exec();
    disconnect();
    emit workFinished();
}

void DataBase::addReadableUser(Twitter::userData *data, QString senderID, queryTypes typeQuery)
{
    connect();
    QSqlQuery updateUserDataQuery;
    QString queryString;
    if (typeQuery == NEW_USER)
    {
        queryString = "INSERT INTO users(userName,displayName,twitterID,desription,tweets,friends,followers,image,imageUrl,usertype) VALUES(\""+data->name\
                +"\",\""+data->screen_name\
                +"\","+data->twitterID\
                +", \""+data->description \
                +"\", "+data->statuses_count \
                +", "+data->friends_count \
                +", "+data->followers_count \
                +", :image"
                +", \""+data->profile_image_url +"\",0)";

        updateUserDataQuery.prepare(queryString);
        updateUserDataQuery.addBindValue(data->profile_image_data);
        updateUserDataQuery.exec();
        updateUserDataQuery.finish();
    }
    queryString = "INSERT INTO readableUsers (userID,twitterID) VALUES("+senderID+","+data->twitterID+")" ;
    updateUserDataQuery.prepare(queryString);
    updateUserDataQuery.exec();
    disconnect();
}

void DataBase::insertTweetsToDatabase(QList<tweets> *dataToInsert)
{
    connect();
    QSqlQuery insertTweetsToDatabaseQuery;
    QString queryString = "";
    insertTweetsToDatabaseQuery.prepare("INSERT INTO tweets (tweetID, tweetTime, username, text, userID) "\
                                        " VALUES(:tweetID,:tweetTime,:username,:text,:twitterUserID)" );

    QVariantList tweetID,tweetTime,username,text,twitterUserID;
    for (int i=0; i< dataToInsert->count();i++)
    {
        tweetID.append(dataToInsert->at(i).tweetID);
        tweetTime.append(dataToInsert->at(i).tweetTime);
        username.append(dataToInsert->at(i).username);
        text.append(dataToInsert->at(i).text);
        twitterUserID.append(dataToInsert->at(i).twitterUserID);        
    }

    insertTweetsToDatabaseQuery.addBindValue(tweetID);
    insertTweetsToDatabaseQuery.addBindValue(tweetTime);
    insertTweetsToDatabaseQuery.addBindValue(username);
    insertTweetsToDatabaseQuery.addBindValue(text);
    insertTweetsToDatabaseQuery.addBindValue(twitterUserID);    
    if(!insertTweetsToDatabaseQuery.execBatch())
        qDebug() << insertTweetsToDatabaseQuery.lastError();
    twitterDB.commit();
    disconnect();
}


QList<Twitter::tweetsData> DataBase::getTimeline(QString maxTweetID, QString userID, int leftLimit, int rightLimit,queryTypes type)
{
    QList<Twitter::tweetsData> result;
    Twitter::tweetsData queryResult;
    QString leftLimitData;
    QString appendCondition;
    if (maxTweetID != "")
        appendCondition = " AND tweets.tweetID >"+maxTweetID;
    QString rightLimitData = QString::number(rightLimit);
    connect();
    QSqlQuery getTimelineQuery;
    if (leftLimit==0)
    {
        leftLimitData="";
    }
    else
    {
        leftLimitData=QString::number(leftLimit)+",";
    }
    QString queryString;
    switch (type) {
    case BY_ID:
        break;
    case NEW_USER:
        break;
    case VIRTUAL_TIMELINE:
    {   queryString = "SELECT tweets.tweetTime, tweets.username, tweets.text FROM tweets, users, readableUsers " \
                      "WHERE tweets.userID=readableUsers.twitterID AND readableUsers.userID = users.userID AND users.userID="+userID +appendCondition \
                      +" ORDER BY tweets.tweetID DESC LIMIT "+leftLimitData+rightLimitData;
        break;
    }
    case USER_TIMELINE:
    {
        queryString = "SELECT tweets.tweetTime, tweets.username, tweets.text FROM tweets WHERE tweets.userID="+userID+ appendCondition \
                      +" ORDER BY tweets.tweetID DESC LIMIT "+leftLimitData+rightLimitData;
        break;
    }
    }
    getTimelineQuery.prepare(queryString);
    getTimelineQuery.exec();
    while (getTimelineQuery.next())
    {
        queryResult.tweetTime = getTimelineQuery.value(0).toString();
        queryResult.username = getTimelineQuery.value(1).toString();
        queryResult.text = getTimelineQuery.value(2).toString();
        result.append(queryResult);
    }
    return result;
}

void DataBase::deleteUser(QString twitterID, QString readerID)
{

    connect();
    QString queryString = "SELECT count() FROM readableUsers WHERE twitterID="+twitterID;
    QSqlQuery deleteUserQuery;
    deleteUserQuery.prepare(queryString);
    deleteUserQuery.exec();
    deleteUserQuery.first();
    int count = deleteUserQuery.value(0).toInt();
    if (count == 1)
    {
        deleteUserQuery.finish();
        queryString = "DELETE FROM tweets WHERE userID="+twitterID;
        deleteUserQuery.prepare(queryString);
        deleteUserQuery.exec();
        deleteUserQuery.finish();
        queryString = "DELETE FROM users WHERE usertype=0 AND twitterID="+twitterID;
        deleteUserQuery.prepare(queryString);
        deleteUserQuery.exec();
    }
    deleteUserQuery.finish();
    queryString = "DELETE FROM readableUsers WHERE twitterID="+twitterID + " AND userID="+readerID;
    deleteUserQuery.prepare(queryString);
    deleteUserQuery.exec();
    deleteUserQuery.finish();
    disconnect();
}

int DataBase::countRecordsInTimeLine(QString userID, QString maxTweetID, queryTypes type)
{
    int result;
    connect();
    QString appendCondition;
    QString queryString;
    if (maxTweetID != "")
        appendCondition = " AND tweets.tweetID >"+maxTweetID;
    if (type==VIRTUAL_TIMELINE)
        queryString = "SELECT count() FROM tweets, users, readableUsers " \
                          "WHERE tweets.userID=readableUsers.twitterID AND readableUsers.userID = users.userID AND users.userID="+userID+ appendCondition;
    else
        queryString = "SELECT count() FROM tweets WHERE userID=\""+userID+"\"" + appendCondition;
    QSqlQuery countRecordsInTimeLineQuery;
    countRecordsInTimeLineQuery.prepare(queryString);
    countRecordsInTimeLineQuery.exec();
    countRecordsInTimeLineQuery.first();
    if (countRecordsInTimeLineQuery.value(0).toString() =="")
        result = 0;
    else
        result = countRecordsInTimeLineQuery.value(0).toInt();
    disconnect();
    return result;

}

int DataBase::countReadableUsers(QString userID)
{
  int result;
  connect();
  QString queryString = "SELECT count() FROM readableUsers WHERE userID="+userID;
  QSqlQuery countReadableUsersQuery;
  countReadableUsersQuery.prepare(queryString);
  countReadableUsersQuery.exec();
  countReadableUsersQuery.first();
  if (countReadableUsersQuery.value(0).toString() =="")
      result = 0;
  else
      result = countReadableUsersQuery.value(0).toInt();
  disconnect();
  return result;
}

void DataBase::updateUserData(QString twitterID, QString parameter, QVariant value)
{
    connect();
    QSqlQuery updateUserDataQuery;
    if (parameter !="image")
    {
        QString queryString = "UPDATE users SET " + parameter+"=\""+value.toString()+"\" WHERE twitterID="+twitterID;
        updateUserDataQuery.prepare(queryString);
    }
    else
    {
        QString queryString = "UPDATE users SET image=:image WHERE twitterID="+twitterID;
        updateUserDataQuery.prepare(queryString);
        updateUserDataQuery.addBindValue(value.toByteArray());
    }
    updateUserDataQuery.exec();
    updateUserDataQuery.finish();
    disconnect();
}

QList<Twitter::userData> DataBase::getReadableUsers(QString userID)
{
    QList<Twitter::userData> result;
    Twitter::userData node;
    connect();
    QSqlQuery getReadableUsersQuery("SELECT users.userName, users.displayName, users.twitterID, users.image FROM users," \
                                    " readableUsers WHERE users.twitterID = readableUsers.twitterID AND readableUsers.userID="+userID);
    while (getReadableUsersQuery.next()) {
        node.name = getReadableUsersQuery.value(0).toString();
        node.screen_name = getReadableUsersQuery.value(1).toString();
        node.twitterID = getReadableUsersQuery.value(2).toString();
        node.profile_image_data = getReadableUsersQuery.value(3).toByteArray();
        result.append(node);
    }
    disconnect();
    return result;

}

QStringList DataBase::getUsersForSync(QString userID)
{
    connect();
    QStringList result;
    QSqlQuery getUsersForSyncQuery("SELECT twitterID FROM readableUsers WHERE userID="+userID);
    while (getUsersForSyncQuery.next()) {
        result.append(getUsersForSyncQuery.value(0).toString());
    }
    disconnect();
    return result;
}

QString DataBase::getLastTweetID(QString twitterID)
{
    connect();
    QString result;
    QSqlQuery getLastTweetIDQuery;
    QString getLastTweetID;
    if (twitterID == "")
        getLastTweetID = "SELECT tweetID FROM tweets ORDER BY tweetID DESC LIMIT 1";
    else
        getLastTweetID = ("SELECT tweetID FROM tweets WHERE userID="+twitterID+" ORDER BY tweetID DESC LIMIT 1");
    getLastTweetIDQuery.prepare(getLastTweetID);
    getLastTweetIDQuery.exec();
    getLastTweetIDQuery.first();
        result = getLastTweetIDQuery.value(0).toString();

    disconnect();
    return result;
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

Twitter::userSettings DataBase::getSettings(QString userID)
{
    connect();
    Twitter::userSettings result;
    QSqlQuery getUsersQuery("SELECT * FROM settings WHERE userID="+userID);
    while (getUsersQuery.next()) {
        result.timelineTweetsByPage = getUsersQuery.value(1).toString();
        result.searchTweetsByPage = getUsersQuery.value(2).toString();
        result.searchUsersByPage = getUsersQuery.value(3).toString();        
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
                          + ", refreshTime="+settings.value(3)+" WHERE userID="+userID);

    disconnect();
    emit workFinished();
}

Twitter::userData DataBase::getData(QString parameter , userparameters type)
{
    Twitter::userData result;
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
    case BY_TWITTER_ID:
    {
        queryString = "SELECT * FROM users WHERE twitterID='"+parameter+"'";
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
   twitter = Twitter::getcls();
   twitterDB = twitter->getDatabase();
   if (!twitter->getDatabaseStatus())
   {
       createDatabase();
   }
}

void DataBase::createDatabase()
{
    connect();
    QSqlQuery createQuery;     


    createQuery.exec("CREATE TABLE readableUsers (userID INTEGER, twitterID INTEGER)");
    createQuery.finish();
    twitterDB.commit();
    createQuery.exec("CREATE TABLE users (userID INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, userName VARCHAR (255), twitterID INTEGER, accessTokenKey VARCHAR (255), accessTokenSecret VARCHAR (255), displayName VARCHAR (150), desription VARCHAR (255), image BLOB, imageUrl VARCHAR (255), tweets INTEGER, friends INTEGER, followers INTEGER, usertype INT)");

    createQuery.finish();
    twitterDB.commit();
    createQuery.exec("CREATE TABLE settings (userID INTEGER, timelineTweetsByPage INTEGER, searchTweetsByPage INTEGER, searchUsersByPage INTEGER, searchTweetsToDatabase INTEGER, refreshTime INTEGER)");
    createQuery.finish();
    twitterDB.commit();

    createQuery.exec("CREATE TABLE tweets (tweetID INTEGER, tweetTime VARCHAR (100), username VARCHAR (255), text VARCHAR (255), userID INTEGER DEFAULT (0))");

    createQuery.finish();
    twitterDB.commit();
    disconnect();

}

