#ifndef GOODSDISPLAY_H
#define GOODSDISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QEvent>

#include "goodsinfo.h"

class GoodsDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit GoodsDisplay(QWidget *parent = 0);
    ~GoodsDisplay(){
        delete labelPicture;
        delete labelPresent;
        delete vl;
    }
    void setPicture(QString pic){
        labelPicture->setPixmap(QPixmap(pic).scaled(140,280));
    }
    void setPresent(QString name,QString price,QString remain){//text-align:right border:1px;
        QString str=QString("<table align=center > \
                             <tr><td align=right>酒名:  </td>  <td>%1</td></tr> \
                             <tr><td align=right>单价:  </td>  <td>%2元/500ml</td></tr> \
                             <tr><td align=right>剩余量:</td>  <td>%3ml</td></tr> \
                             </table>").arg(name,price,remain);//销售量:%4ml,sales
                //<tr><td align=right></td>        <td> </td></tr>
        labelPresent->setText(str);
    }

protected:
//    Qt中五种事件处理方式，控制权由低到高分别为:
//    1、重写事件处理函数
//    2、重写event()函数
//    3、为单个组件安装事件过滤器
//    4、为QApplication安装事件过滤器
//    5、重写QCoreApplication的成员notify()函数

//    bool event(QEvent *e){//被父窗口的过滤器截获
//        if(e->type()==QEvent::MouseButtonPress){
//            qDebug()<<"XXXXX";
//        }
//        return QWidget::event(e);
//    }

private:
    QLabel *labelPicture;
    QLabel *labelPresent;
    QVBoxLayout *vl;
};

#endif // GOODSDISPLAY_H

//style=\"width=\"100px\"; height=\"100px\";border=\"2px\"; text-align=\"right\";\"

//                             <tr><th>Header 1</th><th>Header 2</th></tr>
