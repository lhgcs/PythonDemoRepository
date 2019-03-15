#include "goodstable.h"

GoodsTable::GoodsTable(QWidget *parent) : QWidget(parent)
{
   // this->setMaximumSize(parent->width(),parent->height());
    this->setFixedSize(App::DeskWidth-20,620);
    this->setObjectName("GoodsTable");
    for(int i=0;i<App::goodsNum;i++){
        goodsDisplay.append(new GoodsDisplay(this));
    }
    gl=new QGridLayout(this);
    gl->setContentsMargins(50,0,50,0);
    gl->setVerticalSpacing(20);
    for(int i=0;i<3;i++){
        gl->addWidget(goodsDisplay.at(i),0,i,1,1);
        gl->addWidget(goodsDisplay.at(i+3),1,i,1,1);
        goodsDisplay.at(i)->installEventFilter(this);
        goodsDisplay.at(i+3)->installEventFilter(this);
    }
    for(int i=0;i<App::goodsNum;i++){
        goodsDisplay.at(i)->setPicture("/home/image/"+QString::number(i+1)+".png");
    }
    this->setLayout(gl);
    this->show();
}
