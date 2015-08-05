#include "settings.h"
#include "ui_settings.h"
#include <QDebug>
Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    settingsDB = DataBase::getInstance();
    connect(ui->sliderUsertimeline, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
    connect(ui->sliderSearchedTweets, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
    connect(ui->sliderSearchedUsers, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
    connect(ui->sliderMaxTweets, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
    connect(ui->sliderTimeToSync, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(saveChanges()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));
    getSettings();
}

Settings::~Settings()
{

    delete ui;
}

void Settings::setUserID(int ID)
{
    userID = ID;
}

void Settings::getSettings()
{
    QStringList userSettings;
    userSettings = settingsDB->getSettings(1);
    ui->sliderUsertimeline->setValue(userSettings.value(0).toInt());
    ui->sliderSearchedTweets->setValue(userSettings.value(1).toInt());
    ui->sliderSearchedUsers->setValue(userSettings.value(2).toInt());
    ui->sliderMaxTweets->setValue(userSettings.value(3).toInt());
    ui->sliderTimeToSync->setValue(userSettings.value(4).toInt());
}

void Settings::changeValue()
{
    QSlider *slider = qobject_cast<QSlider *>(sender());
    if (slider->objectName()=="sliderUsertimeline") {
        ui->valueTweetsUsertimeline->setText(QString::number(ui->sliderUsertimeline->value()));
    }
    if (slider->objectName()=="sliderSearchedTweets") {
        ui->valueMaxSearchTweets->setText(QString::number(ui->sliderSearchedTweets->value()));
    }
    if (slider->objectName()=="sliderSearchedUsers") {
        ui->valueMaxUsers->setText(QString::number(ui->sliderSearchedUsers->value()));
    }
    if (slider->objectName()=="sliderMaxTweets") {
        ui->valueMaxTweetsToSync->setText(QString::number(ui->sliderMaxTweets->value()));
    }
    if (slider->objectName()=="sliderTimeToSync") {
        ui->valueSyncTime->setText(QString::number(ui->sliderTimeToSync->value()));
    }
}

void Settings::saveChanges()
{
    QStringList values;
    values.append(ui->valueTweetsUsertimeline->text());
    values.append(ui->valueMaxSearchTweets->text());
    values.append(ui->valueMaxUsers->text());
    values.append(ui->valueMaxTweetsToSync->text());
    values.append(ui->valueSyncTime->text());
    settingsDB->setSettings(1,values);
    close();
}

void Settings::closeEvent(QCloseEvent *)
{
    emit closed();
}
