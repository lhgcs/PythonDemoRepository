#include "control_state.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "my_define.h"

Control_State::Control_State(int status, QWidget *parent):QWidget(parent)
{
    this->resize(WIDTH,HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setObjectName("Control_State");

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(50,50,50,255)); //设置背景黑色
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->setStyleSheet(
    "QCheckBox{color: white;spacing: 20px;}"
    "QCheckBox::indicator {width:60px; height:64px;}"//改变大小
    "QCheckBox::indicator:checked {image:url(/home/image/check.png)}"//选中时的样子
    "QCheckBox::indicator:unchecked {image:url(/home/image/uncheck.png)}"
    "QPushButton{border-radius:10px;font-size:50px;}"
    );

    total_box=new QCheckBox("全选",this);
    total_box->setFocusPolicy(Qt::NoFocus);

    for(int i=0;i<GOODS_TOTAL;i++)
    {
        box[i]=new QCheckBox(this);
        box[i]->setText(QString::number(i+1));
        box[i]->setFocusPolicy(Qt::NoFocus);
    }

    button_ok=new QPushButton("更新状态",this);
    button_close=new QPushButton(" 退出 ",this);
    button_open_all=new QPushButton("打开格子",this);
    button_invert=new QPushButton("当前状态",this);
    button_ok->setFocusPolicy(Qt::NoFocus);
    button_close->setFocusPolicy(Qt::NoFocus);
    button_open_all->setFocusPolicy(Qt::NoFocus);
    button_invert->setFocusPolicy(Qt::NoFocus);

    button_ok->setFixedWidth(216);
    button_close->setFixedWidth(216);
    button_open_all->setFixedWidth(216);
    button_invert->setFixedWidth(216);

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(total_box, 0, 0, 1, 2);//第0行，第0列开始，占1行2列

    pLayout->addWidget(button_close,4, 0, 1, 1);
    pLayout->addWidget(button_invert,4, 2, 1, 1);
    pLayout->addWidget(button_open_all, 4, 1, 1, 1);
    pLayout->addWidget(button_ok,4, 3, 1, 1);

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<4;j++)
        {
             pLayout->addWidget(box[i*4+j], i+1, j, 1, 1);
        }
    }

//    pLayout->setHorizontalSpacing(50);// 设置水平间距
//    pLayout->setVerticalSpacing(20);  // 设置垂直间距
//    pLayout->setContentsMargins(100, 50, 100, 50);// 设置外间距

    pLayout->setHorizontalSpacing(20);// 设置水平间距
    pLayout->setVerticalSpacing(20);  // 设置垂直间距
    pLayout->setContentsMargins(50, 50, 50, 50);// 设置外间距
    setLayout(pLayout);

    QObject::connect(button_ok,   SIGNAL(clicked(bool)),this,SLOT(button_ok_slot()));
    QObject::connect(button_close,SIGNAL(clicked(bool)),this,SLOT(button_close_slot()));
    QObject::connect(total_box,   SIGNAL(clicked(bool)),this,SLOT(total_box_slot()));
    QObject::connect(button_open_all,SIGNAL(clicked(bool)),this,SLOT(button_open_slot()));
    QObject::connect(button_invert,SIGNAL(clicked(bool)),this,SLOT(button_invert_slot()));

    this->status=status;
    flag=true;
    show();
}

