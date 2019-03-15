#include "basestackedwidget.h"
#include "app.h"

baseStackedWidget::baseStackedWidget(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("baseStackedWidget");
    this->setFixedSize(App::DeskWidth,App::DeskHeight);
}

