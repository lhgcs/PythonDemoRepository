#ifndef TOAST_H
#define TOAST_H

#include <QTimer>
#include <QLabel>
#include "configure/app.h"
#include <QStyleOption>
#include <QPainter>

class Toast : public QLabel
{
    Q_OBJECT
public:
    static Toast *getInstance(QWidget *parent){
        if(pToast == NULL){
           pToast = new Toast(parent);
        }
        return pToast;
    }
    static Toast *getInstance(){
        if(pToast == NULL){
           pToast = new Toast();
        }
        return pToast;
    }
    void set_text(const QString &str){
        setText(str);
        this->adjustSize();
        this->move((App::screenWidget-width())/2,height);
        this->raise();
        this->show();
        timer->start(3000);
    }
private:
    explicit Toast(QWidget *parent = 0);
    ~Toast(){
        if(timer->isActive()){
            timer->stop();
        }
        delete timer;
    }
    static Toast *pToast;

private:
    QTimer *timer;
    int height;
//protected:
//    void paintEvent(QPaintEvent *){
//        QStyleOption opt;
//        opt.init(this);
//        QPainter p(this);
//        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//    }
};

#endif // TOAST_H
