#include "labelandedit.h"
#include "configure/app.h"
LabelAndEdit::LabelAndEdit(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("LabelAndEdit");
    label = new QLabel(this);
    lineEdit = new QLineEdit(this);
    label->setFixedWidth(App::screenWidget*0.18);
    label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    lineEdit->setMaximumWidth(App::screenHeight*0.4);
    lineEdit->setMaxLength(9);
    hl = new QHBoxLayout(this);
    hl->setMargin(5);
    hl->addWidget(label,0,Qt::AlignRight);
    hl->addWidget(lineEdit,0,Qt::AlignLeft);
    this->setLayout(hl);
}

