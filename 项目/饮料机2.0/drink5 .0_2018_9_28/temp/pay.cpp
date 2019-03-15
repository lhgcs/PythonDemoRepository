#include "pay.h"
#include <QHBoxLayout>
#include <QPixmap>
#include <QDebug>
#include "my_define.h"

#define BUTTON_W 150

Pay::Pay(int fd,int select, QString *const _price, int *const status, QWidget *parent):QWidget(parent)
{
    this->setFixedSize(720,IMG_H+15+120);
    this->move(0,VIDEO_HEIGHT+10);//+LAYOUT_V
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(232,241,252));//232,241,252
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    button_close=new QPushButton("返回",this);
    button_close->setMaximumHeight(50);
    button_close->setFocusPolicy(Qt::NoFocus);
    button_close->setFixedWidth(BUTTON_W-20);

    label_image=new QLabel(this);
    label_image->setFixedWidth(240);
    label_image->setFixedHeight(270);
    label_image->setAlignment(Qt::AlignCenter);
    label_price=new QLabel(this);
    label_price->setMaximumHeight(50);
    label_price->setAlignment(Qt::AlignCenter);
    label=new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->setFixedWidth(360);
    label_price->setObjectName("label_price");

    label_stock=new QLabel(this);
    label_stock->setMaximumHeight(40);
    label_stock->setAlignment(Qt::AlignCenter);
    label_stock->setObjectName("label_stock");

    label->setText("请选择支付方式，\n并扫码支付");
    button_weixin=new QPushButton(this);
    button_alipay=new QPushButton(this);
    button_weixin->setMaximumHeight(BUTTON_W);
    button_alipay->setMaximumHeight(BUTTON_W);
    button_weixin->setMaximumWidth(BUTTON_W);
    button_alipay->setMaximumWidth(BUTTON_W);
    button_weixin->setFocusPolicy(Qt::NoFocus);
    button_alipay->setFocusPolicy(Qt::NoFocus);
    button_weixin->setObjectName("button_weixin");
    button_alipay->setObjectName("button_alipay");
    QObject::connect(button_weixin,SIGNAL(clicked(bool)),this,SLOT(send_data_slots()));
    QObject::connect(button_alipay,SIGNAL(clicked(bool)),this,SLOT(send_data_slots()));

    QPixmap weixin("/home/ico/weixin.png");
    QPixmap alipay("/home/ico/alipay.png");
    int width=weixin.width()>BUTTON_W?BUTTON_W:weixin.width();
    int height=weixin.height()>BUTTON_W?BUTTON_W:weixin.height();
    button_weixin->setIconSize(QSize(width,height));
    button_alipay->setIconSize(QSize(width,height));
    button_weixin->setIcon(weixin);
    button_alipay->setIcon(alipay);

    this->setStyleSheet(
    "QLabel{background-color:rgb(232,241,252);  color:rgb(76,177,52); font-size:32px;}"
   // "#label_stock{background-color:rgb(232,241,252);  color:rgb(0,163,229);}"//font-size:28px;
    "QPushButton{background-color:rgba(0,0,0,0);border:0px solid rgb(111,156,207);border-style:outset;border-radius: 5px;}"
    );
    button_close->setObjectName("button_close");//202,5,9
    button_close->setStyleSheet("#button_close{background-color:rgb(0,163,229);  color:rgb(255,255,255);border:0px;font-size:40px;}");
    QObject::connect(button_close,SIGNAL(clicked(bool)),this,SLOT(close()));

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(label_image, 0, 0, 2, 1);//第0行，第0列开始，占1行3列
    pLayout->addWidget(label_price, 3, 0, 1, 1);
    pLayout->addWidget(label_stock, 2, 0, 1, 1);

    QHBoxLayout *v = new QHBoxLayout;
    v->addStretch();  //添加伸缩
    v->addWidget(button_close);
    v->addSpacing(20+20);
    pLayout->addLayout(v,3, 1, 1, 2);

    QHBoxLayout *v1 = new QHBoxLayout;
    v1->addStretch();  //添加伸缩
    v1->addWidget(button_weixin);
    QHBoxLayout *v2 = new QHBoxLayout;
    v2->addWidget(button_alipay);
    v2->addStretch();  //添加伸缩
    pLayout->addLayout(v1,1,1,1,1);
    pLayout->addLayout(v2,1,2,1,1);
    pLayout->addWidget(label,0, 1, 1, 2);

    // 设置水平间距
    pLayout->setHorizontalSpacing(10);
    // 设置垂直间距
    pLayout->setVerticalSpacing(0);
    // 设置外间距
    pLayout->setContentsMargins(LAYOUT_H, 0, LAYOUT_H, 120); //left, top, right, bottom
    setLayout(pLayout);

    httpRequest =new HttpRequest(this);
    QObject::connect(httpRequest,SIGNAL(http_qrencode(QString)),this,SLOT(show_qrencode(QString)));
//    QObject::connect(httpRequest,SIGNAL(http_open(int)),this,SLOT(put_out(int)));
//    QObject::connect(httpRequest,SIGNAL(http_status()),this,SLOT(send_status_slots()));
    QObject::connect(httpRequest,SIGNAL(http_data(int,int,int)),this,SLOT(http_data_slot(int,int,int)));    ///////////////////////////////////////

    read_serial=new ReadSerialThread(this);
    QObject::connect(read_serial,SIGNAL(serial_result(int,int)),this,SLOT(read_serial_result(int,int)));

    timer_loading=new QTimer(this);
    timer_loading->setSingleShot(true);
    QObject::connect(timer_loading,SIGNAL(timeout()),this,SLOT(loading_time_out()));

    timer_putout=new QTimer(this);
    timer_putout->setSingleShot(true);
    QObject::connect(timer_putout,SIGNAL(timeout()),this,SLOT(putOut_time_out()));

    timer_close=new QTimer(this);
    timer_close->setSingleShot(true);
    QObject::connect(timer_close,SIGNAL(timeout()),this,SLOT(close_time_out()));

    this->fd=fd;
    this->price_p=_price;
    this->status_p=status;
    this->change_image_price(select);
    read_serial->start();
}
