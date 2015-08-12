#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T12:28:51
#
#-------------------------------------------------

QT       += core gui network webkitwidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ita_course
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    base64.cpp \
    HMAC_SHA1.cpp \
    liboauthcpp.cpp \
    SHA1.cpp \
    urlencode.cpp \
    twitter.cpp \
    setpin.cpp \
    autorize.cpp \
    usersearch.cpp \
    userdetails.cpp \
    tweetssearch.cpp \
    parser.cpp \
    requests.cpp \
    database.cpp \
    settings.cpp \
    readableusers.cpp \
    synchronization.cpp

HEADERS  += mainwindow.h \
    base64.h \
    HMAC_SHA1.h \
    liboauthcpp.h \
    SHA1.h \
    urlencode.h \
    twitter.h \
    setpin.h \
    autorize.h \
    usersearch.h \
    userdetails.h \
    tweetssearch.h \
    parser.h \
    requests.h \
    database.h \
    settings.h \
    readableusers.h \
    synchronization.h

FORMS    += mainwindow.ui \
    setpin.ui \
    autorize.ui \
    usersearch.ui \
    userdetails.ui \
    tweetssearch.ui \
    settings.ui \
    readableusers.ui

RESOURCES += \
    resources.qrc

