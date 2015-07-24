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
    explicit UserSearch(QWidget *parent = 0);
    ~UserSearch();

private:
    Ui::UserSearch *ui;
private slots:
    void userSearchButtonClick();
};

#endif // USERSEARCH_H
