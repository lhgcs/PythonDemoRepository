#ifndef GOODSITEM_H
#define GOODSITEM_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QEvent>

class GoodsItem : public QWidget
{
    Q_OBJECT
public:
    explicit GoodsItem(QWidget *parent = 0);
    void set_fix_size(int width,int height){
        w=width;
        h=height;
        this->setFixedSize(w,h);
    }
    void set_content(const int &id,const QString &txt,const QString &pixPath){
        this->id=id;
        this->txt=txt;
        this->pix.load(pixPath);
        this->pix=pix.scaled(w,h-25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    void set_image(const QString &pixPath){
        this->pix.load(pixPath);
        this->pix=pix.scaled(w,h-25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        this->repaint();
    }
    void set_num(const int &_num){
        this->num=_num;
        this->repaint();
    }
    void set_price(const QString &_price){
        this->price="￥"+_price;
        this->repaint();
    }

    void set_num_price(const int &_num,const QString &_price){
        bool flag=false;
        if(num != _num){
            this->num=_num;
            flag=true;
        }
        if(price.compare(_price) != 0){
            if(_price.isEmpty()){
                this->price=_price;
            }else{
                this->price="￥"+_price;
            }
            flag=true;
        }
        if(flag){
            this->repaint();
        }
    }

    int get_id(){
        return id;
    }

    static int pressWidget;
signals:
    void clicked();
private:
    int w,h;
    int id;
    int num;
    QString txt;
    QPixmap pix;
    QString price;

protected:
    void paintEvent(QPaintEvent *){
        QPainter painter;
        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing, true);// 反走样

        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255,255,255,150));// p.setBrush(Qt::Dense4Pattern);//Dense1Pattern  RadialGradientPattern

        painter.fillRect(0,0,w,h,QBrush(QColor(255,255,255,0)));

        int img_w=width();
        if(!pix.isNull()){
            img_w=pix.width();
            int img_h=pix.height();
            painter.drawPixmap((w-img_w)/2,h-img_h-25,img_w,img_h,pix);
        }

//        painter.setPen(QPen(QColor(239,61,87,200)));
//        painter.setBrush(QBrush(QColor(239,61,87,200)));
//        painter.drawEllipse(QPoint(0,0),35,35);

        QFont font=painter.font();
        painter.setPen(QPen(QColor(255,255,255)));
//        if(txt != ""){
//            font.setPixelSize(20);//改变字体大小
//            painter.setFont(font);
//            painter.drawText(0,0,30,30,Qt::AlignCenter,txt);
//        }

        painter.setPen(Qt::NoPen);
        QRect rect((w-img_w)/2,h-25,img_w,25);
        painter.setBrush(QBrush(QColor(0,0,0,0)));
        painter.drawRect(rect);

        font.setPixelSize(25);//改变字体大小
        painter.setFont(font);
        painter.setPen(QPen(QColor(255,255,255)));//18,183,245
        painter.drawText(rect,Qt::AlignCenter,txt);
    }

    bool event(QEvent *e){
        if(e->type() == QEvent::MouseButtonPress){
            pressWidget=this->id;
            //emit clicked();
            //return true;
        }
        return QWidget::event(e);
    }
};

#endif // GOODSITEM_H
