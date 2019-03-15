#ifndef DIALOGINSTANCE_H
#define DIALOGINSTANCE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QStyleOption>
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>
#include "app.h"

class DialogInstance : public QWidget
{
    Q_OBJECT
public:
    explicit DialogInstance(QWidget *parent = 0);
    ~DialogInstance(){
        delete timer;
        delete label;
        delete button;
        delete vl;
    }
    static DialogInstance* getInstance()  {
         if( m_pInstance == NULL )
             m_pInstance = new DialogInstance();
         return m_pInstance;
    }
    static DialogInstance* getInstance(QString str,int _ms=5000,QWidget *parent =0)  {
         if( m_pInstance == NULL ){
             m_pInstance = new DialogInstance(parent);
         }
         m_pInstance->setText(str,_ms);
         return m_pInstance;
    }
    void setText(QString str,int _ms=5000){
        if(str != NULL){
            label->setText(str);
            this->setVisible(true);
            this->raise();
            if(_ms>0){
                timer->start(_ms);
            }
            else {
                timer->stop();
            }
        }
    }
    void close(){
        timer->stop();
        this->hide();
    }
private:
    static DialogInstance * m_pInstance;
    QVBoxLayout *vl;
    void move_to_centre(){
        this->move((App::DeskWidth-width())/2,(App::DeskHeight-height())/2);
    }
protected:
    //如果要子类化一个QWidget，为了能够使用样式表，则需要提供paintEvent事件。
    //这是因为QWidget的paintEvent()是空的，而样式表要通过paint被绘制到窗口中。
    void paintEvent(QPaintEvent *){
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
private:
    QLabel *label;
    QPushButton *button;
    QTimer *timer;
};

#endif // DIALOGINSTANCE_H
