#include "goodspresent.h"

GoodsPresent::GoodsPresent(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
//    setWindowFlags(Qt::FramelessWindowHint);
    this->setObjectName("GoodsPresent");
    this->setFixedSize(App::DeskWidth,App::DeskHeight);
    this->move(0,0);

//    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setWindowOpacity(0.5);

//    QPalette bgpal = this->palette();
//    //bgpal.setColor(QPalette::Background, QColor(255,255,255,0));
//    bgpal.setColor(QPalette::Foreground, QColor (255,255,255,0));
//    setPalette(bgpal);

    //setScaledContents(true);
    labelPresent=new QLabel(this);
    labelPresent->setFixedSize(App::DeskWidth/2,App::DeskHeight*2/3);
    labelPresent->setObjectName("labelPresent");
    labelPresent->setAlignment(Qt::AlignCenter);

    int w=App::DeskHeight/10;
    int h=w;

    signalMapper=new QSignalMapper(this);
    button=new QToolButton[3];
    for (int i=0;i<3;i++){
        button[i].setFixedSize(App::DeskWidth/4-1,h+50);
        button[i].setIconSize(QSize(w,h));
        signalMapper->setMapping(&button[i],i);
        QObject::connect(&button[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
    }
    QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(keyPress(int)));

    button[0].setIcon(QIcon("/home/ico/logo_alipay.png"));
    button[1].setIcon(QIcon("/home/ico/logo_weixin.png"));
    button[2].setIcon(QIcon("/home/ico/logo_card.png"));
    button[0].setText("支付宝支付");
    button[1].setText("微信支付");
    button[2].setText("积分兑换");

    btnClose=new QPushButton(this);
    btnClose->setFixedSize(App::DeskWidth/4,h+50);
    btnClose->setText("返回");
    btnClose->setObjectName("btnBack");
    btnClose->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnClose,SIGNAL(clicked(bool)),this,SLOT(btnClose_slot()));

    hl1=new QHBoxLayout();
    hl1->setMargin(0);
    hl1->addStretch(1);
    hl1->addWidget(btnClose);
    hl1->addStretch(2);
    hl1->addWidget(&button[0]);
    hl1->addStretch(2);
    hl1->addWidget(&button[1]);
    hl1->addStretch(2);
    hl1->addWidget(&button[2]);
    hl1->addStretch(1);

    vl=new QVBoxLayout(this);
    vl->setContentsMargins(0,0,0,0);
    vl->addStretch(1);
    vl->addWidget(labelPresent,0,Qt::AlignHCenter);
    vl->addStretch(1);
    vl->addLayout(hl1);

    this->setLayout(vl);
    this->show();
}

