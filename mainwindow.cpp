#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebFrame>
#include <QFile>
#include <QStringList>
#include <QTimer>
#include <QThread>

#include "setpin.h"
#include "autorize.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    tw = new Twitter;
    QPixmap myImg(":/data/splash.jpg");
    ui->label_2->setPixmap(myImg);
    QTimer *timer = new QTimer;
    timer->start(20000);
    connect(timer,SIGNAL(timeout()), this, SLOT(networkConnection()));
    ui->statusBar->insertPermanentWidget(0, ui->netImg ,0 );
    ui->statusBar->insertPermanentWidget(0, ui->netText ,0 );
    ui->pushButton->setEnabled(false);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));    
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
//    autorize *setAutorisation = new autorize();
//    setAutorisation->show();
//    setAutorisation->getAutorisation(tw);
     QThread thread;
     //tw->moveToThread(thread);
    tw->getUserTimeline();

}

void MainWindow::button2Clicked()
{


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
