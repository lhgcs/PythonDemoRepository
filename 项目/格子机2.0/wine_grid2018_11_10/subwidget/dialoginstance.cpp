#include "dialoginstance.h"

DialogInstance *DialogInstance::m_pInstance=NULL;

DialogInstance::DialogInstance(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);//会让它置于所有程序的顶层
    this->setObjectName("DialogInstance");
    this->hide();

    this->setFixedSize(400,200);
    this->move_to_centre();
    label=new QLabel(this);
    label->setObjectName("labelDialog");
    label->setFixedSize(360,120);
    label->setAlignment(Qt::AlignCenter);
    label->clear();
    button=new QPushButton(this);
    button->setFocusPolicy(Qt::NoFocus);
    button->setFixedSize(360,50);
    QObject::connect(button,SIGNAL(clicked(bool)),this,SLOT(close()));

    vl=new QVBoxLayout;
    vl->addSpacing(20);
    vl->addWidget(label,1,Qt::AlignHCenter);
    vl->addWidget(button,1,Qt::AlignHCenter);
    setLayout(vl);

    timer=new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(close()));
}

