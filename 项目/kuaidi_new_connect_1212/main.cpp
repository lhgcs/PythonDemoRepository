#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include "configure.h"
#include <qfontdatabase.h>

//open的头文件
//#include<fcntl.h>

int main(int argc, char *argv[])
{
    //设置QObject的成员函数tr()的编码器，后面程序中都可以用tr()直接转换字符串
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    MainWindow w;

    int id = QFontDatabase::addApplicationFont("/mnt/DroidSansFallback.ttf");
    QString msyh = QFontDatabase::applicationFontFamilies (id).at(0);
    QFont font(msyh,10);
    qDebug()<<msyh<<endl;
    font.setPointSize(20);
    a.setFont(font);

    //w.setWindowState(Qt::WindowFullScreen);

   /* QFont font;
    font.setFamily("wenquanyi");
    font.setPixelSize(15);
    w.setFont(font);
    */
#ifdef PC_DEBUG
    w.setFixedSize(768,1366);
    w.show();
#else
    w.showFullScreen();
#endif

/*
    int fd = open("/dev/pwm", 0);
    if (fd < 0) {
        perror("open pwm_buzzer device");
        //exit(1);
    }
*/

    return a.exec();
}
