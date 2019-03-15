#include "loadingqrcode.h"
#include "app.h"
LoadingQrcode::LoadingQrcode(QWidget *parent) : QDialog(parent)
{
    this->setFixedSize(App::DeskWidth,App::DeskHeight);
    this->setFocusPolicy(Qt::NoFocus);
    this->setObjectName("LoadingQrcode");
    //this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    label=new QLabel(this);
    label->setFixedSize(200,200);
    label->setAlignment(Qt::AlignCenter);
    label->setObjectName("label_white");
    labelTip=new QLabel(this);
    labelTip->setAlignment(Qt::AlignCenter);

    btnClose=new QPushButton(this);
    btnClose->setFixedSize(200,60);
    btnClose->setText("返回");
    btnClose->setObjectName("btnBack");
    btnClose->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnClose,SIGNAL(clicked(bool)),this,SLOT(close()));
    timerClose=new QTimer(this);
    timerClose->setSingleShot(true);
    QObject::connect(timerClose,SIGNAL(timeout()),this,SLOT(close()));

    movie=new QMovie("/home/ico/loading.gif");
    movie->setBackgroundColor(Qt::black);
    movie->setScaledSize(QSize(180,180));

    vl=new QVBoxLayout(this);
    vl->addStretch(3);
    vl->addWidget(label,0,Qt::AlignHCenter);
    vl->addWidget(labelTip,0,Qt::AlignHCenter);
    vl->addStretch(1);
    vl->addWidget(btnClose,0,Qt::AlignHCenter);
    vl->addStretch(3);
    this->setLayout(vl);
}

