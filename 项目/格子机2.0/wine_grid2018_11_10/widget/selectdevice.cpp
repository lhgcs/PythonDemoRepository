#include "selectdevice.h"

SelectDevice::SelectDevice(QWidget *parent) : QWidget(parent)
{
//    this->setFocusPolicy(Qt::NoFocus);
    this->setFixedSize(150,500);

    signalMapper=new QSignalMapper(this);
    button=new QPushButton[App::deviceNum];
    for(int i=0;i<App::deviceNum;i++){
//        button[i].setFocusPolicy(Qt::NoFocus);
//        button[i].setIconSize(QSize(180,180));
//        QString img=QString("/home/ico/%1.png").arg(QString(65+i));
//        button[i].setIcon(QIcon(img));
        button[i].setObjectName("btn"+QString(65));
        button[i].setText(QString(65+i)+"柜");
        button[i].setMaximumSize(QSize(150,100));
        button[i].setMinimumSize(QSize(100,50));
        signalMapper->setMapping(&button[i],i);
        QObject::connect(&button[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
    }
    QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(keyPress(int)));

    vl=new QVBoxLayout(this);
    vl->setMargin(0);

    for(int i=0;i<App::deviceNum;i++){
        vl->addStretch(1);
        vl->addWidget(&button[i]);
    }
    vl->addStretch(1);

    this->setLayout(vl);
}

