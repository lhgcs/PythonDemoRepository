#include "seletecardpayment.h"

SeleteCardPayment::SeleteCardPayment(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(parent->width(),parent->height());
    this->setObjectName("SeletePayment");

    btnClose=new QPushButton(this);
    btnClose->setObjectName("btnClose");
    QObject::connect(btnClose,SIGNAL(clicked(bool)),this,SLOT(btnClose_slot()));

    label=new QLabel(this);
    label->setMaximumSize(parent->width(),90);
    label->setMinimumSize(parent->width()/1.2,70);
    label->setObjectName("labelCard");
    label->setText("请刷卡");

    lineEdit=new QLineEdit(this);
    lineEdit->setPlaceholderText(QStringLiteral("请输入购买数量(ML)"));
    lineEdit->setMaxLength(5);
    lineEdit->setMaximumSize(parent->width(),80);
    lineEdit->setMinimumSize(parent->width()/1.2,70);

    //添加图标的代码：
    pTrailingAction = new QAction(this);
    pTrailingAction->setPriority(QAction::LowPriority);
    pTrailingAction->setIcon(QIcon("/home/ico/clear.png"));
    lineEdit->addAction(pTrailingAction, QLineEdit::TrailingPosition);
    // 连接信号和槽
    connect(pTrailingAction, SIGNAL(triggered(bool)), this, SLOT(clear_slot(bool)));

    numInputDialog=new NumInputDialog(this);
    numInputDialog->move(lineEdit->x(),lineEdit->y()+lineEdit->height());
    QObject::connect(numInputDialog,SIGNAL(num(int)),this,SLOT(numInput_slot(int)));
    numInputDialog->show();

    vl=new QVBoxLayout(this);
    vl->setMargin(0);
    vl->addWidget(btnClose,0,Qt::AlignRight);
    vl->addWidget(label,0,Qt::AlignRight);
    vl->addWidget(lineEdit,0,Qt::AlignCenter);
    vl->addWidget(numInputDialog,0,Qt::AlignCenter);
    this->setLayout(vl);
    this->show();

    httpRequest=NULL;
    pay=NULL;
    sql=NULL;
}

