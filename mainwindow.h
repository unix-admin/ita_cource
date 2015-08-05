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
    void urlChange(QUrl url);
    void showPinWindow();
    bool netError;
    DataBase *db;
private slots:
    void buttonClicked();
    void testbutton();
    void changeUrl();
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

private:
void closeEvent(QCloseEvent*) Q_DECL_OVERRIDE;

};

#endif // MAINWINDOW_H
