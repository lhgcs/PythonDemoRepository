#include "paymentresult.h"

#include "QDebug"

PaymentResult::PaymentResult(QWidget *parent) : QWidget(parent)
{
    this->hide();
    this->setFixedSize(parent->width(),parent->height());
 //   setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
//    this->setFixedSize(400,600);
    qDebug()<<parent->width()<<parent->height();
    this->setObjectName("SeletePayment");

    btnClose=new QPushButton(this);
    btnClose->setObjectName("btnClose");
    QObject::connect(btnClose,SIGNAL(clicked(bool)),this,SLOT(hide()));

    labelQR=new QLabel(this);
    labelQR->setAlignment(Qt::AlignCenter);
    labelQR->setMaximumSize(parent->width(),400);
    labelQR->setMinimumSize(parent->width(),200);
    labelQR->setObjectName("labelDialog");

    btn=new QPushButton(this);
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setMaximumSize(parent->width(),200);
    btn->setMinimumSize(parent->width(),100);
    QObject::connect(btn,SIGNAL(clicked(bool)),this,SLOT(btn_slot()));

    movie=new QMovie("/home/ico/loading.gif");
    movie->setBackgroundColor(Qt::black);
    movie->setScaledSize(QSize(120,120));

    QVBoxLayout *vl=new QVBoxLayout(this);
    vl->setMargin(0);
    vl->addStretch(0);
    vl->addWidget(btnClose,0,Qt::AlignRight);
    vl->addStretch(2);
    vl->addWidget(labelQR,0,Qt::AlignCenter);
    vl->addStretch(1);
    vl->addWidget(btn,0,Qt::AlignCenter);
    vl->addStretch(2);
    this->setLayout(vl);

    timer=new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(timer_slot()));
}

//    QPalette pal(this->palette());
//    pal.setColor(QPalette::Background, QColor(232,241,252)); //设置背景黑色
//    this->setAutoFillBackground(true);
//    this->setPalette(pal);
