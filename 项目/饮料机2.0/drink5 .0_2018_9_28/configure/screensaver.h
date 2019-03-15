#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPixmap>
#include <QEvent>
#include <QDebug>
#include <QMovie>
#include "globalapplication.h"

class ScreenSaver : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenSaver(QWidget *parent = 0);

private:
    QLabel *labelSpx;
    QMovie *movie;
    QTimer *timerSpx;
    void set_gif(QString gif){
        movie=new QMovie(gif);
        movie->setBackgroundColor(Qt::black);
        movie->setScaledSize(QSize(720,1280));
        movie->setSpeed(70);
        labelSpx->setMovie(movie);
        movie->start();
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event){//事件过滤器
         if(obj==labelSpx){
            if(event->type()==QEvent::MouseButtonPress) {
                //serialSound->set_sound(soundButton);
                //usleep(200000);
                movie->stop();
                this->hide();
                timerSpx->start();
                //serialSound->set_sound(soundPay);
                return true;
            }
         }
         return QWidget::eventFilter(obj,event);
     }
private slots:
    void timerSpx_timeout(){
        if(--GlobalApplication::closeSec<0){
            timerSpx->stop();
            movie->start();
            this->showFullScreen();
        }
    }
};

#endif // SCREENSAVER_H
