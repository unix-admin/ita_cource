#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "database.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    void setUserID(int ID);

private:
    Ui::Settings *ui;
    int userID;
    DataBase *settingsDB;
private:
    void getSettings();
    void setSettings();
signals:
    closed();



private slots:
    void changeValue();
    void saveChanges();
    void closeEvent(QCloseEvent *);
};

#endif // SETTINGS_H
