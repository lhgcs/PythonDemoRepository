#include "keypress.h"
#include <QDebug>

KeyPress::KeyPress(const QString &device, QObject *parent): QThread(parent)
{
    QByteArray byte = device.toLatin1();//mdevice(device)
    fd=open(byte.data(),O_RDONLY);      //以只读方式打开文件
    if(fd<0){
        qDebug()<<"open device faile"<<device;
    }
}
//Type为EV_KEY时，code表示键盘code或者鼠标Button值
//Type为EV_KEY时，value: 0表示按键抬起。1表示按键按下。（4表示持续按下等？）。
void KeyPress::KeypadFilter()
{
    if(t.type==EV_KEY)//键盘或者按键，表示一个键码
    {
        if(t.code == SUPPLY_SHOW_BUTTON_KEY_VALUE){
            if((t.value == 0)){
                emit SupplyButtonPress(0XB);
                qDebug()<<"key pad show button press";
            }
        }
        else if(t.code == SUPPLY_HIDE_BUTTON_KEY_VALUE){
            if((t.value == 0)){
                emit SupplyButtonPress(0XC);
                qDebug()<<"key pad hide button press";
            }
        }
    }
}

void KeyPress::run()
{
    if(fd<0){
        return;
    }
    while(1){
        if(read(fd,&t,sizeof(t)) == sizeof(t)){
            KeypadFilter();
        }
        QThread::msleep(200);
    }
}

