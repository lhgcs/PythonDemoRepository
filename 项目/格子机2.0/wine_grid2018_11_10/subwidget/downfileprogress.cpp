#include "downfileprogress.h"

DownFileProgress::DownFileProgress(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(App::screenWidget,2);
    init();
    timer=new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(timer_slot()));
}

int DownFileProgress::getTotal() const
{
    return total;
}

void DownFileProgress::setTotal(int value)
{
    total = value;
}

int DownFileProgress::getCurrent() const
{
    return current;
}

void DownFileProgress::setCurrent(int value)
{
    current = value;
}

