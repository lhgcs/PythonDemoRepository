#include "settingdialog.h"
#include "app.h"
SettingDialog::SettingDialog(QWidget *parent):QWidget(parent)
{
    this->hide();
    this->setObjectName("SettingDialog");
    this->setFixedSize(App::DeskWidth-250-40,App::DeskHeight-40);
    label=new QLabel(this);
    label->setText("");

    QStringList list;
    for(int i=0;i<=100;i++){
        list<<QString("%1").arg(i,5,'f',2);
    }
    comboBox_price = new QComboBox(this);
    comboBox_price->addItems(list);

    list.clear();
    for(uint i=0;i<=50000;i+=500){
        list<<QString::number(i);
    }
    comboBox_num = new QComboBox(this);
    comboBox_num->addItems(list);

    comboBox_price->setMaxVisibleItems(15);
    comboBox_num->setMaxVisibleItems(15);

    btnOk=new QPushButton("确定");
    btnCancel=new QPushButton("取消");
    btnOk->setObjectName("btnOk");
    btnCancel->setObjectName("btnCancel");
    connect(btnOk, SIGNAL(clicked(bool)), this, SLOT(on_click_ok()));
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(on_click_cancel()));
    QHBoxLayout *hl=new QHBoxLayout;
    hl->addWidget(btnOk);
    hl->addWidget(btnCancel);

    fl = new QFormLayout(this);
    fl->addRow(QStringLiteral("商品："), label);
    fl->addRow(QStringLiteral("价格："), comboBox_price);
    fl->addRow(QStringLiteral("数量："), comboBox_num);
    fl->addRow(hl);
    fl->setContentsMargins(100,50,100,100);
    this->setLayout(fl);
}
