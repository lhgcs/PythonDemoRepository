#include "showgoods.h"
#include <QGridLayout>

#define TXT_H 35

#include <QSignalMapper>

ShowGoods::ShowGoods(QWidget *parent) : QWidget(parent)
{
    index=0;
    PressMouse=false;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(WIDTH,(TXT_H+ICO_H)*2+20);
    this->setStyleSheet("QToolButton{border: none; background: rgb(232,241,252); color: rgb(0, 160, 230);font-size:24px;}");//background: rgb(68, 69, 73);

    toolButton=new QToolButton[10];
    for(int i=0;i<10;i++)
    {
        toolButton[i].setFixedSize(ICO_W,TXT_H+ICO_H);
        toolButton[i].setIconSize(QSize(ICO_W, ICO_H));
        toolButton[i].setToolButtonStyle(Qt::ToolButtonTextUnderIcon);// 文本位于图标之下
        toolButton[i].installEventFilter(this);
    }
    change_info(0);
    QGridLayout *pl = new QGridLayout();
    for(int i=0;i<5;i++)
    {
        pl->addWidget(&toolButton[i], 0, i, 1, 1);
        pl->addWidget(&toolButton[i+5], 1, i, 1, 1);
    }
    // 设置水平间距
    pl->setHorizontalSpacing(5);
    // 设置外间距
    pl->setContentsMargins(12, 0, 13, 20); //left, top, right, bottom
    setLayout(pl);

    timer=new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(time_out_slot()));
    timer->start(30000);
    this->show();
}

