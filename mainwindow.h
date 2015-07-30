#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <twitter.h>
#include <QLabel>
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

private:
void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

};

#endif // MAINWINDOW_H
