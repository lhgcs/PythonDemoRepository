QT += core network
QT -= gui

TARGET = tcpclient
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    linuxsocket.cpp \
    tcpthread.cpp

DISTFILES += \
    tcpclient.pro.user

HEADERS += \
    linuxsocket.h \
    tcpthread.h

