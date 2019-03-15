#ifndef BUTTONRADIOGROUP_H
#define BUTTONRADIOGROUP_H

#include <QWidget>
#include "buttoncircle.h"

class ButtonRadioGroup : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonRadioGroup(QWidget *parent = 0);
    void set_check(const int &num){
        if(num >= 0 && num < 5){
             buttonCircle[num].set_check();
        }
    }
    void set_visiable(int index,bool visiable){
        if(index >= 0 && index < 5){
            buttonCircle[index].setVisible(visiable);
        }
    }

signals:
    void click(int num);
public slots:
    void click_slot(int num){
        for(int i=0;i<5;i++){
            if(buttonCircle[i].id != num){
                buttonCircle[i].set_uncheck();
            }
        }
        emit click(num);
    }

private:
    ButtonCircle *buttonCircle;

};

#endif // BUTTONRADIOGROUP_H
