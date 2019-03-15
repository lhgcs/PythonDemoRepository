#include "numkeyboard.h"
#include <QSignalMapper>
#include <QBoxLayout>
//#include "model/httprequest.h"
#include <QMessageBox>
#include "configure/app.h"

NumKeyboard::NumKeyboard(QWidget *parent):BaseSubWidget (parent)
{
    this->setObjectName("NumKeyboard");
    this->setFixedSize(App::screenWidget*2/3-80,App::screenHeight/3-80);

//    "QPushButton{background-color:rgb(0,163,229); color:rgb(255,255,255); border:1px solid rgb(111,156,207);"
//    "border-style:outset;border-radius: 5px;font-size:40px;}"
//    "QPushButton:pressed{ border-style: inset; }"
//    "QLineEdit{background-color:   rgb(255,255,255);  color: rgb(0,0,0);border:1px solid rgb(0,163,229);font-size:40px;}"//rgb(50,133,255)

//    QPalette pal(this->palette());
//    pal.setColor(QPalette::Background, QColor(232,241,252)); //设置背景黑色 Qt::black232,241,252
//    this->setAutoFillBackground(true);
//    this->setPalette(pal);

    int w=this->width()/4;
    int h=this->height()/4;

    int maxW=w*0.8;
    int maxH=h*0.8;

    int minW=w*0.55;
    int minH=h*0.55;

    QSignalMapper *signalMapper = new QSignalMapper(this);
    lineEdit=new QLineEdit(this);
    lineEdit->setPlaceholderText(QStringLiteral("请输入商品编号"));
    lineEdit->setMinimumHeight(minH);
    lineEdit->setMaximumHeight(maxH);
    lineEdit->setMaxLength(6);
    lineEdit->setAlignment(Qt::AlignCenter);
    button=new QToolButton[12];
    for (int i = 0; i <12; i++){
        button[i].setToolButtonStyle(Qt::ToolButtonTextOnly);//只显示文本
        button[i].setText(QString ::number(i));
        signalMapper->setMapping(&button[i], i);
        QObject::connect(&button[i], SIGNAL(pressed()), signalMapper, SLOT(map()));
        button[i].setMinimumSize(minW,minH);
        button[i].setMaximumSize(maxW,maxH);
    }
    button[10].setText("←");
    button[11].setText("确认");//↩
    QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(keyPress(int)));

    QGridLayout *gl = new QGridLayout(this);
    gl->addWidget(lineEdit,   0, 0, 1, 4);//第0行，第0列开始，占1行3列
    gl->addWidget(&button[10],1, 3, 1, 1);
    gl->addWidget(&button[0], 2, 3, 1, 1);
    gl->addWidget(&button[11],3, 3, 1, 1);

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            gl->addWidget(&button[i*3+j+1], i+1, j, 1, 1);
        }
    }
    // 设置水平间距
    gl->setHorizontalSpacing(5);
    // 设置垂直间距
    gl->setVerticalSpacing(5);
    // 设置外间距
    gl->setContentsMargins(20, 20, 20, 20); //left, top, right, bottom
    setLayout(gl);
    this->show();
}

void NumKeyboard::keyPress(int i){
    //qDebug()<<i;
    switch (i){
        case 10:{
            QString str=this->lineEdit->text();
            if(!str.isEmpty()){
                this->lineEdit->setText(str.left(str.length()-1));//chop(1)
            }
        }
        break;
        case 11:{
            if(lineEdit->text().isEmpty()) {
                emit num(-1);
            }else{
                emit num(lineEdit->text().toInt());
            }
            this->lineEdit->clear();
        }
        break;
        default:{
            this->lineEdit->setText(this->lineEdit->text().append(i+48));
        }
        break;
    }
    //this->lineEdit->setFocus();
}
