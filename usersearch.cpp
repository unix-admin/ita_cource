#include "usersearch.h"
#include "ui_usersearch.h"
#include <QImageReader>
#include <QEventLoop>
#include <QPixmap>
#include <userdetails.h>
UserSearch::UserSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSearch)
{
    page=1;
    ui->setupUi(this);
    connect(ui->toSearch,SIGNAL(clicked(bool)),this,SLOT(userSearchButtonClick()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(previous()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(next()));
    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(userSearchButtonClick()));
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->scrollArea->setVisible(false);
    resized = false;
    this->resize(this->geometry().width(),this->geometry().height()-500);
}

void UserSearch::getTwitterClass()
{
    twitter = Twitter::getcls();
    userSearchResultsByPage = twitter->getUserSettings()->searchUsersByPage.toInt();
    QVBoxLayout *searchArea = new QVBoxLayout(ui->scrollArea);
    for (int i=0; i< userSearchResultsByPage; i++)
    {
        QTextEdit *textEdit =new QTextEdit();        
        textEdit->setTextColor(QColor("black"));
        textEdit->setObjectName("textEdit"+QString::number(i));
        textEdit->setReadOnly(true);        
        textEdit->setMinimumSize(30,90);
        textEdit->viewport()->setMouseTracking(true);
        textEdit->viewport()->setCursor(Qt::PointingHandCursor);
        textEdit->viewport()->setFocusPolicy(Qt::ClickFocus);
        textEdit->viewport()->installEventFilter(this);
        myListBox.append(textEdit);
        searchArea->setSizeConstraint(QLayout::SetMinAndMaxSize);
        searchArea->addWidget(textEdit);        
    }
    ui->scrollAreaWidgetContents->setLayout(searchArea);

}

UserSearch::~UserSearch()
{
    delete ui;
}

void UserSearch::userSearchButtonClick()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished()));
    QNetworkRequest request;
    request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/users/search.json","q="+QUrl::toPercentEncoding(ui->lineEdit->text()).toStdString()+"&count="+twitter->getUserSettings()->searchUsersByPage.toStdString()+"&page=1"));
    QNetworkReply* reply= manager->get(request);
    connect(reply, SIGNAL(finished()),this, SLOT(userSearchFinished()));
}

void UserSearch::navigateButtonClick()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished()));
    QNetworkRequest request;
    request.setUrl(twitter->generateQueryString("https://api.twitter.com/1.1/users/search.json","q="+QUrl::toPercentEncoding(ui->lineEdit->text()).toStdString()+"&count="+twitter->getUserSettings()->searchUsersByPage.toStdString()+"&page="+QString::number(page).toStdString()));
    QNetworkReply* reply= manager->get(request);
    connect(reply, SIGNAL(finished()),this, SLOT(userSearchFinished()));
}

void UserSearch::userSearchFinished()
{
    QMap<QString,QVariant> searchedUsers;
    QList<QVariant> results;
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray qb = "{\"fuckingJsonFromTwitter\": ";
        qb.append(reply->readAll());
        qb.append("}");
        searchedUsers = QJsonDocument::fromJson(qb).toVariant().toMap();
        results.append(searchedUsers.begin().value().toList());        
        parseResults(results);
      }
    else qDebug()<<reply->errorString();

    reply->deleteLater();
}

void UserSearch::next()
{
    page++;
    navigateButtonClick();
}

void UserSearch::previous()
{
    if(page > 1)
    {
        page--;
        navigateButtonClick();

    }
}

void UserSearch::searchFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QImage img2;
    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());
    pixmap.save("img.jpg");
    reply->deleteLater();
}

void UserSearch::replyFinished()
{
    QNetworkAccessManager *manager = qobject_cast<QNetworkAccessManager *>(sender());
    manager->deleteLater();
}

void UserSearch::parseResults(QList<QVariant> results)
{
    int searchedResults = results.count();
    setVisibleArea(searchedResults);

    if (!results.empty())
    {
         QMap<QString, QVariant> usermap;
         QString textBoxText;
         QMap<QString, QVariant>::const_iterator userName;
         QMap<QString, QVariant>::const_iterator screenName;
         QMap<QString, QVariant>::const_iterator description;
         QMap<QString, QVariant>::const_iterator id;
         QMap<QString, QVariant>::const_iterator image;

         QNetworkAccessManager *manager = new QNetworkAccessManager;
         QNetworkRequest request;
         QEventLoop event;
         for(int i=0; i<searchedResults;i++)
         {
             usermap = results.at(i).toMap();
             userName = usermap.find("name");
             screenName = usermap.find("screen_name");
             description = usermap.find("description");
             id = usermap.find("id_str");
             image = usermap.find("profile_image_url");
             request.setUrl(QUrl(image.value().toString()));
             manager->get(request);
             QNetworkReply* reply= manager->get(request);
             connect( reply, SIGNAL(finished()),this, SLOT(searchFinished()));
             connect( reply, SIGNAL(finished()),&event, SLOT(quit()));
             event.exec();
             textBoxText.append(" <table width=\"300\" border=\"0\"\\><tr><td width=\"70\" height=\"59\"><img src=\"img.jpg\"  ></td>");
             textBoxText.append(" <td width=\"220\">"+userName.value().toString()+"<br>"+screenName.value().toString()+"</td></tr>");
             textBoxText.append("<tr><td colspan=\"2\">"+description.value().toString()+" </tr></table>");
             myListBox.at(i)->setTextColor(QColor("black"));
             myListBox.at(i)->setHtml(textBoxText);
             myListBox.at(i)->setDocumentTitle(id.value().toString());
             myListBox.at(i)->setVisible(true);
             textBoxText.clear();
        }

    }
    else
    {

        myListBox.at(0)->setText("Ничего не найдено!");
        myListBox.at(0)->setDocumentTitle("-1");

    }
}

bool UserSearch::eventFilter(QObject *target, QEvent *event)
{
    if (target->objectName() == "qt_scrollarea_viewport")
    {
     if (event->type() == QEvent::MouseButtonDblClick)
      {
         for (int i = 0; i<userSearchResultsByPage; i++)
         {
           if (target->parent() == myListBox.value(i))
          {
            if(myListBox.value(0)->documentTitle() !="-1")
             {
                 UserDetails *detailsForm = new UserDetails;
                 connect(detailsForm,SIGNAL(formClosed()),detailsForm,SLOT(deleteLater()));
                 detailsForm->prepareData(myListBox.value(i)->documentTitle());
                 break;
             }
           }
         }
      }
    }
    return QWidget::eventFilter(target, event);
}

void UserSearch::setVisibleArea(int searchedResults)
{
    if (searchedResults >= 5 )
    {

        setDefaultWindow();
    }
    else
    {
        if (searchedResults==0)
        {
            searchedResults=1;
        }
        this->resize(400,590+((searchedResults-5)*100));

        ui->scrollArea->resize(380,510+((searchedResults-5)*100));
        for (int i=userSearchResultsByPage-1; i>=searchedResults; i--)
        {
           myListBox.at(i)->setVisible(false);
        }

        ui->pushButton_2->setVisible(false);
        ui->pushButton_3->setVisible(false);
        ui->scrollArea->setVisible(true);
    }
}

void UserSearch::setDefaultWindow()
{
    // this->resize(400,590);
    if (!resized)
    {
        this->setGeometry(this->x(),this->y()-200,400,590);
        resized = true;
    }
    ui->scrollArea->resize(380,500);
    ui->pushButton_2->move(20,560);
    ui->pushButton_3->move(250,560);
    ui->pushButton_2->setVisible(true);
    ui->pushButton_3->setVisible(true);
    ui->scrollArea->setVisible(true);


}
