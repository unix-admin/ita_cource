#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebFrame>
#include <QFile>
#include <QStringList>
#include <QTimer>

#include "setpin.h"
#include "autorize.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    tw = new Twitter;
    ui->webView->setHidden(true);
    QPixmap myImg(":/data/splash.jpg");
    ui->label_2->setPixmap(myImg);
    QTimer *timer = new QTimer;
    timer->start(20000);
    connect(timer,SIGNAL(timeout()), this, SLOT(networkConnection()));


    ui->statusBar->insertPermanentWidget(0, ui->netImg ,0 );
    ui->statusBar->insertPermanentWidget(0, ui->netText ,0 );
    ui->pushButton_2->setVisible(false);
    ui->pushButton->setEnabled(false);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(button2Clicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(testbutton()));
    connect(ui->webView,SIGNAL(loadFinished(bool)), this, SLOT(changeUrl()));

    networkConnection();





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::urlChange(QUrl url)
{

//    QEventLoop loop;
//    ui->webView->setUrl(url);
//    connect(ui->webView,SIGNAL(urlChanged(QUrl)), &loop, SLOT(quit()));
//    loop.exec();
//    std::string urlstr = url.toString().toStdString();


}

void MainWindow::showPinWindow()
{
    setPIN *pin = new setPIN;
    pin->getTwitter(tw);
    pin->exec();
    QEventLoop loop;
    ui->webView->setUrl(tw->accessToken());
    connect(ui->webView,SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
    loop.exec();
    tw->setAccesshToken(ui->webView->page()->currentFrame()->toPlainText().toStdString());

}

void MainWindow::buttonClicked(){

//    QUrl url(tw->getRequestToken());
//    urlChange(url);
//    qDebug() << ui->webView->url().toString();
//    url = "about:blank";
//    tw->setOauthToken(ui->webView->page()->currentFrame()->toPlainText().toStdString());
//    url = tw->getPIN();
//    ui->webView->setUrl(url);
    autorize *setAutorisation = new autorize();
    setAutorisation->show();
    setAutorisation->getAutorisation(tw);


}

void MainWindow::button2Clicked()
{
    setPIN *pin = new setPIN;
    pin->getTwitter(tw);
    pin->exec();
    QEventLoop loop;
    ui->webView->setUrl(tw->accessToken());
    connect(ui->webView,SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
    loop.exec();
    ui->pushButton_2->setVisible(false);
    tw->setAccesshToken(ui->webView->page()->currentFrame()->toPlainText().toStdString());


}

void MainWindow::testbutton()
{
    qDebug() << ui->webView->url().toString();
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

        qDebug() << "Net Error";
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
