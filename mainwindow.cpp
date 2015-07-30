#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebFrame>
#include <QFile>
#include <QStringList>
#include <QTimer>
#include <QThread>

#include "setpin.h"
#include "autorize.h"
#include "usersearch.h"
#include "tweetssearch.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{    
    ui->setupUi(this);    
    ui->centralWidget->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->textEdit->setReadOnly(true);
    ui->textEdit->setVisible(false);
    tw = new Twitter(ui->textEdit);
    QPixmap myImg(":/data/splash.jpg");    
    ui->label_2->setPixmap(myImg);
    QTimer *timer = new QTimer;
    timer->start(20000);
    connect(timer,SIGNAL(timeout()), this, SLOT(networkConnection()));
    ui->statusBar->insertPermanentWidget(0, ui->netImg ,0 );
    ui->statusBar->insertPermanentWidget(0, ui->netText ,0 );
    ui->pushButton->setEnabled(false);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));    
    connect(ui->textEdit,SIGNAL(textChanged()),this, SLOT(splashInvisible()));
    connect(ui->searchButton,SIGNAL(clicked()),SLOT(userSearch()));

    connect(ui->centralWidget,SIGNAL(destroyed(QObject*)),this,SLOT(close()));
    connect(ui->tweetSearchButton,SIGNAL(clicked()),SLOT(tweetSearch()));
    networkConnection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showPinWindow()
{


}

void MainWindow::buttonClicked()
{
    tw->getUserTimeline();


}



void MainWindow::testbutton()
{

}

void MainWindow::changeUrl()
{


}

void MainWindow::networkConnection()
{
    netError = false;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    QNetworkRequest request;
    request.setUrl(QUrl("https://twitter.com/login"));
    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");
    QNetworkReply *reply = manager->get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),SLOT(networkError()));
    connect(reply,SIGNAL(finished()),this,SLOT(networkOk()));


}

void MainWindow::networkError()
{
      netError = true;
}

void MainWindow::networkOk()
{

    if (netError)
    {
        ui->netText->setText(QString::fromStdString("Сеть недоступна"));
        ui->netImg->setPixmap(QPixmap(":/data/images/disconnected.png"));
        ui->pushButton->setEnabled(false);
    }
    else
    {
        ui->netText->setText(QString::fromStdString("Сеть доступна"));
        ui->netImg->setPixmap(QPixmap(":/data/images/connected.png"));
        ui->pushButton->setEnabled(true);
    }

}

void MainWindow::replyFinished(QNetworkReply *)
{

}

void MainWindow::updateUserTimeLine()
{
    ui->textEdit->setText(tw->userTimeLineText());
}

void MainWindow::splashInvisible()
{
    ui->label_2->setVisible(false);
}

void MainWindow::userSearch()
{

   UserSearch *userSearchForm = new UserSearch();
   userSearchForm->getTwitterClass(tw);
   userSearchForm->show();
}

void MainWindow::close()
{
    QApplication::quit();
}

void MainWindow::tweetSearch()
{
    TweetsSearch *searchTweet = new TweetsSearch;
    searchTweet->prepare(tw);

}

void MainWindow::closeEvent(QCloseEvent *event)
{
 QApplication::exit();
}
