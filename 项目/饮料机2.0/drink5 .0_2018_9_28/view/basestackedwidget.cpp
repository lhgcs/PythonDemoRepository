#include "basestackedwidget.h"
#include "configure/app.h"

baseStackedWidget::baseStackedWidget(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("baseStackedWidget");
    this->setFixedSize(App::screenWidget,App::screenHeight);
}

