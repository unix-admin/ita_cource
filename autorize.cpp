#include "autorize.h"
#include "ui_autorize.h"

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
    show();
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
    ui->webView->setVisible(false);
    QUrlQuery response(ui->webView->page()->currentFrame()->toPlainText());
    response.setQueryDelimiters('=','&');
    QString displayName = response.queryItemValue("screen_name");
    QString accessToken = response.queryItemValue("oauth_token");
    QString accessTokenSecret = response.queryItemValue("oauth_token_secret");
    DataBase *db = DataBase::getInstance();
    if (db->checkUser(displayName))
    {
        QMessageBox message;
        message.warning(this,"Ошибка","Пользователь существует!");
        message.show();
    }
    else
    {
        db->addNewUser(accessToken,accessTokenSecret,displayName);
    }
    delete pin;
    QWebSettings::clearMemoryCaches();
    this->deleteLater();
    close();
    }
    else
    {
        QWebSettings::clearMemoryCaches();
        delete pin;
        this->deleteLater();
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
