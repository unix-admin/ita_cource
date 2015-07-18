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
    void button2Clicked();
    void testbutton();
    void changeUrl();
    void networkConnection();
    void networkError();
    void networkOk();
    void replyFinished(QNetworkReply*);

private:


};

#endif // MAINWINDOW_H
