#ifndef SHOWGOODS_H
#define SHOWGOODS_H

#include <QObject>
#include <QWidget>
#include <QToolButton>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>

#include "my_define.h"
#define ROUND_RADIUS 10
#define ROUND_Y      (226*2+10)
#define ICO_W 135
#define ICO_H 191

class ShowGoods : public QWidget
{
    Q_OBJECT
public:
    explicit ShowGoods(QWidget *parent = 0);
    void paintEvent(QPaintEvent *even)
    {
        QWidget::paintEvent(even);
        drawRound(index);
    }
//    void mousePressEvent(QMouseEvent *event)
//    {
//        PressMouse = true;
//        start_pos = event->pos().x();
//        if(event->type()==QMouseEvent::MouseMove)
//        {

//        }
//         qDebug()<<"鼠标移入";
//       // QWidget:: mousePressEvent(event);
//    }
//    void mouseReleaseEvent(QMouseEvent *event)
//    {
//        if(PressMouse)
//        {
//            int end_pos = event->pos().x();
//            if(qAbs(end_pos-start_pos)>30)
//            {
//                if(end_pos<start_pos)
//                {
//                   if(++index>5)index=0;
//                }
//                else if(end_pos>start_pos)
//                {
//                   if(--index<0)index=5;
//                }
//                change_info(index);
//            }
//            else
//            {
//               QWidget::mouseReleaseEvent(event);
//            }
//        }
//        PressMouse = false;
//    }

//    void Widget::enterEvent(QEvent *)
//    {
//        qDebug()<<"鼠标移入";
//    }
//    void Widget::leaveEvent(QEvent *)
//    {
//        qDebug()<<"鼠标移出";
//    }

//    void mouseMoveEvent(QMouseEvent *event)
//    {
//        if(event==QEvent::MouseTrackingChange)
////        if(end_pos<start_pos)
////        {
//        if(++index>5)index=0;
////        }
////        else if(end_pos>start_pos)
////        {
////        if(--index<0)index=5;
////        }
//        change_info(index);
//        update();
//    }

    bool eventFilter(QObject *obj, QEvent *event)//事件过滤器
     {
         if (event->type()==QMouseEvent::MouseButtonPress)
         {
             PressMouse = true;
//             start_pos_x=cursor().pos().x();
             start_pos_y=cursor().pos().y();
             timer->start(30000);
         }
         else if(event->type()==QMouseEvent::MouseButtonRelease)
         {
             if(PressMouse)
             {
                 int end_pos_y=cursor().pos().y();
                 if(qAbs(end_pos_y-start_pos_y)>70)
                 {
                      if(end_pos_y>start_pos_y)
                      {
                         if(--index<0)index=5;
                      }
                      else
                      {
                          if(++index>5)index=0;
                      }
                       change_info(index);
                       update();
                 }
                 else
                 {
                     for(int i=0;i<10;i++)
                     {
                         if((obj == &toolButton[i]))
                         {
                             emit num(index*10+i+1);
                             break;
                         }
                     }
                 }
                 PressMouse = false;
             }
//             else
//             {
//                 return QWidget::eventFilter(obj, event);
//             }
         }
         else
         {
             return QWidget::eventFilter(obj, event);
         }
     }

    void stop_timer()
    {
        timer->stop();
    }

signals:
    void num(int value);
private slots:
    void start_timer()
    {
        timer->start(30000);
    }
    void time_out_slot()
    {
        if(++index>5)index=0;
        change_info(index);
        update();
    }

private:
    QTimer *timer;
    int index;
    QToolButton *toolButton;
    bool PressMouse;
//    volatile int start_pos_x;
    volatile int start_pos_y;

    void change_info(int index)
    {
        int num=index*10;
        for(int i=0;i<10;i++)
        {
            QString img="/home/image/"+QString::number(num+i+1)+".png";
            toolButton[i].setIcon(QIcon(QPixmap(img).scaled(ICO_W,ICO_H)));
            toolButton[i].setText("货道:"+QString::number(num+i+1));
        }
    }

    void drawRound(int index)//画图这类事情要放到paintevent函数里面去做
    {
        QPainter paint(this);
        paint.setPen(QPen(Qt::NoPen));
        //paint.setPen(QPen(QColor(0,163,229,50),1,Qt::SolidLine));      //设置画笔形式  Qt::SolidLine DashLine
        paint.setBrush(QBrush(QColor(0,163,229,50),Qt::SolidPattern)); //设置画刷形式
        paint.setRenderHint(QPainter::Antialiasing, true);
        int x[6]={0};
//        x[2]=WIDTH/2.0-ROUND_RADIUS*3.0;//+ROUND_RADIUS/2.0;
//        x[1]=WIDTH/2.0-ROUND_RADIUS*3.0*2;//+ROUND_RADIUS/2.0;
//        x[0]=WIDTH/2.0-ROUND_RADIUS*3.0*3;//+ROUND_RADIUS/2.0;
//        x[3]=WIDTH/2.0-ROUND_RADIUS/2.0;
//        x[4]=WIDTH/2.0+ROUND_RADIUS*3.0*1;//-ROUND_RADIUS/2.0;
//        x[5]=WIDTH/2.0+ROUND_RADIUS*3.0*2;//-ROUND_RADIUS/2.0;

        x[2]=WIDTH/2.0-ROUND_RADIUS*3.0;
        x[3]=WIDTH/2.0-ROUND_RADIUS/2.0;
        int temp=x[3]-x[2];
        x[1]=x[2]-temp;
        x[0]=x[1]-temp;
        x[4]=x[3]+temp;
        x[5]=x[4]+temp;
        for(int i=0;i<6;i++)
        {
            paint.drawEllipse(x[i],ROUND_Y,ROUND_RADIUS,ROUND_RADIUS);
        }
        paint.setBrush(QBrush(QColor(0,163,229,250),Qt::SolidPattern));
        paint.drawEllipse(x[index],ROUND_Y,ROUND_RADIUS,ROUND_RADIUS);
    }
};

#endif // SHOWGOODS_H
