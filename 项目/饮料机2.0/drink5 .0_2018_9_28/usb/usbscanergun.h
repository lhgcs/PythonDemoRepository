#ifndef USBSCANERGUN_H
#define USBSCANERGUN_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include <stdio.h>

#include "Serial/serial.h"

#define TEMP_BUFF_LEN 1024

class UsbScanerGun : public QThread
{
    Q_OBJECT
public:
    UsbScanerGun(QObject *parent=0);
signals:
    void data(QString str);
public:
    int fd;
    bool runFlag;
    void run(){
        if((this->fd<=0))return;
        char temp_buff[TEMP_BUFF_LEN];
        memset(temp_buff,0,TEMP_BUFF_LEN);
        int read_len=0;
        qDebug()<<"read start";
        while(runFlag){

            QThread::msleep(1000);
            read_len=read(this->fd,temp_buff,TEMP_BUFF_LEN);
            if(read_len>0){
//                QString str(temp_buff);

                qDebug()<<"len:"<<read_len;

                QByteArray arr;

                for(int i=0;i<read_len;i++){
                    printf("%c",temp_buff[i]);
                    arr.append(temp_buff[i]);
                }

                printf("%s",temp_buff);

//                QString str = QString::fromLocal8Bit(temp_buff);

                QString strr = arr;

                QString str;
                str = QString("%1").arg(temp_buff);
                qDebug()<<str;
                //QString str=QString::fromUtf8(temp_buff);
                emit data(str);
//                emit data(QString(QLatin1String(temp_buff)));
                memset(temp_buff,0,TEMP_BUFF_LEN);
            }

        }
    }
};

#endif // USBSCANERGUN_H
