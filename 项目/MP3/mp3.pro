#-------------------------------------------------
#
# Project created by QtCreator 2018-07-20T16:16:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mp3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    playmp3.cpp \
    playsound.cpp

HEADERS  += mainwindow.h \
    playmp3.h \
    playsound-x11/decoder.h \
    playsound-x11/stream.h \
    playsound.h

FORMS    += mainwindow.ui


INCLUDEPATH += ./playsound-x11/alsa/include \
               ./playsound-x11/libmab/include

LIBS += -L$$PWD/playsound-x11/alsa/lib/ -lasound
LIBS += -L$$PWD/playsound-x11/libmab/lib/libmad.a
LIBS += -L$$PWD/playsound-x11/libmab/lib/libmad.la

#LIBS +=-L$$PWD/playsound-x11/libmab/mad/lib/libmad.a #\
#       -L$$PWD/playsound-x11/libmab/mad/lib/libmad.la
#LIBS +=-L$$PWD/playsound/arm-alsa/lib -lasound

#alsamixer

