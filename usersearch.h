#ifndef USERSEARCH_H
#define USERSEARCH_H

#include <QWidget>
#include "twitter.h"
#include <QTextEdit>
#include <QList>
#include <QLayout>
#include <QEventLoop>
#include <QJsonDocument>
#include <QMap>
#include <QVariant>
#include <QImage>

namespace Ui {
class UserSearch;
}

class UserSearch : public QWidget
{
    Q_OBJECT

public:
    UserSearch(QWidget *parent = 0);
    void getTwitterClass();

    ~UserSearch();

private:
    Ui::UserSearch *ui;
    Twitter *twitter;
    QList<QTextEdit*> myListBox;
    int page;
    int userSearchResultsByPage;
    bool resized;
private slots:
    void userSearchButtonClick();
    void navigateButtonClick();
    void userSearchFinished();
    void next();
    void previous();
    void searchFinished();
    void replyFinished();

private:
    void prepareUserSearchResults();
    void userSearch(QString username);
    void parseResults(QList<QVariant> results);
    bool eventFilter(QObject *target, QEvent *event);
    void setVisibleArea(int searchedResults);
    void setDefaultWindow();
};

#endif // USERSEARCH_H
