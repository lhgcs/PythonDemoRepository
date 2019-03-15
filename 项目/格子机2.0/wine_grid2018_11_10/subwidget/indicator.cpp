#include "indicator.h"
#include "app.h"

Indicator::Indicator(QWidget *parent) : BaseSubWidget(parent)
{
    count=0;
    index=0;
    this->setObjectName("Indicator");
    this->setFixedSize(App::DeskWidth,INDICATOR_RADIUS+2);
}

