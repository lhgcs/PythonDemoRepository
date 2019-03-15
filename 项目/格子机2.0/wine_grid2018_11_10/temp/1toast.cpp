#include "toast.h"

Toast::Toast(QWidget *parent) : QLabel(parent)
{
    this->hide();
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    //this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setAlignment(Qt::AlignCenter);
    this->setObjectName("Toast");

//    label->setScaledContents(true);   //自动和文本一样的高度
//    LabelName->setWordWrap(true);
//    LabelName->setAlignment(Qt::AlignTop);

}

Toast *Toast::pToast = NULL;
