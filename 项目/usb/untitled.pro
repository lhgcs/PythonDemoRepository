#-------------------------------------------------
#
# Project created by QtCreator 2018-01-09T20:15:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


INCLUDEPATH += /home/yk/Desktop/libusb-0.1.12/libusb-x11/include
LIBS +=-L/home/yk/Desktop/libusb-0.1.12/libusb-x11/lib -lusb

#INCLUDEPATH += /home/yk/Desktop/libusb-1.0.21/libusb-arm/include/libusb-1.0
#LIBS +=-L/home/yk/Desktop/libusb-1.0.21/libusb-arm/lib -lusb-1.0


