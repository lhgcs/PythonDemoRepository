#ifndef DIALOG_H
#define DIALOG_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QStyleOption>
#include <QPainter>
#include <QTimer>

#include "app.h"

class Dialog : public QWidget
{
    Q_OBJECT
public:
    Dialog(QString str,int _ms=11000, QWidget *parent=0);
    Dialog(QWidget *parent=0);
    ~Dialog(){
        delete timer;
        delete label;
        delete button;
    }
    void show(){
        this->setVisible(true);
        this->raise();
        timer->start(ms);
    }
    void close(){
        timer->stop();
        this->hide();
    }
    void setText(QString str){
        if(str.isEmpty()){
            label->setText("<font color=red  size=3>请输入正确的商品编号</font>");
        }
        else{
            label->setText(str);
        }
    }
    void setText(QString str,int _ms){
        ms=_ms;
        label->setText(str);
        this->show();
    }
    void move_to_centre(){
        this->move((App::DeskWidth-400)/2,(App::DeskHeight-240)/2);
    }
    bool shake_flag;
protected:
    //如果要子类化一个QWidget，为了能够使用样式表，则需要提供paintEvent事件。
    //这是因为QWidget的paintEvent()是空的，而样式表要通过paint被绘制到窗口中。
    void paintEvent(QPaintEvent *){
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
    void closeEvent(QCloseEvent *){
        shake_flag=false;
    }
private:
    QLabel *label;
    QPushButton *button;
    QTimer *timer;

    int ms;
};

#endif // DIALOG_H
