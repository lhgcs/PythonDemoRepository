#include "pickupcode.h"

PickUpCode::PickUpCode(QWidget *parent) : QWidget(parent)
{
    this->hide();
    this->setWindowFlags(Qt::FramelessWindowHint);
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
    edit=new QLineEdit(this);
    edit->setPlaceholderText("请输入取货码");
    edit->setMaxLength(10);
    group=new QButtonGroup(this);
    button=new QPushButton[14];
    for (int i=0; i <14; i++){
        button[i].setMaximumSize(100,150);
        group->addButton(&button[i],i);
        if(i<10){
            button[i].setText(QString ::number(i));
        }
        button[i].setFocusPolicy(Qt::NoFocus);
    }
    QObject::connect(group,SIGNAL(buttonClicked(int)),this,SLOT(button_slot(int)));
    button[10].setText(".");
    button[11].setText("←");
    button[12].setText("清除");
    button[13].setText("取货");

    gl = new QGridLayout(this);
    gl->setHorizontalSpacing(5);//设置水平间距
    gl->setVerticalSpacing(5);  //设置垂直间距
    gl->setContentsMargins(0,0,0,0);

    gl->addWidget(edit, 0, 0, 1, 4);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            gl->addWidget(&button[i*3+j+1], i+1, j, 1, 1);
        }
    }

    gl->addWidget(&button[10], 4, 0, 1, 1);
    gl->addWidget(&button[0],  4, 1, 1, 1);
    gl->addWidget(&button[11], 4, 2, 1, 1);

    gl->addWidget(&button[12], 1, 3, 2, 1);
    gl->addWidget(&button[13], 3, 3, 2, 1);

    this->setLayout(gl);
}

/*

  温馨提示:
  请讲您所获取的取货码输入至相应区域,
  点击"取货"后取出商品,完成取货流程.

 */


