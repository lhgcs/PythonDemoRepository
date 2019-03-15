#ifndef GOODSPRESENT_H
#define GOODSPRESENT_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QSignalMapper>
#include <QHBoxLayout>
#include "app.h"
#include <QStyleOption>
#include <QPainter>

class GoodsPresent : public QWidget
{
    Q_OBJECT
public:
    explicit GoodsPresent(QWidget *parent = 0);
    ~GoodsPresent(){
        delete []button;
        delete btnClose;
        delete labelPresent;
        delete signalMapper;
        delete hl1;
        delete vl;
    }

//colspan=2
    void set_present(QString img,QString name,QString price,QString remain,QString sark,QString box){//text-align:right border:1px; ,QString sales
        QString str=QString("<table align=center > \
                             <tr><td rowspan=\"5\" align=center padding=10px><img src=%1 height=300 width=212></td><td > </td><td > </td> </tr> \
                             <tr><td > </td><td align=right >商品: </td>  <td align=left >%2</td></tr> \
                             <tr><td > </td><td align=right >单价: </td>  <td align=left >%3</td></tr> \
                             <tr><td > </td><td align=right >位置: </td>  <td align=left >%4柜 %5格</td></tr> \
                             <tr><td > </td><td align=right >剩余量:</td> <td align=left >%6</td></tr> \
                             </table>").arg(img,name,price,sark,box,remain);
        labelPresent->setText(str);
    }
signals:
    void num(int layout,int value);
private:
    QLabel *labelPresent;
    QToolButton *button;
    QPushButton *btnClose;
    QSignalMapper *signalMapper;
    QHBoxLayout *hl1;
    QVBoxLayout *vl;
private slots:
    void keyPress(int value){
        if((value>=0)&&(value<3)){
            emit num(App::goodsPresentLayout,value);
        }
    }
    void btnClose_slot(){
        emit num(App::goodsPresentLayout,3);
    }
//protected:
//    void MyWidget::closeEvent(QCloseEvent *)//此函数在QWidget关闭时执行
//    {
//        emit num(App::goodsPresentLayout,3);
//    }

protected:
    void paintEvent(QPaintEvent *){
         QStyleOption opt;
         opt.init(this);
         QPainter p(this);
         style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
     }
};


#endif // GOODSPRESENT_H
