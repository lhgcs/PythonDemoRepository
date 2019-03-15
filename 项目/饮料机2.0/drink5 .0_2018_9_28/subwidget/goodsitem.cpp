#include "goodsitem.h"

int GoodsItem::pressWidget=0;

GoodsItem::GoodsItem(QWidget *parent) : QWidget(parent)
{
    //this->setObjectName("GoodsItem");
    txt="";
    num=0;
}

