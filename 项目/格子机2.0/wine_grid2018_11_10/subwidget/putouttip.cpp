#include "putouttip.h"
#include "app.h"

PutOutTip::PutOutTip(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(App::DeskWidth,App::DeskHeight);
    this->setFocusPolicy(Qt::NoFocus);
    this->setObjectName("LoadingQrcode");

    label=new QLabel(this);
    label->setAlignment(Qt::AlignCenter);

    timer=new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(timer_slot()));

    vl=new QVBoxLayout(this);
    vl->addStretch(1);
    vl->addWidget(label);
    vl->addStretch(1);

    this->setLayout(vl);
    timer->start(10000);
}

