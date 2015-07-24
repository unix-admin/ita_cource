#include "usersearch.h"
#include "ui_usersearch.h"

UserSearch::UserSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSearch)
{
    ui->setupUi(this);
}

UserSearch::~UserSearch()
{
    delete ui;
}

void UserSearch::userSearchButtonClick()
{

}
