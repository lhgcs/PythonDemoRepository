#include "addorreduce.h"

#define WIDTH_MAX 100
#define WIDTH_MIN 50

AddOrReduce::AddOrReduce(QWidget *parent) : BaseSubWidget(parent)
{
    this->setObjectName("AddOrReduce");

    checkBox=new QCheckBox(this);
    checkBox->setFocusPolicy(Qt::NoFocus);
    checkBox->setChecked(false);

    btnAdd=new QPushButton("+",this);
    btnSub=new QPushButton("-",this);
    btnAdd->setFocusPolicy(Qt::NoFocus);
    btnSub->setFocusPolicy(Qt::NoFocus);
    label=new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->setText("0");

    checkBox->setFixedSize(WIDTH_MAX,50);

    btnAdd->setMaximumSize(WIDTH_MAX,64);
    btnSub->setMaximumSize(WIDTH_MAX,64);
    label->setMaximumSize(WIDTH_MAX,64);

    btnAdd->setMinimumSize(WIDTH_MIN,30);
    btnSub->setMinimumSize(WIDTH_MIN,30);
    label->setMinimumSize(WIDTH_MIN,30);

    this->setMaximumSize(WIDTH_MAX*3.3,60);
    this->setMinimumSize(WIDTH_MIN*3.3,30);

    QObject::connect(btnAdd,SIGNAL(clicked(bool)),this,SLOT(button_add_slot()));
    QObject::connect(btnSub,SIGNAL(clicked(bool)),this,SLOT(button_sub_slot()));

    h=new QHBoxLayout(this);
    h->setMargin(0);
    h->addWidget(checkBox);
    h->addWidget(btnSub);
    h->addWidget(label);
    h->addWidget(btnAdd);
    this->setLayout(h);
    minLimit=0;
    maxLimit=10;
    id=0;
}

