#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QLabel>
#include <QWebFrame>
#include <QFile>
#include <QStringList>
#include <QTimer>
#include <QThread>
#include <QMainWindow>

#include "twitter.h"
#include "setpin.h"
#include "autorize.h"
#include "usersearch.h"
#include "tweetssearch.h"
#include "database.h"
#include "settings.h"
#include "parser.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    Twitter *tw;
    bool netError;
    DataBase *db;
    DataBase::userData data;
    int leftLimit;
    int rightLimit;
    int pages;
private slots:
    void buttonClicked();

    void networkConnection();
    void networkError();
    void networkOk();
    void replyFinished(QNetworkReply*);
    void updateUserTimeLine();
    void splashInvisible();
    void userSearch();
    void close();
    void tweetSearch();
    void settingsShow();
    void userShow();
    void getNewUserData();
    void moved();
    void myUserClicked();

private:
    void closeEvent(QCloseEvent*) Q_DECL_OVERRIDE;
    QString getVirtualTimeLine(int left, int right);

};

#endif // MAINWINDOW_H
