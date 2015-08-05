#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStringList>
class DataBase : public QObject
{
    Q_OBJECT
public:
    static DataBase * getInstance() {
            if(!p_instance)
                p_instance = new DataBase();
            return p_instance;
    }
    QStringList getUsers();
    QStringList getSettings(int userID);
    QStringList getUserParameters(QString userName);
    void setSettings(int userID, QStringList settings);


private:
    static DataBase * p_instance;
    DataBase();
    DataBase( const DataBase& );
    DataBase& operator=( DataBase& );
    void connect();
    QSqlDatabase tweetsDB;
    void disconnect();

signals:
    workFinished();
};


#endif // DATABASE_H
