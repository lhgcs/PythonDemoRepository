#include "shoppingcart.h"
#include "QBoxLayout"

#define BUTTON_W 70//60
#define BUTTON_H 50

#define LABEL_W 40//53
#define LABEL_H 40//40

ShoppingCart::ShoppingCart(QWidget *parent):QWidget(parent)
{
    this->setObjectName("shopping");
    this->setStyleSheet(
        "#shopping{background-color:rgb(232,241,252);}"//rgb(232,241,252)
        "QLabel{background-color:rgba(0,0,0,0);  color:rgba(0,255,0,255);}"
        "QPushButton{background-color:rgba(0,0,0,0); color:rgba(0,255,0,255);border-radius:0px; }"
    );
    this->setWindowFlags(Qt::FramelessWindowHint);
    button_add=new QPushButton("+",this);
    button_sub=new QPushButton("-",this);
    label=new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->setText("0");

    QObject::connect(button_add,SIGNAL(clicked(bool)),this,SLOT(button_add_slot()));
    QObject::connect(button_sub,SIGNAL(clicked(bool)),this,SLOT(button_sub_slot()));

    button_add->setFixedSize(BUTTON_W,BUTTON_H);
    button_sub->setFixedSize(BUTTON_W,BUTTON_H);
    label->setFixedSize(LABEL_W,LABEL_H);
//    label->setMaximumSize(LABEL_W,LABEL_H);
//    this->setFixedHeight(50);
    QHBoxLayout *h=new QHBoxLayout;
    h->addWidget(button_sub);
    h->addWidget(label);
    h->addWidget(button_add);
    h->setContentsMargins(0,0,1,0);
    this->setLayout(h);
    this->show();
}

