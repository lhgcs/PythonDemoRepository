#include "numinputdialog.h"

NumInputDialog::NumInputDialog(QWidget *parent):QDialog(parent)
{
    this->hide();
    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setFixedSize(400,300);//360,240
    this->setStyleSheet(
        "QPushButton{"
            "font-size:40px;"
            "color:rgb(255,255,255); "
            "background-color:rgb(0,163,229); "
            "border:1px solid rgb(111,156,207);"
            "border-style:outset;"
        "}"
        "QPushButton:pressed{ "
            "border-style: inset; "
        "}"
    );
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(232,241,252)); //设置背景黑色
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    signalMapper = new QSignalMapper(this);
    button=new QPushButton[10];
    for (int i=0; i <10; i++){
        button[i].setText(QString ::number(i));
        signalMapper->setMapping(&button[i], i);
        QObject::connect(&button[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        button[i].setMaximumSize(90,80);
    }
    buttonBackspace= new QPushButton("退格",this);
    buttonEnter= new QPushButton("确认",this);
    signalMapper->setMapping(buttonBackspace, 10);
    signalMapper->setMapping(buttonEnter, 11);
    QObject::connect(buttonBackspace, SIGNAL(clicked()), signalMapper, SLOT(map()));
    QObject::connect(buttonEnter,     SIGNAL(clicked()), signalMapper, SLOT(map()));
    QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(keyPress(int)));
    buttonBackspace->setMaximumSize(90,80);
    buttonEnter->setMaximumSize(90,80);

    gl = new QGridLayout(this);
    gl->addWidget(buttonBackspace,0, 3, 1, 1);
    gl->addWidget(&button[0], 1, 3, 1, 1);
    gl->addWidget(buttonEnter,2, 3, 1, 1);

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            gl->addWidget(&button[i*3+j+1], i, j, 1, 1);
        }
    }
    gl->setHorizontalSpacing(5);//设置水平间距
    gl->setVerticalSpacing(5);  //设置垂直间距
    gl->setContentsMargins(20, 10, 20, 10);
    this->setLayout(gl);
}
