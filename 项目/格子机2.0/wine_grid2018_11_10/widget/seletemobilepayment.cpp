#include "seletemobilepayment.h"
#include "QAction"

SeleteMobilePayment::SeleteMobilePayment(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(parent->width(),parent->height());
    this->setObjectName("SeletePayment");

    btnClose=new QPushButton(this);
    btnClose->setObjectName("btnClose");
    btnClose->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnClose,SIGNAL(clicked(bool)),this,SLOT(btnClose_slot()));

    lineEdit=new QLineEdit(this);
    lineEdit->setPlaceholderText(QStringLiteral("请输入购买数量(ML)"));
    lineEdit->setMaxLength(5);
    lineEdit->setMaximumSize(parent->width(),80);
    lineEdit->setMinimumSize(parent->width()/1.2,70);

    //添加图标的代码：
    pTrailingAction = new QAction(this);
    pTrailingAction->setPriority(QAction::LowPriority);

//    QPixmap pixmap("/home/ico/clear.png");
//    pixmap=pixmap.scaled(QSize(80,80));
//    pTrailingAction->setIcon(pixmap);

    pTrailingAction->setIcon(QIcon("/home/ico/clear.png"));
    lineEdit->addAction(pTrailingAction, QLineEdit::TrailingPosition);
    // 连接信号和槽
    connect(pTrailingAction, SIGNAL(triggered(bool)), this, SLOT(clear_slot(bool)));

    btnWeixin=new QToolButton(this);
    btnWeixin->setObjectName("btnWeixin");
    btnWeixin->setText(tr("微信支付"));
    btnWeixin->setIcon(QPixmap("/home/ico/weixin.png"));
    btnWeixin->setIconSize(QSize(110,110));
    btnWeixin->setAutoRaise(true);
//    btnWeixin->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnWeixin->setFocusPolicy(Qt::NoFocus);

    btnAlipay=new QToolButton(this);
    btnAlipay->setObjectName("btnAlipay");
    btnAlipay->setText(tr("支付宝支付"));
    btnAlipay->setIcon(QPixmap("/home/ico/alipay.png"));
    btnAlipay->setIconSize(QSize(110,110));
    btnAlipay->setAutoRaise(true);
//    btnAlipay->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnAlipay->setFocusPolicy(Qt::NoFocus);

    hl=new QHBoxLayout();
    hl->setContentsMargins(0,0,0,0);
    hl->addStretch(2);
    hl->addWidget(btnWeixin);
    hl->addStretch(1);
    hl->addWidget(btnAlipay);
    hl->addStretch(2);

    vl=new QVBoxLayout(this);
    vl->setContentsMargins(0,0,0,0);
    vl->addStretch(0);
    vl->addWidget(btnClose,0,Qt::AlignRight);
    vl->addStretch(1);
    vl->addWidget(lineEdit,0,Qt::AlignCenter);
    vl->addStretch(1);
    vl->addLayout(hl,Qt::AlignCenter);
    vl->addStretch(2);
    this->setLayout(vl);
    this->show();

    numInputDialog=new NumInputDialog(this);
    numInputDialog->move((parent->width()-numInputDialog->width())/2,lineEdit->y()+lineEdit->height()+100);
    QObject::connect(numInputDialog,SIGNAL(num(int)),this,SLOT(numInput_slot(int)));
    numInputDialog->show();

    QObject::connect(btnWeixin,SIGNAL(clicked(bool)),this,SLOT(btnWeixin_slot()));
    QObject::connect(btnAlipay,SIGNAL(clicked(bool)),this,SLOT(btnAlipay_slot()));

    lineEdit->installEventFilter(this);

    httpRequest=NULL;
    pay=NULL;
    sql=NULL;
}

