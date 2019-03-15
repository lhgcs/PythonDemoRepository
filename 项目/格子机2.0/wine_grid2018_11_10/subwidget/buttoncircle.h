#ifndef BUTTONCIRCLE_H
#define BUTTONCIRCLE_H

#include <QWidget>
#include <QPainter>
#include <QFont>
#include <QEvent>

class ButtonCircle : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonCircle(QWidget *parent = 0);
    int id;
    QString txt;
    void set_size(int size){
        this->setFixedSize(size,size);
    }
    void set_uncheck(){
        isPress=false;
        this->update();
    }
    void set_check(){
        isPress=true;
        this->update();
    }

signals:
    void click(int num);
protected:
    void paintEvent(QPaintEvent *){

        int w=this->width();
        int h=this->height();

        QPainter painter;
        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing, true);// 反走样

        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255,255,255,150));// p.setBrush(Qt::Dense4Pattern);//Dense1Pattern  RadialGradientPattern
        painter.fillRect(0,0,w,h,QBrush(QColor(255,255,255,0)));
        QFont font=painter.font();
        font.setPixelSize(40);//改变字体大小
        painter.setFont(font);

        QColor color(239,61,87,200);

        if(isPress == true){
            painter.setBrush(QBrush(color));
            painter.drawEllipse(QPoint(w/2,h/2),w/2-10,h/2-10);

            painter.setPen(Qt::NoPen);
            QRect rect(0,0,w,w);
            painter.setBrush(QBrush(QColor(0,0,0,0)));
            painter.drawRect(rect);

            painter.setPen(QPen(QColor(255,255,255)));
            painter.drawText(rect,Qt::AlignCenter,txt);
        }else{
            painter.setPen(QPen());

            //参数为：画刷，线宽，画笔风格，画笔端点，画笔连接风格
            QPen pen(color, 5, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
            painter.setPen(pen);

            QRect rect(0+20,0+20,w-40,w-40);
            int startAngle = 0 * 16; //值为，实际角度 * 16
            int spanAngle = 360 * 16;
            //三个参数：rect表示弧线所在的矩形，startAngle起始角度，spanAngle跨越角度
            painter.drawArc(rect, startAngle, spanAngle);

            painter.setPen(QPen(color));
            painter.drawText(rect,Qt::AlignCenter,txt);
        }
    }

    bool event(QEvent *e){
        if(e->type() == QEvent::MouseButtonPress){
            isPress=true;
            this->repaint();
            emit click(id);
        }
        return QWidget::event(e);
    }

private:
    bool isPress;
};

#endif // BUTTONCIRCLE_H
