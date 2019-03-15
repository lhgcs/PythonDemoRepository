#include "switchbutton.h"

QString SwitchButton::imageOff="/home/ico/switch_off.png";
QString SwitchButton::imageOn="/home/ico/switch_on.png";

SwitchButton::SwitchButton(QWidget *parent) : QWidget(parent)
{
    //this->setFixedSize(136,64);
//    this->checked=true;
//    this->setChecked(false);
    this->setChecked(false);
}
