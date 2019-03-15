#include "sysstart.h"
#include "my_define.h"

SysStart::SysStart(QWidget *parent):QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//无边框,不可改变大小
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setWindowOpacity(1);
    this->setObjectName("SysStart");
    this->setStyleSheet("#SysStart{background-color:rgb(255,255,255);}" "QLabel{background:transparent}");
    label=new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->resize(WIDTH,HEIGHT);
    movie=new QMovie("/home/ico/start.gif");
    label->setMovie(movie);
    movie->start();
    this->showFullScreen();
}

