#-------------------------------------------------
#
# Project created by QtCreator 2017-07-04T15:15:45
#
#-------------------------------------------------

QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = drink
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    configure/app.cpp \
    configure/globalapplication.cpp \
    configure/screensaver.cpp \
    configure/toast.cpp \
    configure/xmldom.cpp \
    util/filehandler.cpp \
    util/firmwaredownload.cpp \
    model/load.cpp \
    subwidget/dialoginstance.cpp \
    subwidget/numkeyboard.cpp \
    subwidget/virtualkeyboard.cpp \
    model/httprequest.cpp \
    view/basestackedwidget.cpp \
    view/basewidget.cpp \
    subwidget/goodsgrid.cpp \
    subwidget/goodsitem.cpp \
    serial/serial.cpp \
    serial/serialputout.cpp \
    view/addware.cpp \
    subwidget/addorreduce.cpp \
    network/checknetworkstatus.cpp \
    network/connecttonetwork.cpp \
    network/module_4g.cpp \
    network/pingutil.cpp \
    network/shellutil.cpp \
    network/dhcpmsg.c \
    network/netutil.c \
    view/paywidget.cpp \
    view/settingwidget.cpp \
    model/goodinfo.cpp \
    model/httpdownfilethread.cpp \
    subwidget/basesubwidget.cpp \
    subwidget/downfileprogress.cpp \
    subwidget/indicator.cpp \
    subwidget/statusbar.cpp \
    configure/configure.cpp \
    configure/watchdog.cpp \
    control/linuxsocket.cpp \
    cjsonhelper.c \
    md5_1.cpp \
    md5/md5.c \
    md5/getmd5.cpp \
    md5/getcrc16.cpp \
    control/gpio.cpp \
    jsonhandler.cpp \
    labelandedit.cpp \
    paramterwidget.cpp \
    numericalkeyboard.cpp \
    logindialog.cpp \
    btncallback.cpp \
    model/databasehelper.cpp \
    model/tablegoodinfo.cpp \
    model/tableinterface.cpp \
    model/tableputouttimeinfo.cpp \
    view/settingtimewidget.cpp \
    model/mysqltablemodel.cpp \
    model/mysqlquerymodel.cpp


HEADERS  += mainwindow.h \
            qrencode.h \
    configure/app.h \
    configure/globalapplication.h \
    configure/screensaver.h \
    configure/toast.h \
    configure/xmldom.h \
    util/filehandler.h \
    util/firmwaredownload.h \
    model/httprequest.h \
    model/load.h \
    subwidget/dialoginstance.h \
    subwidget/numkeyboard.h \
    subwidget/virtualkeyboard.h \
    view/basestackedwidget.h \
    view/basewidget.h \
    subwidget/goodsgrid.h \
    subwidget/goodsitem.h \
    serial/serial.h \
    serial/serialputout.h \
    view/addware.h \
    subwidget/addorreduce.h \
    network/checknetworkstatus.h \
    network/connecttoeth.h \
    network/connecttonetwork.h \
    network/dhcpmsg.h \
    network/module_4g.h \
    network/netutil.h \
    network/pingutil.h \
    network/shellutil.h \
    view/paywidget.h \
    view/settingwidget.h \
    model/goodinfo.h \
    model/httpdownfilethread.h \
    subwidget/basesubwidget.h \
    subwidget/downfileprogress.h \
    subwidget/indicator.h \
    subwidget/statusbar.h \
    configure/configure.h \
    configure/watchdog.h \
    control/linuxsocket.h \
    cjsonhelper.h \
    md5/md5.h \
    md5/getmd5.h \
    md5/getcrc16.h \
    control/gpio.h \
    jsonhandler.h \
    labelandedit.h \
    paramterwidget.h \
    numericalkeyboard.h \
    logindialog.h \
    btncallback.h \
    model/databasehelper.h \
    model/tablegoodinfo.h \
    model/tableinterface.h \
    model/tableputouttimeinfo.h \
    view/settingtimewidget.h \
    model/mysqltablemodel.h \
    model/mysqlquerymodel.h

DISTFILES += \
    libqrencode.so.3.4.4 \
    configure/ui.qss

LIBS +=-L$$PWD/ -lqrencode -lrt
#LIBS +=-L$$PWD/qrencode-3.4.4/_install/lib -lqrencode

RESOURCES += \
    qss.qrc

FORMS +=
