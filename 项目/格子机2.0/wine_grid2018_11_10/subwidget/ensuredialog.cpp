#include "ensuredialog.h"

EnsureDialog::EnsureDialog(QWidget *parent):QWidget(parent)
{
    //this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    this->setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口
    //this->setAttribute(Qt::WA_DeleteOnClose,true);
    // 两种方式都行：
    this->setAttribute(Qt::WA_ShowModal, true);//属性设置
    //   setWindowModality(Qt::WindowModal);//设置阻塞类型

    this->resize(400,200);
    move_to_centre();
    label=new QLabel(this);
    label->setFixedSize(360,120);
    label->setAlignment(Qt::AlignCenter);
    //label->setText("确定用积分兑换商品吗？");
    btnSure=new QPushButton("确定",this);
    btnCancel=new QPushButton("取消",this);

    btnSure->setFixedSize(this->width()/2.0+1,50);
    btnCancel->setFixedSize(this->width()/2.0+1,50);
    QObject::connect(btnSure,SIGNAL(clicked(bool)),this,SLOT(btn_clicked()));
    QObject::connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(btn_clicked()));

    this->setObjectName("EnsureDialog");
    btnSure->setObjectName("btnSure");
    btnCancel->setObjectName("btnCancel");
    this->setStyleSheet(
        "#EnsureDialog{"
            "color:rgb(255,255,255);"
            "background:rgba(255,255,255,200);"
            "border-radius:12px;"
            "border:none;"
        "}"
        "QLabel{"
            "font:24px;"
            "color:rgb(0,0,0);"
            "background:rgba(0,0,0,0);"
            "border:none;"
        "}"
        "#btnSure{"
            "font:24px;"
            "color:rgb(255,255,255);"
            "background:rgb(1,137,249);"
            "border-bottom-right-radius:12px;"
            "border:none;"
        "}"
        "#btnCancel{"
            "font:24px;"
            "color:rgb(255,255,255);"
            "background:rgb(216,18,17);"
            "border-bottom-left-radius:12px;"
            "border:none;"
        "}"
        "QPushButton:hover{"
            "border:2px solid rgb(255,255,255);"
        "}"
        "QPushButton:pressed{"
            "border:2px solid rgb(255,255,255);"
            "padding-left:3px;"
            "padding-top:3px;"
        "}"
        );

    h=new QHBoxLayout;
    h->setMargin(0);
    h->addStretch(1);
    h->addWidget(btnCancel);
    h->addSpacing(0);//窗口部件之间间隔大小
    h->addWidget(btnSure);
    h->addStretch(1);

    v=new QVBoxLayout;
    v->setMargin(0);
    v->addStretch(2);
    v->addWidget(label);
    v->addStretch(1);
    v->addLayout(h);
    setLayout(v);

    timer=new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(close()));

    res=-1;
    this->show();
}
