#ifndef READABLEUSERS_H
#define READABLEUSERS_H

#include <QWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QGridLayout>
#include <QIcon>

#include "twitter.h"
#include "database.h"
#include "userdetails.h"
namespace Ui {
class ReadableUsers;
}

class ReadableUsers : public QWidget
{
    Q_OBJECT

public:
    explicit ReadableUsers(QWidget *parent = 0);
    ~ReadableUsers();

private:
    Ui::ReadableUsers *ui;
    QList<QLayout*> users;
    QIcon buttonIcon;
    QPixmap userPhoto;
    DataBase *database;
    Twitter *clsTwitter;
    int readableUsersCount;
    bool eventFilter(QObject *target, QEvent *event);
    QGridLayout *layout;
private slots:
    void deleteUser();

};

#endif // READABLEUSERS_H
