#ifndef ENSUREDIALOG_H
#define ENSUREDIALOG_H

#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QTimer>
#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include "app.h"

class EnsureDialog : public QWidget
{
    Q_OBJECT
public:
    EnsureDialog(QWidget *parent = 0);
    ~EnsureDialog(){
        delete timer;
        delete label;
        delete btnSure;
        delete btnCancel;
        delete h;
        delete v;
    }

    void show(){
        this->setVisible(true);
        this->raise();
        timer->start(10000);
    }

    void setText(QString str){
        label->setText(str);
    }

    void move_to_centre(){
        this->move((App::DeskWidth-this->width())/2,(App::DeskHeight-this->height())/2);
    }

protected:
    void closeEvent(QCloseEvent *){
        if(timer->isActive()){
            timer->stop();
        }
        //emit upload(res);
    }
    void paintEvent(QPaintEvent *){
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

signals:
    void upload(int value);

private slots:
    void btn_clicked(){
        QObject *pObj = sender();
        if(pObj == btnSure){
            res=0;
        }
        else if(pObj == btnCancel){
            res=1;
        }
        emit upload(res);
        this->close();
    }

private:
    int res;
    QTimer *timer;
    QLabel *label;
    QPushButton *btnSure,*btnCancel;
    QHBoxLayout *h;
    QVBoxLayout *v;
};

#endif // ENSUREDIALOG_H
