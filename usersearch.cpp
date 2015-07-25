#include "usersearch.h"
#include "ui_usersearch.h"

UserSearch::UserSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSearch)
{
    ui->setupUi(this);
}

UserSearch::UserSearch(Twitter *clsTwitter)
{
    twitter = clsTwitter;
}

UserSearch::~UserSearch()
{
    delete ui;
}

void UserSearch::userSearchButtonClick()
{
    twitter->userSerch(ui->textEdit->toHtml());

    connect(this,SIGNAL(twitter->finished()),this,SLOT(fin()));
}

void UserSearch::fin()
{
    qDebug() << "Test";
}
