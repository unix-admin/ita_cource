#include "setpin.h"
#include "ui_setpin.h"

setPIN::setPIN(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setPIN)
{
    checkPin =0;
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(buttonOKClicked()));
    connect(ui->buttonBox,SIGNAL(rejected()),this, SLOT(close()));
}

setPIN::~setPIN()
{
    delete ui;
}

void setPIN::getTwitter(Twitter *clsTwitter)
{
    twitter = clsTwitter;

}

void setPIN::buttonOKClicked()
{
 twitter->setPin(ui->lineEdit->text().toStdString());
 checkPin = 1;
 close();
}
