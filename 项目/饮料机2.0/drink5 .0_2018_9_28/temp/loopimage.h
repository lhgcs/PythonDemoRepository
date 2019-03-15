#ifndef LOOPIMAGE_H
#define LOOPIMAGE_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QVector>
#include <QEvent>
#include "my_define.h"
#include <QPoint>
#include <QMouseEvent>

class LoopImage : public QWidget
{
    Q_OBJECT
public:
    explicit LoopImage(int status_arr[], QWidget *parent = 0);
//    void start()
//    {
//        timer->start(150);
//        this->show();
//    }
    void stop()
    {
        timer->stop();
    }

protected:
    void paintEvent (QPaintEvent *event)
    {
        QPainter painter(this);
        for(int i=0;i<4;i++)
        {
            if(!pix.at(i).isNull())
            {
                painter.drawPixmap(x0+IMG_W*(3-i),0,IMG_W,IMG_H,pix.at(i).scaled(IMG_W,IMG_H));//使用GPU处理  %vector_size
            }
        }
        x0+=5;
        if(x0>0)
        {
            x0=IMG_W*-1;
            vector.append(vector.at(0));
            vector.erase(vector.begin());
            load_pix();
        }
        paint_flag=true;
    }

protected:
    void mousePressEvent(QMouseEvent *event)
    {
        PressMouse = true;
        start_pos = event->pos().x();
    }
    void mouseReleaseEvent(QMouseEvent *event)
    {
        if(PressMouse)
        {
            int end_pos = event->pos().x();
            if(qAbs(end_pos-start_pos)>30)
            {
                if(end_pos<start_pos)
                {
                    move_image(false);
                }
                else if(end_pos>start_pos)
                {
                    move_image(true);
                }
            }
            else
            {
                int value=-1;
                if((start_pos>x0)&&(start_pos<x0+IMG_W))
                {
                    value=3;
                }
                else if((start_pos>x0+IMG_W)&&(start_pos<x0+IMG_W*2))
                {
                    value=2;
                }
                else if((start_pos>x0+IMG_W*2)&&(start_pos<x0+IMG_W*3))
                {
                    value=1;
                }
                else if((start_pos>x0+IMG_W*3)&&(start_pos<x0+IMG_W*4))
                {
                    value=0;
                }
                if(value>=0)
                {
                    emit num(vector.at(value).toInt());
                }
            }
        }
        PressMouse = false;
    }

signals:
    void num(int value);

private slots:
    void time_out_slot()
    {
//        if(paint_flag)
//        {
//            // update();
//             paint_flag=false;
//        }
    }

private:
    QTimer *timer;
    QVector<QString> vector;
    QVector<QPixmap> pix;
    int x0;
    int start_pos;
    bool PressMouse;
    const int *status_p;
    bool paint_flag;
    void load_pix();

    void move_image(bool right_direction)
    {
        timer->stop();
        if(right_direction)
        {
            for(int i=1;i<=51;i++)
            {
                x0+=20;
                if(x0>710)break;
                update();
            }
            for(int i=0;i<3;i++)
            {
                vector.append(vector.at(0));
                vector.erase(vector.begin());
            }
        }
        else
        {
            for(int i=1;i<=51;i++)
            {
                x0-=20;
                if(x0<-1000)break;
                update();
            }
            for(int i=0;i<3;i++)
            {
                QString str=vector.back();
                vector.pop_back();//移除最后一个元素
                vector.insert(vector.begin(),str);
            }
        }
        x0=1;
        timer->start(100);
    }
};

#endif // LOOPIMAGE_H
