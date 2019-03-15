#include "basewidget.h"
#include "configure/app.h"

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setObjectName("BaseWidget");
    this->setFixedSize(App::screenWidget,App::screenHeight);
}

