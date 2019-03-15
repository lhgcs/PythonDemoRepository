#include "buttonradiogroup.h"
#include <QHBoxLayout>
#include "app.h"

ButtonRadioGroup::ButtonRadioGroup(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(App::DeskWidth,App::DeskHeight/4-20);
    buttonCircle=new ButtonCircle[5];
    for (int i=0;i<5;i++){
        buttonCircle[i].setParent(this);
        buttonCircle[i].id=65+i;
        buttonCircle[i].txt=QString(65+i);
        buttonCircle[i].set_size(100);
//        QObject::connect(&buttonCircle[i],SIGNAL(click(int)),this,SIGNAL(click(int)));
        QObject::connect(&buttonCircle[i],SIGNAL(click(int)),
                this,SLOT(click_slot(int)));
    }

    QHBoxLayout *hl=new QHBoxLayout(this);
    hl->setContentsMargins(0,0,0,0);
    hl->addStretch(5);
    for (int i=0;i<5;i++){
        hl->addWidget(&buttonCircle[i]);
    }
    hl->addStretch(5);
    this->setLayout(hl);
}

