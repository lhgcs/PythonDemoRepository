#-------------------------------------------------
#
# Project created by QtCreator 2017-12-07T10:20:08
#
#-------------------------------------------------

QT       += core gui network sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wine_grid
TEMPLATE = app
#配置TEMPLATE为subdirs设置多工程
#TEMPLATE = subdirs

#配置SUBDIRS，指定需要添加的工程模块
SOURCES += main.cpp\
        mainwindow.cpp \
    filehandler.cpp \
    basewidget.cpp \
    numinputdialog.cpp \
    app.cpp \
    httprequest.cpp \
    usbhandler.cpp \
    toast.cpp \
    usbwatcher.cpp \
    usbscanergun.cpp \
    qrencode-3.4.4/bitstream.c \
    qrencode-3.4.4/mask.c \
    qrencode-3.4.4/mmask.c \
    qrencode-3.4.4/mqrspec.c \
    qrencode-3.4.4/qrencode.c \
    qrencode-3.4.4/qrinput.c \
    qrencode-3.4.4/qrspec.c \
    qrencode-3.4.4/rscode.c \
    qrencode-3.4.4/split.c \
    selectdevice.cpp \
    selectgoods.cpp \
    goodspresent.cpp \
    signallight.cpp \
    loadingqrcode.cpp \
    selectdeviceradio.cpp \
    putouttip.cpp \
    replenishment.cpp \
    switchbutton.cpp \
    subwidget/dialoginstance.cpp \
    subwidget/goodsgrid.cpp \
    subwidget/basestackedwidget.cpp \
    subwidget/basesubwidget.cpp \
    subwidget/goodsitem.cpp \
    subwidget/indicator.cpp \
    buttonradiogroup.cpp \
    buttoncircle.cpp \
    subwidget/ensuredialog.cpp \
    pickupcode.cpp

HEADERS  += mainwindow.h \
    filehandler.h \
    basewidget.h \
    numinputdialog.h \
    app.h \
    httprequest.h \
    qrencode.h \
    usbhandler.h \
    toast.h \
    usbwatcher.h \
    usbscanergun.h \
    qrencode-3.4.4/bitstream.h \
    qrencode-3.4.4/mask.h \
    qrencode-3.4.4/mmask.h \
    qrencode-3.4.4/mqrspec.h \
    qrencode-3.4.4/qrencode.h \
    qrencode-3.4.4/qrencode_inner.h \
    qrencode-3.4.4/qrinput.h \
    qrencode-3.4.4/qrspec.h \
    qrencode-3.4.4/rscode.h \
    qrencode-3.4.4/split.h \
    qrencode-3.4.4/config.h \
    selectdevice.h \
    selectgoods.h \
    goodspresent.h \
    signallight.h \
    loadingqrcode.h \
    selectdeviceradio.h \
    putouttip.h \
    replenishment.h \
    switchbutton.h \
    subwidget/dialoginstance.h \
    subwidget/goodsgrid.h \
    subwidget/basestackedwidget.h \
    subwidget/basesubwidget.h \
    subwidget/goodsitem.h \
    subwidget/indicator.h \
    buttonradiogroup.h \
    buttoncircle.h \
    subwidget/ensuredialog.h \
    pickupcode.h

include ($$PWD/StatusBar/StatusBar.pri)
#include ($$PWD/GoodsDisplay/GoodsDisplay.pri)
include ($$PWD/Sqlite/Sqlite.pri)
include ($$PWD/Setting/Setting.pri)

include ($$PWD/Serial/Serial.pri)
include ($$PWD/Loading/Loading.pri)

include ($$PWD/IC/IC.pri)
include ($$PWD/network/network.pri)

include ($$PWD/Login/Login.pri)
include ($$PWD/Upgrade/Upgrade.pri)

#INCLUDEPATH +=$$PWD StatusBar
#include(StatusBar/StatusBar.pri)

LIBS +=-L$$PWD/ -lrt
#LIBS +=-L/home/yk/Desktop/qrencode-3.4.4/_install/lib -lqrencode
#LIBS +=-L$$PWD/qrencode-3.4.4/_install/lib -lqrencode

DISTFILES +=

RESOURCES += \
    qss.qrc

OTHER_FILES += \
    qrencode-3.4.4/autogen.sh \
    qrencode-3.4.4/ChangeLog \
    qrencode-3.4.4/config.h.in \
    qrencode-3.4.4/configure \
    qrencode-3.4.4/COPYING \
    qrencode-3.4.4/Doxyfile \
    qrencode-3.4.4/libqrencode.pc.in \
    qrencode-3.4.4/Makefile.am \
    qrencode-3.4.4/Makefile.in \
    qrencode-3.4.4/NEWS \
    qrencode-3.4.4/qrencode.1.in \
    qrencode-3.4.4/qrencode.spec \
    qrencode-3.4.4/qrencode.spec.in \
    qrencode-3.4.4/README \
    qrencode-3.4.4/TODO \
    qrencode-3.4.4/acinclude.m4 \
    qrencode-3.4.4/aclocal.m4 \
    qrencode-3.4.4/configure.ac
