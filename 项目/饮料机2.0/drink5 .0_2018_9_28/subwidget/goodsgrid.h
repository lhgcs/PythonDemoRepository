#ifndef GOODSGRID_H
#define GOODSGRID_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QSignalMapper>
#include "goodsitem.h"
#include "configure/app.h"
#include "view/basestackedwidget.h"
#include <QStackedLayout>
#include <QMouseEvent>
#include <QEvent>
#include "indicator.h"
#include "QStackedWidget"

class GoodsGrid : public QWidget
{
    Q_OBJECT
private:
    int itemCount;
    GoodsItem *btn;
    QSignalMapper *signalMapper;
    QStackedWidget *stackedLayout;
    QWidget *widget;
    Indicator *indicator;

public:
    explicit GoodsGrid(int _w,int _h,QWidget *parent = 0):QWidget(parent){
        this->setObjectName("GoodsGrid");
        #if JIUHAOCANG
        int rowCount=3;   //行数
        int columnCount=3;//列数
        #else
        int rowCount=2;   //行数
        int columnCount=5;//列数
        #endif
        int pageCount=0;  //页数
        itemCount=App::goodsTotal;

        pageCount=itemCount/(rowCount*columnCount);
        if(itemCount%(rowCount*columnCount) != 0){
            pageCount++;
        }
        this->setFixedSize(_w,_h);
        int w=this->width()-10-columnCount*10;
        int h=this->height()-50;
        int imgw=w/columnCount;
        int imgh=h/rowCount;

        btn=new GoodsItem[itemCount];
        stackedLayout=new QStackedWidget();
        stackedLayout->setFixedSize(_w,_h-20);
        signalMapper=new QSignalMapper(this);
        int k=0;
        widget=new QWidget[pageCount];
        for(int p=0;p<pageCount;p++){
            QGridLayout *gl=new QGridLayout();
            widget[p].setFixedSize(_w,_h-20);
           // widget[p].setAttribute(Qt::WA_TransparentForMouseEvents);//父窗口响应鼠标点击事件  只有按下
            gl->setContentsMargins(10,10,10,5);
            gl->setHorizontalSpacing(10);
            gl->setVerticalSpacing(10);
            for(int i=0;i<rowCount;i++){
                for(int j=0;j<columnCount;j++){
                    if(++k>itemCount){
                        break;
                    }else{
                        int index=i*columnCount+j+p*rowCount*columnCount;
                        gl->addWidget(&btn[index],i,j);
                        btn[index].set_fix_size(imgw,imgh);
                        btn[index].set_content(k,QString::number(k),"/home/image/"+QString::number(k));
                        connect(&btn[index], SIGNAL(clicked()), signalMapper, SLOT(map()));
                        signalMapper->setMapping(&btn[index],k);
                    }
                }
                if(k>itemCount){
                    break;
                }
            }
            widget[p].setLayout(gl);
            widget[p].installEventFilter(this); // 统一注册
            stackedLayout->addWidget(&widget[p]);
        }

        indicator=new Indicator(this);

        QVBoxLayout *vl=new QVBoxLayout(this);
        vl->setMargin(0);
        vl->addWidget(stackedLayout);
        vl->addWidget(indicator);
        setLayout(vl);
        connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(mapped(int)));
        stackedLayout->setCurrentIndex(0);
        indicator->draw_indicator(stackedLayout->count(),0);
    }

    void set_itemColumn(int item){
        itemCount=item;
    }

    void set_image(){
        for(int k=0;k<App::goodsTotal;k++){
            btn[k].set_image("/home/image/"+QString::number(k+1));
        }
    }

    void set_stock(const int &id,const int &num){
        if(id >= 0 && id < App::goodsTotal){
            btn[id].set_num(num);
        }
    }

    void set_stock_price(const int &id,int num,QString price){
        if(id >= 0 && id < App::goodsTotal){
            btn[id].set_num_price(num,price);
        }
    }

signals:
    void mapped(int value);
private slots:
    void set_press_widget_slot(int value){
        pressWidget=value;
    }

protected:
    bool PressMouse;
    int pressPosX;
    int pressPosY;
    int pressWidget;

//    bool eventFilter(QObject *target, QEvent *event)
//    {
//        //globalPos

//        if(event->type() == QMouseEvent::MouseButtonPress){
//            PressMouse=true;
//            start_pos_y=cursor().pos().y();
//             qDebug()<<"rel11111111e";
//        }

//    bool event(QEvent *event){
////        for(int i=0;i<5;i++){
////        if(target == &widget[i]){

//            if(event->type() == QMouseEvent::MouseButtonPress){
//                PressMouse=true;
//                start_pos_y=cursor().pos().y();
//                 qDebug()<<"rel11111111e";
//            }else if(event->type()==QMouseEvent::MouseButtonRelease){
//                qDebug()<<"relase";
//                if(PressMouse){
//                    qDebug()<<"relase2";
//                    int end_pos_y=cursor().pos().y();
//                    if(qAbs(end_pos_y-start_pos_y)>70){
//                        qDebug()<<"relase3";
//                        int index=stackedLayout->currentIndex();
//                        int count=stackedLayout->count()-1;
//                         if(end_pos_y>start_pos_y){
//                            if(--index<0)index=count;
//                         }
//                         else{
//                             if(++index>count)index=0;
//                         }
//                         stackedLayout->setCurrentIndex(index);
//                         return true;
//                    }else{
//                        qDebug()<<"relase4";
//                    }
//                    PressMouse = false;
//                }
//            }

//       // }
//     //   }
//        return QWidget::event(event);
//      //  return QWidget::eventFilter(target, event); // 发还给具体控件
//    }


    //鼠标按下事件处理函数
    void mousePressEvent(QMouseEvent *ev)//一定要点在没有控件的地方才会有效果
    {
        pressPosX = ev->x();
        pressPosY = ev->y();
    }
    //鼠标抬起事件处理函数
    void mouseReleaseEvent(QMouseEvent *ev){
        int releasePosX = ev->x();
        int releasePosY = ev->y();

        int lenX=qAbs(releasePosX-pressPosX);
        int lenY=qAbs(releasePosY-pressPosY);

        if(lenX>70){
            GoodsItem::pressWidget=-1;
            int index=stackedLayout->currentIndex();
            int count=stackedLayout->count()-1;
             if(releasePosX>pressPosX){
                if(--index<0)index=count;
             }
             else{
                 if(++index>count)index=0;
             }
             stackedLayout->setCurrentIndex(index);
             indicator->draw_indicator(stackedLayout->count(),index);
             return ;
        }
        if(lenY>70){
            GoodsItem::pressWidget=-1;
            int index=stackedLayout->currentIndex();
            int count=stackedLayout->count()-1;
             if(releasePosY>pressPosY){
                if(--index<0)index=count;
             }
             else{
                 if(++index>count)index=0;
             }
             stackedLayout->setCurrentIndex(index);
             indicator->draw_indicator(stackedLayout->count(),index);
             return ;
        }
        if(GoodsItem::pressWidget>0){
            emit mapped(GoodsItem::pressWidget);
            GoodsItem::pressWidget=-1;
        }
        QWidget::mouseReleaseEvent(ev);
    }
};

#endif // GOODSGRID_H
