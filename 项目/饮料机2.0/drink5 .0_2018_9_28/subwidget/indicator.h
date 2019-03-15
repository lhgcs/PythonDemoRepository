#ifndef INDICATOR_H
#define INDICATOR_H

#include <QWidget>
#include "basesubwidget.h"

#define INDICATOR_RADIUS 7

class Indicator : public BaseSubWidget
{
    Q_OBJECT
public:
    explicit Indicator(QWidget *parent = 0);
    void draw_indicator(const int _count,const int _index){
        this->count=_count;
        this->index=_index;
        this->update();
    }
private:
    int count;
    int index;

protected:
    //画图这类事情要放到paintevent函数里面去做
    void paintEvent(QPaintEvent *){
         QPainter paint(this);
         paint.setRenderHint(QPainter::Antialiasing, true);
         paint.setPen(QPen(QColor(239,61,87,100),1,Qt::SolidLine));      //设置画笔形式  Qt::SolidLine DashLine
         paint.setBrush(QBrush(QColor(239,61,87,100),Qt::SolidPattern)); //设置画刷形式

         if(count > 0){
             if(count == 1){
                 paint.drawEllipse((width()-INDICATOR_RADIUS)/2,0,INDICATOR_RADIUS,INDICATOR_RADIUS);
             }else{
                 int x=(width()-INDICATOR_RADIUS*(count*2-1))/2;
                 int x_select=0;
                 for(int i=0;i<count;i++){
                     if(i == index){
                         x_select=x;
                         x+=2*INDICATOR_RADIUS;
                         continue;
                     }
                     paint.drawEllipse(x,0,INDICATOR_RADIUS,INDICATOR_RADIUS);
                     x+=2*INDICATOR_RADIUS;
                 }
                 paint.setPen(QPen(QColor(239,61,87,200),1,Qt::SolidLine));
                 paint.setBrush(QBrush(QColor(239,61,87,200),Qt::SolidPattern));
                 paint.drawEllipse(x_select,0,INDICATOR_RADIUS,INDICATOR_RADIUS);
             }
         }
     }
};

#endif // INDICATOR_H
