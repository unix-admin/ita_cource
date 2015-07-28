#include "usersearch.h"
#include "ui_usersearch.h"

UserSearch::UserSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSearch)
{
    ui->setupUi(this);
}

void UserSearch::getTwitterClass(Twitter *clsTwitter)
{
    twitter = clsTwitter;
    userSearchResultsByPage = clsTwitter->getuserSearchResultByPage();
    QVBoxLayout *searchArea = new QVBoxLayout(ui->scrollArea);
    for (int i=0; i< userSearchResultsByPage; i++)
    {
        QTextEdit *textEdit =new QTextEdit();
        textEdit->setTextColor(QColor("black"));
        textEdit->setObjectName("textEdit"+QString::number(i));
        textEdit->setReadOnly(true);
        textEdit->setEnabled(false);
        textEdit->setMinimumSize(30,90);
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



}

void UserSearch::userSearchFinished()
{
    qDebug() << "Test";
}
