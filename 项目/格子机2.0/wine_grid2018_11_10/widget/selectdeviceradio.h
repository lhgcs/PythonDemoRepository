#ifndef SELECTDEVICERADIO_H
#define SELECTDEVICERADIO_H

#include <QWidget>
#include <QPushButton>
#include <QSignalMapper>
#include <QHBoxLayout>
#include "app.h"
#include <QTimer>
#include <QDebug>
#include <QButtonGroup>
#include <QRadioButton>

class SelectDeviceRadio : public QWidget
{
    Q_OBJECT
public:
    explicit SelectDeviceRadio(QWidget *parent = 0);
    ~SelectDeviceRadio(){
        delete []button;
        delete vl;
    }
    void init(){
        button[0].setChecked(true);
    }
    void set_visual(){
        for(int i=0;i<App::deviceNum;i++){
            if(App::deviceVisual[i]){
                button[i].setVisible(true);
            }
            else{
                button[i].setVisible(false);
            }
        }
    }

signals:
    void num(int layout,int value);
private:
    QRadioButton *button;
    QButtonGroup *buttonGroup;
    QVBoxLayout *vl;
    QTimer *timer;
    int selectButton;
private slots:
    void buttonJudge(int buttonId){
        timer->start();
        selectButton=buttonId;
    }
    void timer_slot(){
        if((selectButton>=0)&&(selectButton<App::deviceNum)){
            emit num(App::selectDeviceLayout,selectButton);
        }
    }
};

#endif // SELECTDEVICERADIO_H
