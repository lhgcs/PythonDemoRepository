#include "seletemoneypayment.h"

SeleteMoneyPayment::SeleteMoneyPayment(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(parent->width(),parent->height());
    this->setObjectName("SeletePayment");

    btnClose=new QPushButton(this);
    btnClose->setObjectName("btnClose");
    btnClose->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnClose,SIGNAL(clicked(bool)),this,SLOT(btnClose_slot()));

    label=new QLabel(this);
    label->setFixedWidth(parent->width());
    label->setMinimumHeight(parent->height()/2);
    label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    label->setObjectName("labelTip");

    btn=new QPushButton("支付",this);
    btn->setObjectName("BtnPay");
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setMaximumSize(parent->width(),100);
    btn->setMinimumSize(parent->width()/2,50);
    QObject::connect(btn,SIGNAL(clicked(bool)),this,SLOT(btn_slot()));

    vl=new QVBoxLayout(this);
    vl->setMargin(0);
    vl->addWidget(btnClose,0,Qt::AlignRight);
    vl->addStretch(1);
    vl->addWidget(label,0,Qt::AlignCenter);
    vl->addStretch(1);
    vl->addWidget(btn,0,Qt::AlignCenter);
    vl->addStretch(1);
    this->setLayout(vl);
    this->show();

    httpRequest=NULL;
    pay=NULL;
    totalMoney="0";
    serialPutOut=NULL;
}

