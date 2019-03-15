#include "statusbar.h"
#include "app.h"
StatusBar::StatusBar(QWidget *parent) : QWidget(parent)
{
    this->setMaximumSize(250,40);
    this->setMinimumSize(200,30);
    labelTime=new QLabel(this);
    labelTime->setAlignment(Qt::AlignRight);
    labelTime->setMaximumSize(250,50);
    labelTime->setMinimumSize(200,30);
    timer.start(60000,this);
}

