#-------------------------------------------------
#
# Project created by QtCreator 2016-10-17T09:50:53
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tic-tac-toe
TEMPLATE = app

CONFIG += qt
SOURCES += main.cpp\
        mainwindow.cpp \
    serverwaiting.cpp \
    clientconnecting.cpp \
    playingform.cpp \
    network.cpp \
    error.cpp

HEADERS  += mainwindow.h \
    serverwaiting.h \
    clientconnecting.h \
    playingform.h \
    network.h \
    error.h

FORMS    += mainwindow.ui \
    serverwaiting.ui \
    clientconnecting.ui \
    playingform.ui \
    errorwind.ui

DISTFILES += \
    plugins/platform/qwindows.dll
RC_ICONS = icon.ico
