#include "addgoods.h"

AddGoods::AddGoods(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(parent->width(),parent->height());
    tableView=new TableView(this);
    tableView->setFixedSize(parent->width(),parent->height());
    btnUpLoad=new QPushButton("上传数据",this);
    btnUpLoad->setObjectName("btnOk");
    vl=new QVBoxLayout(this);
    vl->setContentsMargins(0,0,0,0);
    vl->addWidget(tableView);
    vl->addWidget(btnUpLoad);
    vl->addStretch();
    this->setLayout(vl);
    QObject::connect(btnUpLoad,SIGNAL(clicked(bool)),this,SLOT(btnUpLoad_slots()));
}

