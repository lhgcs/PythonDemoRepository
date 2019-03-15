#include "paramterwidget.h"
#include "configure/app.h"
#include <QIntValidator>

ParamterWidget::ParamterWidget(int w,int h,QWidget *parent) : BaseSubWidget(parent)
{
    this->setObjectName("ParamterWidget");
    this->setFixedSize(w,h);

    labelTitle = new QLabel(this);
    labelTitle->setText("升降平台参数设置");
    labelTitle->setObjectName("labelTitle");

    testInput = new LabelAndEdit(this);
    testInput->set_text("Y轴步进",0);

    btnTest=new QPushButton("测试",this);
    btnReset=new QPushButton("复位",this);
    btnTest->setMaximumHeight(60);
    btnReset->setMaximumHeight(60);
    btnTest->setFixedWidth(App::screenWidget/4.0);
    btnReset->setFixedWidth(App::screenWidget/4.0);
    btnTest->setFocusPolicy(Qt::NoFocus);
    btnReset->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnTest,SIGNAL(clicked(bool)),this,SLOT(btnTest_slot()));
    QObject::connect(btnReset,SIGNAL(clicked(bool)),this,SLOT(btnReset_slot()));

    btnSaveY=new QPushButton("保存",this);
    btnSaveSpeed=new QPushButton("保存",this);
    btnSaveY->setMaximumHeight(100);
    btnSaveSpeed->setMaximumHeight(100);
    btnSaveY->setFixedWidth(App::screenWidget/4.0);
    btnSaveSpeed->setFixedWidth(App::screenWidget/4.0);
    btnSaveY->setFocusPolicy(Qt::NoFocus);
    btnSaveSpeed->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnSaveY,SIGNAL(clicked(bool)),this,SLOT(btnSaveY_slot()));
    QObject::connect(btnSaveSpeed,SIGNAL(clicked(bool)),this,SLOT(btnSaveSpeed_slot()));

    testgl = new QGridLayout;
    testgl->setMargin(0);
    testgl->addWidget(testInput,0,0,1,2,Qt::AlignLeft);
    testgl->addWidget(btnTest,1,0,1,1,Qt::AlignRight);
    testgl->addWidget(btnReset,1,1,1,1,Qt::AlignLeft);

    yInput = new LabelAndEdit[6];
    for(int i=0;i<6;i++){
        yInput[i].set_text("高度"+QString::number(i+1),0);
    }

    ygl = new QGridLayout();
    for(int i=0;i<3;i++){
        for(int j=0;j<2;j++){
            ygl->addWidget(&yInput[i*2+j],i,j,1,1);
        }
    }
    ygl->addWidget(btnSaveY,3,0,1,2,Qt::AlignCenter);

    QStringList list;
    list<<"复位速度"<<"最高速度"<<"加速度"<<"减速度"<<"细分数";
    speedInput = new LabelAndEdit[4];
    for(int i=0;i<4;i++){
        speedInput[i].set_text(list[i],0);
    }

    speed_gl = new QGridLayout();
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            speed_gl->addWidget(&speedInput[i*2+j],i,j,1,1);
        }
    }
    speed_gl->addWidget(btnSaveSpeed,2,0,1,2,Qt::AlignCenter);

    vl = new QVBoxLayout();
    vl->setMargin(0);
    vl->addStretch(1);
    vl->addWidget(labelTitle,0,Qt::AlignCenter);
    vl->addStretch(1);
    vl->addLayout(testgl);
    vl->addStretch(1);
    vl->addLayout(ygl);
    vl->addStretch(1);
    vl->addLayout(speed_gl);
    vl->addStretch(1);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->setFixedSize(w-40,h-20-180);
    stackedWidget->setAttribute(Qt::WA_TranslucentBackground, true);

    QWidget *widget = new QWidget(this);
    widget->setWindowOpacity(0.0);
    widget->setAttribute(Qt::WA_TranslucentBackground, true);
    widget->setFixedSize(stackedWidget->width(),stackedWidget->height());
    widget->setLayout(vl);

    settingTimeWidget = new SettingTimeWidget();

    stackedWidget->addWidget(widget);
    stackedWidget->addWidget(settingTimeWidget);

    key = new NumericalKeyboard(this);

    btnTime=new QPushButton("时间设置",this);
    btnTime->setMaximumHeight(60);
    btnTime->setFixedWidth(App::screenWidget/2.0);
    btnTime->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnTime,SIGNAL(clicked(bool)),this,SLOT(btnTime_slot()));

    btnBack=new QPushButton("返回",this);
    btnBack->setMaximumHeight(60);
    btnBack->setFixedWidth(App::screenWidget/2.0);
    btnBack->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnBack,SIGNAL(clicked(bool)),this,SLOT(btnBack_slot()));

    btnTime->setFixedWidth(App::screenWidget/3.0);
    btnBack->setFixedWidth(App::screenWidget/3.0);

    hl = new QHBoxLayout;
    hl->addWidget(btnTime);
    hl->addWidget(btnBack);

    v=new QVBoxLayout(this);
    v->setMargin(10);
    v->addWidget(stackedWidget,0,Qt::AlignCenter);
    v->addWidget(key);
    v->addLayout(hl);
    this->setLayout(vl);

    for(int i=0;i<6;i++){
        mapHeight[i]="0";
    }
    for(int i=0;i<5;i++){
        mapSpeed[i]="0";
    }
    get_data();
}

