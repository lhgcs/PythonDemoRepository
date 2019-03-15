#include "basesubwidget.h"
#include "configure/app.h"

BaseSubWidget::BaseSubWidget(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setObjectName("BaseSubWidget");
    this->setFixedSize(App::screenWidget,App::screenHeight);
}

