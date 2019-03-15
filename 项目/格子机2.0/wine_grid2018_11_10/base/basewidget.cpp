#include "basewidget.h"

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//无边框

    this->setAttribute(Qt::WA_X11DoNotAcceptFocus, true);//无焦点点击
    this->resize(720,720);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setObjectName("BaseWidget");
    this->setStyleSheet(
//        "#BaseWidget{"
//            "border-image:url(/home/ico/wine.jpg);"//border-image
//        "}"
//         "QLabel{"
//            "font-size:36px;"
//            "color:rgb(255,246,0);"//253,65,53
//            "background-color:rgba(0,0,0,255);"
//            "border:none;"
//         "}"
         "QLineEdit{"
            "font-size:36px;"
            "color:rgb(0,0,0);"
            "background-color:rgba(0,0,0,0);"
            "border:1px solid rgb(111,156,207);"
         "}"
        "QToolButton{"
           "font:36px;"
           "color:rgba(0,0,0,255);"
           "background-color:rgba(0,0,0,0);"
           "border:none;"
        "}"
    );

    QPalette palette = this->palette();
    QPixmap pixmap("/home/ico/wine.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
//    palette.setBrush(QPalette::Background,QBrush(Qt::red));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}

