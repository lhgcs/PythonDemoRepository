#include "goodsdisplay.h"

GoodsDisplay::GoodsDisplay(QWidget *parent) : QWidget(parent)
{
    labelPicture=new QLabel(this);
    labelPicture->setAlignment(Qt::AlignCenter);
    labelPicture->setScaledContents(true);
    labelPresent=new QLabel(this);
    labelPresent->setAlignment(Qt::AlignCenter);
    labelPicture->setMaximumSize(300,300);
    labelPresent->setMaximumSize(300,300);
    labelPresent->setObjectName("labelPresent");
    vl=new QVBoxLayout(this);
    vl->setContentsMargins(0,0,0,0);
    vl->addWidget(labelPicture,0,Qt::AlignHCenter|Qt::AlignBottom);
    vl->addWidget(labelPresent,0,Qt::AlignHCenter|Qt::AlignTop);
    this->setLayout(vl);
    this->show();
}
