#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStringList>
class DataBase
{
public:
    DataBase();
public:

    QStringList getUsers();
private:
    void connect();
    QSqlDatabase tweetsDB;
    void disconnect();

};

#endif // DATABASE_H
