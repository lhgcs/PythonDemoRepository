#ifndef SIGNALLIGHT_H
#define SIGNALLIGHT_H

#include <QWidget>
#include <QPainter>

class SignalLight : public QWidget
{
    Q_OBJECT
public:
    explicit SignalLight(QWidget *parent = 0);
    void set_index(int _index){
        index=_index;
        this->update();
    }
protected:
    void paintEvent(QPaintEvent *even){
         QWidget::paintEvent(even);
         QPainter paint(this);
         paint.setPen(QPen(Qt::NoPen));
         //paint.setPen(QPen(QColor(0,163,229,50),1,Qt::SolidLine));      //设置画笔形式  Qt::SolidLine DashLine
         paint.setBrush(QBrush(QColor(255,255,255,200),Qt::SolidPattern)); //设置画刷形式
         paint.setRenderHint(QPainter::Antialiasing, true);
         int r=10;
         int space=10;
         for(int i=0;i<2;i++){
             paint.drawEllipse(2,(r+space)*i,r,r);
         }
         paint.setBrush(QBrush(QColor(0,163,229,250),Qt::SolidPattern));
         paint.drawEllipse(0,(r+space)*index,r+4,r+4);
     }
private:
    int index;
};

#endif // SIGNALLIGHT_H
