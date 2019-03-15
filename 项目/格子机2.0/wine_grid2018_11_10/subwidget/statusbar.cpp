#include "statusbar.h"

StatusBar::StatusBar(QWidget *parent) : BaseSubWidget(parent){
    this->setObjectName("StatusBar");
    this->setFixedSize(App::screenWidget,25);
    timerId=-1;

    label_mac_id =new QLabel(this);
    label_Temp   =new QLabel(this);
    label_time   =new QLabel(this);
    label_network=new QLabel(this);
    label_mac_id ->setFixedHeight(25);
    label_Temp   ->setFixedHeight(25);
    label_time   ->setFixedHeight(25);
    label_network->setFixedHeight(25);
    label_mac_id ->setAlignment(Qt::AlignVCenter);
    label_Temp   ->setAlignment(Qt::AlignVCenter);
    label_time   ->setAlignment(Qt::AlignVCenter);
    label_network->setAlignment(Qt::AlignVCenter);

    QHBoxLayout *hl = new QHBoxLayout(this);
    hl->setContentsMargins(0,0,0,0);
    hl->addWidget(label_time,   0,Qt::AlignVCenter);
    hl->addStretch(1);
    hl->addWidget(label_Temp,   0,Qt::AlignVCenter);
    hl->addStretch(1);
    hl->addWidget(label_mac_id, 0,Qt::AlignVCenter);
    hl->addWidget(label_network,0,Qt::AlignVCenter);
    this->setLayout(hl);

    cnt=0;
}

