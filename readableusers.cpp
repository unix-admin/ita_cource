#include "readableusers.h"
#include "ui_readableusers.h"

ReadableUsers::ReadableUsers(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReadableUsers)
{

    ui->setupUi(this);
    buttonIcon.addFile(":/data/images/Delete.ico");
    database = new DataBase;
    clsTwitter = Twitter::getcls();
    readableUsersCount = database->countReadableUsers(clsTwitter->getUserData()->id);
    QList<Twitter::userData> myReadableUsers = database->getReadableUsers(clsTwitter->getUserData()->id);
    layout = new QGridLayout(ui->scrollArea);
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    for(int i=0; i<myReadableUsers.count(); i++)
    {
        QLabel *photo = new QLabel;
        photo->setObjectName("photo"+myReadableUsers.at(i).twitterID);
        photo->setCursor(Qt::PointingHandCursor);
        photo->installEventFilter(this);
        QLabel *text = new QLabel;
        text->setObjectName("text"+myReadableUsers.at(i).twitterID);
        text->setText(myReadableUsers.at(i).name + "\n" + myReadableUsers.at(i).screen_name);
        text->setCursor(Qt::PointingHandCursor);
        text->setAccessibleDescription(myReadableUsers.at(i).twitterID);
        text->installEventFilter(this);
        QPushButton *button = new QPushButton;
        button->setObjectName("button"+myReadableUsers.at(i).twitterID);
        button->setIcon(buttonIcon);
        connect(button,SIGNAL(clicked(bool)),this,SLOT(deleteUser()));
        userPhoto.loadFromData(myReadableUsers.at(i).profile_image_data);
        photo->setPixmap(userPhoto.scaled(70,70,Qt::KeepAspectRatio));
        layout->addWidget(photo,i,0);
        layout->addWidget(text,i,1);
        layout->addWidget(button,i,2);
    }
    ui->scrollAreaWidgetContents->setLayout(layout);
}

ReadableUsers::~ReadableUsers()
{
    delete database;
    delete ui;
}

bool ReadableUsers::eventFilter(QObject *target, QEvent *event)
{
        if (target->objectName().contains("photo"))
        {
         if (event->type() == QEvent::MouseButtonDblClick)
          {
                     UserDetails *detailsForm = new UserDetails;
                     connect(detailsForm,SIGNAL(formClosed()),detailsForm,SLOT(deleteLater()));
                     detailsForm->prepareData(target->objectName().replace("photo",""));
          }
        }
        else
         if (target->objectName().contains("text"))
         {
           if (event->type() == QEvent::MouseButtonDblClick)
           {
               UserDetails *detailsForm = new UserDetails;
               connect(detailsForm,SIGNAL(formClosed()),detailsForm,SLOT(deleteLater()));
               detailsForm->prepareData(target->objectName().replace("text",""));
           }
         }
        return QWidget::eventFilter(target, event);
}

void ReadableUsers::deleteUser()
{
    QMessageBox* deleteMessage = new QMessageBox("Удаление пользователя","Вы действительно хотите удалить выбранного пользователя?", \
                        QMessageBox::Information,
                        QMessageBox::Yes,
                        QMessageBox::No,QMessageBox::Escape);
    int dialogResult = deleteMessage->exec();
    delete deleteMessage;
    if (dialogResult == 3)
    {
        QPushButton *button = qobject_cast<QPushButton *>(sender());
        QLabel *text = this->findChild<QLabel *>(button->objectName().replace("button","text"));
        QLabel *photo = this->findChild<QLabel *>(button->objectName().replace("button","photo"));
        database->deleteUser(button->objectName().replace("button",""),clsTwitter->getUserData()->id);
        photo->deleteLater();
        text->deleteLater();
        button->deleteLater();

    }

}
