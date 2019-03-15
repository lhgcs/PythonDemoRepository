#ifndef DIALOGINSTANCE_H
#define DIALOGINSTANCE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QStyleOption>
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>
#include "configure/app.h"
#include <QPropertyAnimation>

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
private:
    static DialogInstance * m_pInstance;
    QVBoxLayout *vl;
    void close(){
        timer->stop();
        this->hide();
    }
    void move_to_centre(){
        this->move((App::screenWidget-width())/2,(App::screenHeight-height())/2+150);
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

    void show_dialog(QString str,bool shake=false){
        move_to_centre();
        setText(str);
        if(shake){
            QPropertyAnimation *pAnimation = new QPropertyAnimation(this,"pos",this);
            pAnimation->setDuration(500);//设置动画的播放时长
            pAnimation->setLoopCount(2);
            //pAnimation->setStartValue(QPoint((WIDTH-dialog->width())/2,(HEIGHT-dialog->height())/2));
            pAnimation->setKeyValueAt(0,   QPoint(this->geometry().x() - 3, this->geometry().y() - 3));//设置每一时刻的位置，实现抖动效果
            pAnimation->setKeyValueAt(0.1, QPoint(this->geometry().x() + 6, this->geometry().y() + 6));
            pAnimation->setKeyValueAt(0.2, QPoint(this->geometry().x() - 6, this->geometry().y() + 6));
            pAnimation->setKeyValueAt(0.3, QPoint(this->geometry().x() + 6, this->geometry().y() - 6));
            pAnimation->setKeyValueAt(0.4, QPoint(this->geometry().x() - 6, this->geometry().y() - 6));
            pAnimation->setKeyValueAt(0.5, QPoint(this->geometry().x() + 6, this->geometry().y() + 6));
            pAnimation->setKeyValueAt(0.6, QPoint(this->geometry().x() - 6, this->geometry().y() + 6));
            pAnimation->setKeyValueAt(0.7, QPoint(this->geometry().x() + 6, this->geometry().y() - 6));
            pAnimation->setKeyValueAt(0.8, QPoint(this->geometry().x() - 6, this->geometry().y() - 6));
            pAnimation->setKeyValueAt(0.9, QPoint(this->geometry().x() + 6, this->geometry().y() + 6));
            pAnimation->setKeyValueAt(1,   QPoint(this->geometry().x() - 3, this->geometry().y() - 3));
            pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }

};

#endif // DIALOGINSTANCE_H
