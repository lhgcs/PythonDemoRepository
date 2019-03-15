#include "paywidget.h"
#include "configure/app.h"

#include <QThread>

PayWidget::PayWidget(QStackedWidget *parent) //: BaseSubWidget(parent)
{
    this->setObjectName("PayWidget");
    this->setFixedSize(App::screenWidget,App::screenHeight*2.0/3);

    button_close=new QPushButton("取消交易",this);
    button_close->setFocusPolicy(Qt::NoFocus);
    button_close->setMaximumSize(250,80);
    button_close->setMinimumSize(200,50);
    //在传参的情况下，slot函数的参数个数一定要小于等于signal函数的参数个数
    //QObject::connect(button_close,SIGNAL(clicked(bool)),this,SLOT(close()));
    QObject::connect(button_close,SIGNAL(clicked(bool)),this,SLOT(button_close_slot()));
    //使用 lambda 表达式连接信号和槽,不受signal参数的影响（因为lambda可以捕获参数）
//    QObject::connect(button_close,&QPushButton::clicked,[=](QObject *obj){
//    });
//    connect(button_close,&QPushButton::clicked,[this](){
//      close();
//    });

    /*
    connect()第五个参数的作用，指定连接方式：默认，队列，直接
    多线程时才有意义
    默认的时候
    如果是多线程，默认使用队列
    如果是单线程， 默认使用直接方式
    队列: 槽函数所在的线程和接收者一样
    直接：槽函数所在线程和发送者一样

    这里重点介绍下connect的第五个参数Qt::ConnectionType。
    此参数可以有三种选择Qt::AutoConnection、Qt::DirectConnection、Qt::QueuedConnection，
    分别是自动连接，直接连接和队列连接。
    正如上面所说，直接连接是同步的，槽函数将和信号同一线程，
    而队列连接是异步的，槽函数会属于接收对象所属线程。
    而自动连接是缺省选项，将自动选择直接连接和队列连接之一。
    而什么时候选择什么连接呢，
    发送者和接收者处于相同线程，选择直接连接；发送者和接收者处于不同线程，使用队列连接。
    */

//    QThread *thread = new QThread;
//    Worker *worker = new Worker();
//    worker->moveToThread(thread);
//    connect(thread, SIGNAL(started()), worker, SLOT(first()));
//    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()),Qt::QueuedConnection);
//    thread->start();

    label_image=new QLabel(this);
    label_image->setMaximumSize(300,400);
    label_image->setMinimumSize(250,300);
    label_image->setAlignment(Qt::AlignCenter);

    label_info=new QLabel(this);
    label_info->setMaximumHeight(80);
    label_info->setAlignment(Qt::AlignCenter);
    label_info->setText("");

    label_weixin=new QLabel(this);
    label_alipay=new QLabel(this);
    label_weixin->setMaximumSize(180,180);
    label_alipay->setMaximumSize(180,180);
    label_weixin->setMinimumSize(150,150);
    label_alipay->setMinimumSize(150,150);
    label_weixin->hasHeightForWidth();
    label_alipay->hasHeightForWidth();

    label_time=new QLabel(this);
    label_time->setFixedSize(70,50);
    label_time->setObjectName("label_time");
    label_time->setAlignment(Qt::AlignRight|Qt::AlignTop);

    QHBoxLayout *hl1 = new QHBoxLayout;
    hl1->setMargin(0);
    hl1->addStretch(1);
    hl1->addWidget(label_weixin,1,Qt::AlignCenter);
    hl1->addSpacing(10);
    hl1->addWidget(label_alipay,1,Qt::AlignCenter);
    hl1->addStretch(1);

    QLabel *labelTip=new QLabel(this);
    labelTip->setText("请使用微信或支付宝扫描进行支付");
    labelTip->setObjectName("labelTip");

    QVBoxLayout *vl1 = new QVBoxLayout;
    vl1->setMargin(0);
    vl1->addLayout(hl1);
    vl1->addWidget(labelTip,1,Qt::AlignHCenter|Qt::AlignTop);
    vl1->addWidget(label_info,1,Qt::AlignCenter|Qt::AlignTop);
    vl1->setStretchFactor(hl1,4);
    vl1->setStretchFactor(label_info,1);

    QHBoxLayout *hl2 = new QHBoxLayout;
    hl2->setMargin(0);
    hl2->addStretch(1);
    hl2->addWidget(label_image,1,Qt::AlignCenter);
    hl2->addStretch(1);
    hl2->addLayout(vl1);
    hl2->addStretch(1);

    QVBoxLayout *vl2 = new QVBoxLayout;
    vl2->setMargin(10);
    vl2->addWidget(label_time,0,Qt::AlignRight|Qt::AlignTop);
    vl2->addStretch(3);
    vl2->addLayout(hl2);
    vl2->addStretch(1);
    vl2->addWidget(button_close,0,Qt::AlignCenter);
    vl2->addStretch(3);

    setLayout(vl2);

    timer_loading=new QTimer(this);
    timer_loading->setSingleShot(true);
    QObject::connect(timer_loading,SIGNAL(timeout()),this,SLOT(loading_time_out()));

    timer_close=new QTimer(this);
    QObject::connect(timer_close,SIGNAL(timeout()),this,SLOT(timer_close_slot()));
    close_cnt=0;

    this->show();
}

