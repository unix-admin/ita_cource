#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T12:28:51
#
#-------------------------------------------------

QT       += core gui network webkitwidgets

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
    autorize.cpp

HEADERS  += mainwindow.h \
    base64.h \
    HMAC_SHA1.h \
    liboauthcpp.h \
    SHA1.h \
    urlencode.h \
    twitter.h \
    setpin.h \
    autorize.h

FORMS    += mainwindow.ui \
    setpin.ui \
    autorize.ui

RESOURCES += \
    resources.qrc