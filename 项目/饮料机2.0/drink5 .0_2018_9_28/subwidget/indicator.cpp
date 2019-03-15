#include "indicator.h"
#include "configure/app.h"

Indicator::Indicator(QWidget *parent) : BaseSubWidget(parent)
{
    count=0;
    index=0;
    this->setObjectName("Indicator");
    this->setFixedSize(App::screenWidget,INDICATOR_RADIUS+2);
}

