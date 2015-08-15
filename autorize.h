#ifndef AUTORIZE_H
#define AUTORIZE_H

#include <QWidget>
#include "twitter.h"
#include <QUrl>
#include <QWebFrame>
#include <QUrlQuery>
#include <QMessageBox>
#include "setpin.h"
#include "database.h"
#include "database.h"

namespace Ui {
class autorize;
}

class autorize : public QWidget
{
    Q_OBJECT

public:
    explicit autorize(QWidget *parent = 0);

    ~autorize();
    void getAutorisation();
private:
    void urlChange(QUrl url);
    void showPinWindow();


private:
    Ui::autorize *ui;
    Twitter *clsTwitter;
    DataBase *db;
private slots:
    void changeUrl();

signals:
    formClosed();
};

#endif // AUTORIZE_H
