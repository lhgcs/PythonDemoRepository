#include "screensaver.h"

ScreenSaver::ScreenSaver(QWidget *parent) : QWidget(parent)
{
    this->hide();
    this->setFixedSize(720,1280);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);//会让它置于所有程序的顶层

    timerSpx=new QTimer(this);
    timerSpx->setInterval(10000);
    QObject::connect(timerSpx,SIGNAL(timeout()),this,SLOT(timerSpx_timeout()));
    labelSpx=new QLabel(this);
    //labelSpx->setPixmap(QPixmap(SPX_PNG).scaled(QSize(WIDTH,HEIGHT),Qt::IgnoreAspectRatio));
    //Qt::KeepAspectRatio的使用，默认参数是Qt::IgnoreAspectRatio  Qt::KeepAspectRatioByExpanding
    labelSpx->setScaledContents(true);          //设置图片显示时自动匹配大小
    labelSpx->installEventFilter(this);
    labelSpx->setMinimumSize(720,1280);
//    labelSpx->showFullScreen();
//    this->showFullScreen();
    set_gif("/home/ico/ad.gif");
    timerSpx->start();
}

