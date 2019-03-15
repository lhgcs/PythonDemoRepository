#ifndef SELECTDEVICE_H
#define SELECTDEVICE_H

#include <QWidget>
#include <QPushButton>
#include <QSignalMapper>
#include <QHBoxLayout>
#include "app.h"

class SelectDevice : public QWidget
{
    Q_OBJECT
public:
    explicit SelectDevice(QWidget *parent = 0);
    ~SelectDevice(){
        delete []button;
        delete signalMapper;
        delete vl;
    }
signals:
    void num(int layout,int value);
private:
    QPushButton *button;
    QSignalMapper *signalMapper;
    QVBoxLayout *vl;
private slots:
    void keyPress(int value){
        if((value>=0)&&(value<App::deviceNum)){
            emit num(App::selectDeviceLayout,value);
        }
    }
};

#endif // SELECTDEVICE_H
