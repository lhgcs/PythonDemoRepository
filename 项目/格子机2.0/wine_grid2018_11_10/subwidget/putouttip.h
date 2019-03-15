#ifndef PUTOUTTIP_H
#define PUTOUTTIP_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QEventLoop>
#include "app.h"

#include <QStyleOption>
#include <QPainter>

class PutOutTip : public QWidget
{
    Q_OBJECT
public:
    explicit PutOutTip(QWidget *parent = 0);
    ~PutOutTip(){
        timer->stop();
        delete label;
        delete timer;
        delete vl;
    }

    void pay_ok(){
        label->setPixmap(QPixmap("/home/ico/pay_ok_green.png"));
        this->repaint();
//        QEventLoop eventloop;
//        QTimer::singleShot(1000, &eventloop, SLOT(quit()));
//        eventloop.exec();
    }
    void put_out_ing(){
        label->setText("正在送出，请耐心等待");
        timer->start(10000);
    }
    void put_out_ok(){
        label->setText("请取走商品");
        timer->start(3000);
    }
    void put_out_ok(QString str){
        label->setText(QString("请取走商品%1").arg(str));
        timer->start(3000);
    }
    void put_out_failed(){
        label->setText("商品送出失败，已支付金额将退回给你");
        timer->start(4000);
    }
    void put_out_failed2(){
        label->setText("商品送出失败，已消费积分将退还给你");
        timer->start(4000);
    }
signals:
    void num(int layout,int value);
private:
    QLabel *label;
    QTimer *timer;
    QVBoxLayout *vl;
private slots:
    void timer_slot(){
        this->close();
        emit num(App::putOutTipLayout,0);
    }
protected:
    void paintEvent(QPaintEvent *){
        QStyleOption  opt;
        opt.init(this);
        QPainter  p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // PUTOUTTIP_H
