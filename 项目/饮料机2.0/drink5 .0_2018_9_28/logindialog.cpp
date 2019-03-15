#include "logindialog.h"
#include "configure/app.h"
#include "util/firmwaredownload.h"

LoginDialog::LoginDialog(int w,int h,QWidget *widget):BaseSubWidget(widget)
{
    this->setObjectName("Login");
    this->setFixedSize(w,h);

    editUser=new QLineEdit(this);
    editPassWord=new QLineEdit(this);
    editUser->setMinimumSize(App::screenWidget/2,30);
    editUser->setMaximumSize(App::screenWidget/2,50);
    editPassWord->setMinimumSize(App::screenWidget/2,30);
    editPassWord->setMaximumSize(App::screenWidget/2,50);
    editUser->setMaxLength(15);
    editPassWord->setMaxLength(15);
    editUser->setPlaceholderText("用户名");
    editPassWord->setPlaceholderText("密码");

    checkBoxUser=new QCheckBox(this);
    checkBoxPassWord=new QCheckBox(this);
    checkBoxUser->setChecked(false);
    checkBoxPassWord->setChecked(true);
    checkBoxUser->setFocusPolicy(Qt::NoFocus);
    checkBoxPassWord->setFocusPolicy(Qt::NoFocus);
    QObject::connect(checkBoxUser,    SIGNAL(toggled(bool)),this,SLOT(checkBoxUser_slot()));
    QObject::connect(checkBoxPassWord,SIGNAL(toggled(bool)),this,SLOT(checkBoxPassWord_slot()));

    checkBoxUser_slot();
    checkBoxPassWord_slot();

    labelTitle=new QLabel(this);
    labelTitle->setText("登录("+FirmWareDownload::mCurentVersion+")");
    labelTitle->setObjectName("labelTitle");

    hl1=new QHBoxLayout;
    hl1->addStretch();
    hl1->addWidget(editUser);
    hl1->addWidget(checkBoxUser);
    hl1->addStretch();

    hl2=new QHBoxLayout;
    hl2->addStretch();
    hl2->addWidget(editPassWord);
    hl2->addWidget(checkBoxPassWord);
    hl2->addStretch();

    keyBoard=new VirtualKeyboard(this);

    btnLogin=new QPushButton("登录",this);
    btnCancel=new QPushButton("取消",this);
    btnLogin->setFocusPolicy(Qt::NoFocus);
    btnCancel->setFocusPolicy(Qt::NoFocus);
    btnLogin->setMinimumSize(App::screenWidget/2+60,30);
    btnLogin->setMaximumSize(App::screenWidget/2+60,50);
    btnCancel->setMinimumSize(App::screenWidget/2+60,30);
    btnCancel->setMaximumSize(App::screenWidget/2+60,50);

    QObject::connect(btnLogin,SIGNAL(clicked(bool)),this,SLOT(btnLogin_slot()));
    QObject::connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel_slot()));

    vl=new QVBoxLayout(this);
    vl->setMargin(50);
    vl->addStretch();
    vl->addWidget(labelTitle,0,Qt::AlignCenter);
    vl->addStretch();
    vl->addLayout(hl1);
    vl->addLayout(hl2);
    vl->addWidget(btnLogin,0,Qt::AlignCenter);
    vl->addWidget(btnCancel,0,Qt::AlignCenter);
    vl->addStretch();
    vl->addWidget(keyBoard);
    this->setLayout(vl);
    this->show();
    QObject::connect(keyBoard,SIGNAL(send_data(int)),this,SLOT(keyBoard_slot(int)));
}

