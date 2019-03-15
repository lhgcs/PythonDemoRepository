#include "dialog.h"
#include <QBoxLayout>
#include <QDebug>
#include "app.h"

Dialog::Dialog(QString str, int _ms, QWidget *parent):QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);//会让它置于所有程序的顶层
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setObjectName("Dialog");
    ms=_ms;

    this->setFixedSize(400,220);
    this->move((App::DeskWidth-400)/2,(App::DeskHeight-200)/2);
    label=new QLabel(this);
    label->setObjectName("labelDialog");
    label->setFixedSize(360,120);
    label->setAlignment(Qt::AlignCenter);
    label->setText(str);
    button=new QPushButton(this);
    button->setFocusPolicy(Qt::NoFocus);
    button->setFixedSize(360,50);
    QObject::connect(button,SIGNAL(clicked(bool)),this,SLOT(close()));

    QVBoxLayout *vl=new QVBoxLayout;
    vl->addSpacing(50);
    vl->addWidget(label,1,Qt::AlignHCenter);
    vl->addWidget(button,1,Qt::AlignHCenter);
    setLayout(vl);

    timer=new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(close()));
    this->show();
    shake_flag=false;
}

Dialog::Dialog(QWidget *parent):QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);//会让它置于所有程序的顶层
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setObjectName("Dialog");

    this->setFixedSize(400,220);
    this->move((App::DeskWidth-400)/2,(App::DeskHeight-200)/2);
    label=new QLabel(this);
    label->setObjectName("labelDialog");
    label->setFixedSize(360,120);
    label->setAlignment(Qt::AlignCenter);
    label->clear();
    button=new QPushButton(this);
    button->setFocusPolicy(Qt::NoFocus);
    button->setFixedSize(360,50);
    QObject::connect(button,SIGNAL(clicked(bool)),this,SLOT(close()));

    QVBoxLayout *vl=new QVBoxLayout;
    vl->addSpacing(50);
    vl->addWidget(label,1,Qt::AlignHCenter);
    vl->addWidget(button,1,Qt::AlignHCenter);
    setLayout(vl);

    timer=new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(close()));
    this->show();
    shake_flag=false;
}

//        "QPushButton{"
//            "color:rgb(51,51,51); "
//            "background-color:rgba(255,255,255,50);"
//            "border:0px solid rgb(111,156,207);"
//            "border-radius: 5px;"
//            "border-style:outset;"
//        "}"
//        "QPushButton:hover{"
//            "background-color:rgba(255,255,255,100);"
//            "border-style:inset; "
//        "}"
//        "QPushButton:pressed{"
//            "background-color:rgba(255,255,255,100);"
//            "padding-left:3px;"
//            "padding-top:3px;"
//        "}"

//            "background-color:rgb(232,241,252);"
//            "border:2px dashed rgb(111,156,207);"

//"<font color=red  size=3>请输入正确的商品编号</font>"
