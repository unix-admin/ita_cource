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


QStringList DataBase::getUsers()
{
     connect();
     QStringList result;
     QSqlQuery getUsersQuery("SELECT username FROM users");
     while (getUsersQuery.next()) {
         result.append(getUsersQuery.value(0).toString());
     }
     result.append("Новый пользователь");
     disconnect();

     return result;
}

QStringList DataBase::getSettings(int userID)
{
    connect();
    QStringList result;
    QSqlQuery getUsersQuery("SELECT * FROM settings WHERE userID="+QString::number(userID));
    while (getUsersQuery.next()) {
        result.append(getUsersQuery.value(1).toString());
        result.append(getUsersQuery.value(2).toString());
        result.append(getUsersQuery.value(3).toString());
        result.append(getUsersQuery.value(4).toString());
        result.append(getUsersQuery.value(5).toString());
    }
    disconnect();
    return result;
}

QStringList DataBase::getUserParameters(QString userName)
{
    connect();
    QSqlQuery getUserParametersQuery("SELECT * FROM users WHERE username='"+userName+"'");
    QStringList result;
    while (getUserParametersQuery.next()) {
        result.append(getUserParametersQuery.value(0).toString());
        result.append(getUserParametersQuery.value(1).toString());
        result.append(getUserParametersQuery.value(2).toString());
        result.append(getUserParametersQuery.value(3).toString());
        result.append(getUserParametersQuery.value(4).toString());
        result.append(getUserParametersQuery.value(5).toString());
        result.append(getUserParametersQuery.value(6).toString());
        result.append(getUserParametersQuery.value(7).toString());
        result.append(getUserParametersQuery.value(8).toString());
    }
    disconnect();
    result.append(getSettings(result.value(0).toInt()));
    return result;
}

void DataBase::setSettings(int userID, QStringList settings)
{
    connect();
    QSqlQuery setSettingsQuery;
    setSettingsQuery.exec("UPDATE settings SET timelineTweetsByPage="+settings.value(0)\
                          +", searchTweetsByPage="+settings.value(1)\
                          +", searchUsersByPage="+settings.value(2) \
                          +", searchTweetsToDatabase="+settings.value(3) \
                          + ", refreshTime="+settings.value(4)+" WHERE userID="+QString::number(userID));

    disconnect();
    emit workFinished();
}

DataBase::DataBase()
{
    tweetsDB = QSqlDatabase::addDatabase("QSQLITE");
    tweetsDB.setDatabaseName("data.db3");
}

