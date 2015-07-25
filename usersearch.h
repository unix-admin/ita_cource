#ifndef USERSEARCH_H
#define USERSEARCH_H

#include <QWidget>
#include "twitter.h"
namespace Ui {
class UserSearch;
}

class UserSearch : public QWidget
{
    Q_OBJECT

public:
    UserSearch(QWidget *parent = 0);
    UserSearch(Twitter *clsTwitter);
    ~UserSearch();

private:
    Ui::UserSearch *ui;
    Twitter *twitter;
private slots:
    void userSearchButtonClick();
    void fin();
};

#endif // USERSEARCH_H
