#include "settingwidget.h"
#include <QVBoxLayout>
#include "configure/app.h"

SettingWidget::SettingWidget(QWidget *parent) : BaseSubWidget(parent)
{
    this->setFixedSize(App::screenWidget,App::screenHeight*2/3);
    this->setObjectName("SettingWidget");
    QString str0[] = {"默认",""};

    labelTitle = new QLabel(this);
    labelTitle->setText("系统参数设置");
    labelTitle->setObjectName("labelTitle");

    label=new QLabel(this);
    label->setText("设置机台号");

    groupMacId = new QButtonGroup(this);
    for(int i=0;i<2;i++){
        radioMacId[i] = new QRadioButton(str0[i],this);
        radioMacId[i]->setFocusPolicy(Qt::NoFocus);
        groupMacId->addButton(radioMacId[i],i);
    }

    groupMacId->button(0)->setChecked(true);
    QObject::connect(groupMacId, SIGNAL(buttonClicked(int)), this, SLOT(radioMacId_slot(int)));

    lineEditCustomMacId=new QLineEdit(this);
    lineEditCustomMacId->setPlaceholderText(QStringLiteral("自定义机台好"));
    lineEditCustomMacId->setMaxLength(15);

    QHBoxLayout *hl=new QHBoxLayout();
    hl->setContentsMargins(0,0,0,0);
    hl->addWidget(radioMacId[1]);
    hl->addWidget(lineEditCustomMacId);

    labelGoodsNum=new QLabel(this);
    labelGoodsNum->setText("设置商品数量");
    lineEditGoodsNum=new QLineEdit(this);
    lineEditGoodsNum->setPlaceholderText(QStringLiteral("自定义商品数量(1~200)"));
    lineEditGoodsNum->setMaxLength(6);
    lineEditGoodsNum->setValidator(new QIntValidator(1, 200, this));

    labelPay =new QLabel(this);
    labelPay->setText("设置支付方式");
    checkAlipay=new QCheckBox(this);
    checkWeixin=new QCheckBox(this);
    checkAlipay->setText("支付宝");
    checkWeixin->setText("微信");
    checkAlipay->setFocusPolicy(Qt::NoFocus);
    checkWeixin->setFocusPolicy(Qt::NoFocus);

    keyBoard=new VirtualKeyboard(this);
    QObject::connect(keyBoard,SIGNAL(send_data(int)),this,SLOT(keyBoard_slot(int)));

    btnClose=new QPushButton("退出");
    btnClose->setObjectName("settingBtnClose");
    btnClose->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnClose,SIGNAL(clicked(bool)),this,SLOT(btnClose_slot()));

    btnSave=new QPushButton("保存");
    btnSave->setObjectName("settingBtnClose");
    btnSave->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnSave,SIGNAL(clicked(bool)),this,SLOT(btnSave_slot()));

    btnFactory=new QPushButton("恢复出厂设置");
    btnFactory->setObjectName("settingBtnClose");
    btnFactory->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnFactory,SIGNAL(clicked(bool)),this,SLOT(btnFactory_slot()));

    QGridLayout *gl=new QGridLayout();
    gl->setMargin(0);
    gl->setHorizontalSpacing(20);

    gl->addWidget(labelGoodsNum,   0,0,1,1);
    gl->addWidget(lineEditGoodsNum,0,1,1,2);

    gl->addWidget(label,        1,0,1,1);
    gl->addWidget(radioMacId[0],1,1,1,1,Qt::AlignLeft);
    gl->addLayout(hl,           1,2,1,1);

    gl->addWidget(labelPay,     2,0,1,1);
    gl->addWidget(checkAlipay,  2,1,1,1,Qt::AlignLeft);
    gl->addWidget(checkWeixin,  2,2,1,1,Qt::AlignLeft);

    QHBoxLayout *hl2=new QHBoxLayout();
    hl2->setMargin(0);
    hl2->addWidget(btnFactory);
    hl2->addWidget(btnSave);
    hl2->addWidget(btnClose);

    QVBoxLayout *vLayout=new QVBoxLayout(this);
    vLayout->setMargin(50);
    vLayout->addStretch(1);
    vLayout->addWidget(labelTitle,0,Qt::AlignHCenter);
    vLayout->addStretch(3);
    vLayout->addLayout(gl);
    vLayout->addStretch(1);
    vLayout->addWidget(keyBoard,0,Qt::AlignHCenter);
    vLayout->addStretch(1);
    vLayout->addLayout(hl2);
    vLayout->addStretch(4);

    this->setLayout(vLayout);
    this->show();
}
