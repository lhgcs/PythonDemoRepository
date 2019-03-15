#include "seletebuy.h"

SeleteBuy::SeleteBuy(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("SeleteBuy");
    this->setFixedSize(400,600);

    btnMoney=new QToolButton(this);
    btnMoney->setObjectName("btnMoney");
    btnMoney->setText(tr("现金支付"));
    btnMoney->setAutoRaise(true);
    btnMoney->setFocusPolicy(Qt::NoFocus);

    btnMobile=new QToolButton(this);
    btnMobile->setObjectName("btnMobile");
    btnMobile->setText(tr("移动支付"));
    btnMobile->setAutoRaise(true);
    btnMobile->setFocusPolicy(Qt::NoFocus);

    btnCard=new QToolButton(this);
    btnCard->setObjectName("btnCard");
    btnCard->setText(tr("会员卡支付"));
    btnCard->setAutoRaise(true);
    btnCard->setFocusPolicy(Qt::NoFocus);

    vl=new QVBoxLayout(this);
    vl->addWidget(btnMoney);
    vl->addWidget(btnMobile);
    vl->addWidget(btnCard);
    vl->setMargin(0);
    this->setLayout(vl);
    this->show();

    QObject::connect(btnMoney, SIGNAL(clicked(bool)),this,SLOT(btnMoney_slot()));
    QObject::connect(btnMobile,SIGNAL(clicked(bool)),this,SLOT(btnMobile_slot()));
    QObject::connect(btnCard,  SIGNAL(clicked(bool)),this,SLOT(btnCard_slot()));

    seleteMobilePayment=NULL;
    seleteMoneyPayment=NULL;
    seleteCardPayment=NULL;

    paymentResult=new PaymentResult(this);

    httpRequest=NULL;
    sql=NULL;
    serialPutOut=NULL;
}

