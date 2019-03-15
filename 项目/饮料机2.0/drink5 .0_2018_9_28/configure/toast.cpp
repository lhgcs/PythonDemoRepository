#include "toast.h"

Toast *Toast::pToast = NULL;

Toast::Toast(QWidget *parent) : QLabel(parent)
{
    this->hide();
    height=App::screenHeight*6.0/7;
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    //this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setAlignment(Qt::AlignCenter);
    this->setObjectName("Toast");
//    this->setStyleSheet(
//        "QLabel{"
//            "font:26px;"
//            "color:rgb(255,255,255);"
//            "background:rgba(80,80,80,150);"
//            "border-radius:12px;"
//            "border:none;"
//            "padding:20px;"
//        "}");
    timer=new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(close()));
}
