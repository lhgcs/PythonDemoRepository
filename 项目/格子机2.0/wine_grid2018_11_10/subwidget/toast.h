#ifndef TOAST_H
#define TOAST_H

#include <QTimer>
#include <QLabel>
#include <QBasicTimer>
#include <QTimerEvent>
#include "app.h"
#include <QDebug>

class Toast : public QLabel
{
    Q_OBJECT
public:
    explicit Toast(QWidget *parent = 0);
    ~Toast(){
        time_stop();
    }
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
//    void set_text(const QString &str){
//        this->time_stop();
//        this->setText(str);
//        this->adjustSize();
//        this->move((App::DeskWidth-width())/2.0,App::DeskHeight*0.7);
//        this->raise();
//        this->show();
//        this->timer->start(2000);
//    }

    void set_text(const QString &str,unsigned int time_ms = 2000){
        this->time_stop();
        this->setText(str);
        this->adjustSize();
        this->move((App::DeskWidth-width())/2.0,App::DeskHeight*0.7);
        //this->raise();
        this->timer->start(time_ms);
        this->show();
    }

private:
    QTimer *timer;
    static Toast *pToast;
    void time_stop(){
//        if(timer.timerId() > 0){
//            if(timer.isActive()){
//                timer.stop();
//            }
//            killTimer(timer.timerId());
//        }

        if(timer->isActive()){
            timer->stop();
        }
        this->hide();
    }

//protected:
//    void timerEvent(QTimerEvent *event){
//        if (event->timerId()==timer.timerId()) {
//            this->close();
//        } else {
//            QWidget::timerEvent(event);
//        }
//    }

public slots:
    void timer_slot(){
        time_stop();
    }
};

#endif // TOAST_H
