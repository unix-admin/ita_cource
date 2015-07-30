#ifndef USERDETAILS_H
#define USERDETAILS_H

#include <QWidget>
#include <twitter.h>
#include <QMap>
#include <QJsonDocument>
#include <QVariant>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QPixmap>

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
private:
    void getUserinfo(std::string id);
    void getUserTimeline(std::string id);
    void parseUserinfoResults(QMap<QString,QVariant> results);
    void parseUserTimelineResults(QList<QVariant> results);
    void setDefaultWindow();
private slots:
    void getUserInfoFinished();
    void getUserTimelineFinished();
    void imageDownloadFinished();
signals:
    void getInfoFinished();
};

#endif // USERDETAILS_H
