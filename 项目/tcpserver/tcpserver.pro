#-------------------------------------------------
#
# Project created by QtCreator 2017-12-01T11:57:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcpserver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpthread.cpp

HEADERS  += mainwindow.h \
    tcpthread.h

FORMS    += mainwindow.ui
