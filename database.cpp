#include "database.h"

DataBase::DataBase()
{
    tweetsDB = QSqlDatabase::addDatabase("QSQLITE");
    tweetsDB.setDatabaseName("data.db3");
}

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

