#ifndef DOWNFILEPROGRESS_H
#define DOWNFILEPROGRESS_H

#include "basesubwidget.h"
#include "configure/app.h"
#include "model/httpdownfilethread.h"
#include <QTimer>

class DownFileProgress : public QWidget
{
    Q_OBJECT
public:
    explicit DownFileProgress(QWidget *parent = 0);

    int getTotal() const;
    void setTotal(int value);

    int getCurrent() const;
    void setCurrent(int value);

    void start_timer(){
        if(timer->isActive()){
            timer->stop();
        }
        current=0;
        len=-1;
        if(total>0){
            timer->start(1000);
        }else{
            init();
        }
    }
    void init(){
        total=0;
        current=0;
        len=-1;
        this->repaint();
    }

signals:
    void finished();
public slots:
    void timer_slot(){
        if(total >0){
            current=HttpDownFileThread::getDownThreadNum();
            if(current == 0){
                timer->stop();
                len=-1;
                total=0;
                emit finished();
            }else{
                len=(total-current)*1.0/total*App::screenWidget;
                this->repaint();
            }
        }else{
            total=0;
            current=0;
            len=-1;
            if(timer->isActive()){
                timer->stop();
            }
        }
    }
private:
    int total;
    int current;
    int len;
    QTimer *timer;

protected:
    void paintEvent(QPaintEvent *){
        QPainter painter(this);
        //painter.save();

        QRect rect = this->geometry();
        qDebug()<<current<<total<<len;
        painter.fillRect(QRect(QPoint(0,0),QSize(rect.width(),rect.height())),QBrush(QColor(255,0,0,0)));
        if(total > 0 && len >= 0 ){
            if(len < 15){
                painter.fillRect(QRect(QPoint(0,0),QSize(15,rect.height())),QBrush(QColor(0,255,0)));
            }else{
                painter.fillRect(QRect(QPoint(0,0),QSize(len,rect.height())),QBrush(QColor(0,255,0)));
            }
        }

//        if(total > 0 && current != total){
//            // 绘制区域为当前界面的整个区域（居中）
//            painter.drawText(rect, Qt::AlignCenter, QString::number(current)+"/"+QString::number(total));//QStringLiteral
//        }

        //painter.restore();
    }

};

#endif // DOWNFILEPROGRESS_H
