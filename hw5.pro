#-------------------------------------------------
#
# Project created by QtCreator 2015-05-02T21:35:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hw5
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gameboard.cpp

HEADERS  += mainwindow.h \
    gameboard.h \
    countdowntimer.h

FORMS    += mainwindow.ui \
    gameboard.ui \
    countdowntimer.ui

CONFIG   += c++11

RESOURCES += \
    image.qrc
