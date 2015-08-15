#include "settings.h"
#include "ui_settings.h"
#include <QDebug>
Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    settingsDB = new DataBase;
    connect(ui->sliderUsertimeline, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
    connect(ui->sliderSearchedTweets, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
    connect(ui->sliderSearchedUsers, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
    connect(ui->sliderMaxTweets, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
    connect(ui->sliderTimeToSync, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(saveChanges()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));

}

Settings::~Settings()
{
    delete settingsDB;
    delete ui;
}

void Settings::setUserID(QString ID)
{
    userID = ID;
    getSettings();
}

void Settings::getSettings()
{
    Twitter::userSettings userSettings;
    userSettings = settingsDB->getSettings(userID);
    ui->sliderUsertimeline->setValue(userSettings.timelineTweetsByPage.toInt());
    ui->sliderSearchedTweets->setValue(userSettings.searchTweetsByPage.toInt());
    ui->sliderSearchedUsers->setValue(userSettings.searchUsersByPage.toInt());
    ui->sliderMaxTweets->setValue(userSettings.searchTweetsToDatabase.toInt());
    ui->sliderTimeToSync->setValue(userSettings.refreshTime.toInt());
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
    settingsDB->setSettings(userID,values);
    close();
}

void Settings::closeEvent(QCloseEvent *)
{
    emit closed();
}
