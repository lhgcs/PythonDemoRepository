#include "selectdeviceradio.h"

SelectDeviceRadio::SelectDeviceRadio(QWidget *parent) : QWidget(parent)
{
//    this->setFocusPolicy(Qt::NoFocus);
    this->setFixedSize(150,500);

    buttonGroup = new QButtonGroup;
    button=new QRadioButton[App::deviceNum];
    for(int i=0;i<App::deviceNum;i++){
        button[i].setFocusPolicy(Qt::NoFocus);
        button[i].setText(QString(65+i)+"柜");
        button[i].setMaximumSize(QSize(150,100));
        button[i].setMinimumSize(QSize(100,50));
        buttonGroup->addButton(&button[i],i);
    }
    QObject::connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));

    timer=new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(300);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));

    vl=new QVBoxLayout(this);
    vl->setMargin(0);

    vl->addStretch();
    for(int i=0;i<App::deviceNum;i++){
        vl->addWidget(&button[i]);
    }
    vl->addStretch();

    this->setLayout(vl);
}

