#include "setpin.h"
#include "ui_setpin.h"

setPIN::setPIN(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setPIN)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(buttonOKClicked()));
    connect(ui->buttonBox,SIGNAL(rejected()),this, SLOT(close()));
}

setPIN::~setPIN()
{
    delete ui;
}

setPIN::getTwitter(Twitter *clsTwitter)
{
    twitter = clsTwitter;

}

void setPIN::buttonOKClicked()
{
 twitter->setPin(ui->lineEdit->text().toStdString());

 close();
}
