#include "autorize.h"
#include "ui_autorize.h"
#include "setpin.h"

autorize::autorize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autorize)
{
    ui->setupUi(this);
    connect(ui->webView,SIGNAL(loadFinished(bool)), this, SLOT(changeUrl()));
}

autorize::~autorize()
{
    delete ui;
}

void autorize::getAutorisation(Twitter *pTwitter)
{
    clsTwitter = pTwitter;
    QUrl url(clsTwitter->getRequestToken());
    urlChange(url);
    qDebug() << ui->webView->url().toString();
    url = "about:blank";
    clsTwitter->setOauthToken(ui->webView->page()->currentFrame()->toPlainText().toStdString());
    url = clsTwitter->getPIN();
    ui->webView->setUrl(url);
}

void autorize::urlChange(QUrl url)
{
    QEventLoop loop;
    ui->webView->setUrl(url);
    connect(ui->webView,SIGNAL(urlChanged(QUrl)), &loop, SLOT(quit()));
    loop.exec();
    std::string urlstr = url.toString().toStdString();
}

void autorize::showPinWindow()
{
    setPIN *pin = new setPIN;
    pin->getTwitter(clsTwitter);
    pin->exec();
    if (pin->checkPin == 1)
    {
    QEventLoop loop;
    ui->webView->setUrl(clsTwitter->accessToken());
    connect(ui->webView,SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
    loop.exec();
    clsTwitter->setAccessToken(ui->webView->page()->currentFrame()->toPlainText().toStdString());
    }
    else
    {
        close();
    }
}

void autorize::changeUrl()
{

    if (this->ui->webView->url().toString().toStdString()=="https://api.twitter.com/oauth/authorize")
    {

         showPinWindow();

    }
}
