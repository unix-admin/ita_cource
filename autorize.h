#ifndef AUTORIZE_H
#define AUTORIZE_H

#include <QWidget>
#include "twitter.h"
#include <QUrl>
#include <QWebFrame>
namespace Ui {
class autorize;
}

class autorize : public QWidget
{
    Q_OBJECT

public:
    explicit autorize(QWidget *parent = 0);

    ~autorize();
    void getAutorisation(Twitter *pTwitter);
private:
    void urlChange(QUrl url);
    void showPinWindow();


private:
    Ui::autorize *ui;
    Twitter *clsTwitter;
private slots:
    void changeUrl();
};

#endif // AUTORIZE_H
