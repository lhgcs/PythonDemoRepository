#include "updatestatus.h"
#include <QGridLayout>

UpdateStatus::UpdateStatus(int *const _status_p,QWidget *parent) : QWidget(parent)///QDialog(parent)
{
    status_p=_status_p;
    index=0;
    all_flag=true;
    top_up_flag=true;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet(
      "QCheckBox{color: black;}"//spacing: 20px;
      "QCheckBox::indicator {width:48px; height:48px;}"//改变大小
      "QCheckBox::indicator:checked {image:url(/home/ico/check.png)}"//选中时的样子
      "QCheckBox::indicator:unchecked {image:url(/home/ico/uncheck.png)}"
      "QPushButton{border-radius:0px;font-size:30px;"
      "background-color:rgba(0,0,0,0);border:1px solid rgb(111,156,207);color:rgb(111,156,207)}"
      );
    this->setWindowOpacity(1);

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(232,241,252));//232,241,252
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    checkBox=new QCheckBox[20];
    for(int i=0;i<20;i++)
    {
        checkBox[i].setMaximumWidth(100);
    }
    button=new ShoppingCart[20];
    button_all=new QPushButton("全选本页",this);
    button_top_up=new QPushButton("加满",this);
    button_up=new QPushButton("上传",this);
    button_down=new QPushButton("下载",this);
    button_close=new QPushButton("退出",this);
    button_reset=new QPushButton("校正",this);
    button_all->setMinimumHeight(50);
    button_top_up->setMinimumHeight(50);
    button_up->setMinimumHeight(50);
    button_down->setMinimumHeight(50);
    button_close->setMinimumHeight(50);
    button_reset->setMinimumHeight(50);

    button_previous=new QPushButton("上一页",this);
    button_next=new QPushButton("下一页",this);
    button_previous->setMaximumHeight(100);
    button_next->setMaximumHeight(100);

    QObject::connect(button_all,SIGNAL(clicked(bool)),this,SLOT(button_all_slot()));
    QObject::connect(button_top_up,SIGNAL(clicked(bool)),this,SLOT(button_top_up_slot()));
    QObject::connect(button_up,SIGNAL(clicked(bool)),this,SLOT(button_up_slot()));
    QObject::connect(button_down,SIGNAL(clicked(bool)),this,SLOT(button_down_slot()));
    QObject::connect(button_close,SIGNAL(clicked(bool)),this,SLOT(close()));
    QObject::connect(button_reset,SIGNAL(clicked(bool)),this,SLOT(button_reset_slot()));

    QObject::connect(button_previous,SIGNAL(clicked(bool)),this,SLOT(button_previous_slot()));
    QObject::connect(button_next,SIGNAL(clicked(bool)),this,SLOT(button_next_slot()));

    for(int i=0;i<GOODS_TOTAL;i++)
    {
       temp_status[i]=_status_p[i];
    }
    change_text(index);

    QGridLayout *pl = new QGridLayout();
    for(int i=0;i<10;i++)
    {
        pl->addWidget(&checkBox[i], i, 1, 1, 1);
        pl->addWidget(&button[i], i, 2, 1, 1);
    }

    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    pl->addItem(spacer,0,3,10,1);

    for(int i=0;i<10;i++){
        pl->addWidget(&checkBox[10+i], i, 4, 1, 1);
        pl->addWidget(&button[10+i], i, 5, 1, 1);
    }
    QHBoxLayout *hl=new QHBoxLayout;
    hl->addWidget(button_all);
    hl->addWidget(button_top_up);
    hl->addWidget(button_reset);
    hl->addWidget(button_up);
    hl->addWidget(button_down);
    hl->addWidget(button_previous);
    hl->addWidget(button_next);
    hl->addWidget(button_close);
    hl->setContentsMargins(0,0,0,0);
    pl->addLayout(hl, 10, 0, 1, 7);

    // 设置水平间距
    pl->setHorizontalSpacing(8);
    // 设置外间距
    pl->setContentsMargins(10, 0, 10, 0); //left, top, right, bottom  70  90
    setLayout(pl);

    this->resize(WIDTH,HEIGHT-405);
    this->raise();
    this->move(0,405);
    isbusy=false;
}

