#include "login.h"
#include "app.h"

Login::Login(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(App::DeskWidth,App::DeskHeight);
    this->move(0,0);
    this->setObjectName("Login");

    editUser=new QLineEdit(this);
    editPassWord=new QLineEdit(this);
    editUser->setMaximumSize(600,70);
    editPassWord->setMaximumSize(600,70);
    editUser->setObjectName("loginQLineEdit");
    editPassWord->setObjectName("loginQLineEdit");

    editUser->setMinimumSize(200,50);
    editPassWord->setMinimumSize(200,50);
    editPassWord->setEchoMode(QLineEdit::Password);

    label=new QLabel(this);
    label->setText(App::macId);
    pLayout = new QFormLayout();
    pLayout->addRow(QStringLiteral("机台号："), label);
    pLayout->addRow(QStringLiteral("用户名："), editUser);
    pLayout->addRow(QStringLiteral("密码："), editPassWord);
    pLayout->setSpacing(5);
    pLayout->setContentsMargins(120,50,120,50);

    keyBoard=new VirtualKeyboard(this);
    keyBoard->setMaximumSize(App::DeskWidth,App::DeskHeight);
    keyBoard->setMinimumSize(300,300);
    vLayout=new QVBoxLayout(this);
    vLayout->addStretch(1);
    vLayout->addLayout(pLayout);
    vLayout->addWidget(keyBoard);
    vLayout->addStretch(1);
    vLayout->setContentsMargins(20,20,20,20);
    setLayout(vLayout);

    editUser->setFocus();
    QObject::connect(keyBoard,SIGNAL(send_data(int)),this,SLOT(keyBoard_slot(int)));
}
