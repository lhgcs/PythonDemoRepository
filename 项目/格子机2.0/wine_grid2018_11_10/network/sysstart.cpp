#include "sysstart.h"
#include <QTimer>

#include "app.h"

SysStart::SysStart(QWidget *parent):QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//无边框,不可改变大小
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(50,50,50,255)); //设置背景黑色
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    resize(App::DeskWidth,App::DeskHeight);
    Progess = new LoadingpProgess(this);
    ExcetionInfo = new QLabel(this);
    layout = new QVBoxLayout(this);
    isLoading = false;

    ExcetionInfo->hide();
    ExcetionInfo->setAlignment(Qt::AlignCenter);
    this->setStyleSheet("QLabel{"
               "font:40px;"
               "color:rgb(0, 160, 230);"
               "border:none;"
           "}");

//    Progess->setLoadingItemInfo("hello");
//    Progess->setProgessRange(0,20);

    layout->addStretch(3);
    layout->addWidget(ExcetionInfo);
    layout->addStretch(3);
    layout->addWidget(Progess);
    layout->addStretch(1);

    layout->setContentsMargins(20,20,20,20);

    setLayout(layout);
    this->showFullScreen();





//    NetWork->setSystartHandel(mSystart);
//    if(NetWork->checkNetWork())
//    {
//        return ;
//    }


}

//void SysStart::timerEvent(QTimerEvent *)//定时器时间
//{
//    this->update();
//}


void SysStart::paintEvent(QPaintEvent *)
{
  if(isLoading)
  {
    static int rotate=0;
    QPainter painter(this);                               //this为绘图设备，即表明在该部件上进行绘制
    painter.setRenderHint(QPainter::Antialiasing, true);  //告诉绘图引擎应该在可能的情况下进行边的反锯齿绘制
    QRadialGradient gradient(640, 360, 180, 640, 360);    //(cx, cy) 坐标，半径，(fx, fy) 坐标
    gradient.setColorAt(0,   QColor::fromRgbF(1, 0, 0, 1));
    gradient.setColorAt(0.2, QColor::fromRgbF(1, 0, 1, 0.8));
    gradient.setColorAt(0.4, QColor::fromRgbF(1, 1, 0, 0.6));
    gradient.setColorAt(0.6, QColor::fromRgbF(1, 1, 0, 0.4));
    gradient.setColorAt(0.8, QColor::fromRgbF(0, 1, 1, 0.2));
    gradient.setColorAt(1,   QColor::fromRgbF(0, 0, 1, 0));
    gradient.setSpread(QGradient::ReflectSpread );//设置显示模式
    painter.setPen(QPen(QBrush(gradient), 10));
    for (int i = 0; i <=3; ++i)
    {
        int temp=(i*90+rotate)*16;
        painter.drawArc(640-CIRCLE_R1-4,360-CIRCLE_R1-4,CIRCLE_R1*2,CIRCLE_R1*2,temp,960);//画圆弧
        painter.drawArc(640-CIRCLE_R2-4,360-CIRCLE_R2-4,CIRCLE_R2*2,CIRCLE_R2*2,(i*90-rotate)*16,960);
        painter.drawArc(640-CIRCLE_R3-4,360-CIRCLE_R3-4,CIRCLE_R3*2,CIRCLE_R3*2,temp,960);
    }
    QFont font=painter.font();
    font.setPixelSize(50);//改变字体大小
    painter.setFont(font);
    painter.setPen(QPen(QColor(255,255,255)));
    painter.translate(640,360); //将点设为原点
    painter.rotate(90);
    painter.setPen(QPen(QColor(255,255,0), 10));
    //painter.drawText(-50,-50,100,50,Qt::AlignHCenter|Qt::AlignBottom,"威玎");
   // painter.drawText(-50,0,100,50,Qt::AlignHCenter|Qt::AlignTop,"科技");
    if((rotate+=6)>=90)
    {
        rotate=0;
    }
  }
  else {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);;
  }
}

void SysStart::LodingExectionSlot(QString info)
{
//    killTimer(id);
    Progess->hide();
    ExcetionInfo->setText(info);
    ExcetionInfo->show();
    isLoading = false;
    this->update();
}


