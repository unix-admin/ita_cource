#ifndef USERSEARCH_H
#define USERSEARCH_H

#include <QWidget>
#include "twitter.h"
#include <QTextEdit>
#include <QList>
#include <QLayout>
namespace Ui {
class UserSearch;
}

class UserSearch : public QWidget
{
    Q_OBJECT

public:
    UserSearch(QWidget *parent = 0);
    void getTwitterClass(Twitter *clsTwitter);

    ~UserSearch();

private:
    Ui::UserSearch *ui;
    Twitter *twitter;
    QList<QTextEdit*> myListBox;
    int userSearchResultsByPage;
private slots:
    void userSearchButtonClick();
    void userSearchFinished();
private:
    void prepareUserSearchResults();
    void userSearch(QString username);
};

#endif // USERSEARCH_H
