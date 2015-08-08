#ifndef USERDETAILS_H
#define USERDETAILS_H

#include <QWidget>
#include <QMap>
#include <QJsonDocument>
#include <QVariant>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QPixmap>
#include "requests.h"
#include "parser.h"
#include "database.h"
#include "twitter.h"
namespace Ui {
class UserDetails;
}

class UserDetails : public QWidget
{
    Q_OBJECT

public:
    explicit UserDetails(QWidget *parent = 0);
    ~UserDetails();
    void prepareData(Twitter *clsTwitter, std::string userid);

private:
    Ui::UserDetails *ui;
    Twitter *twitter;
    std::string id;
    QPixmap photo;
    DataBase *db;
    DataBase::userData userData;
private:
    void getUserinfo(std::string id);    
signals:
    formClosed();
private slots:
    void toDatabase();
};

#endif // USERDETAILS_H
