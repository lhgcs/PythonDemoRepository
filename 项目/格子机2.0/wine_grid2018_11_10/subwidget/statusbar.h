#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QWidget>
#include <QLabel>
#include <QTimerEvent>
#include <QHBoxLayout>
#include <QDateTime>
#include "configure/app.h"
#include "basesubwidget.h"
#include <QTcpSocket>

class StatusBar : public BaseSubWidget
{
    Q_OBJECT
public:
    explicit StatusBar(QWidget *parent = 0);
    void start(){
        stop();
        timerId=startTimer(60000);
    }
    void stop(){
        if(timerId > 0){
            killTimer(timerId);
            timerId=-1;
        }
    }
    void set_temp(int value){
        if(value > 0 && value < 100){
            label_Temp->setText(QString::number(value)+"℃");
        }else{
            label_Temp->clear();
        }
    }
    void set_macId(QString macId){
         label_mac_id->setText(macId);
    }
    void set_network_status(QString pix){
         label_network->setPixmap(QPixmap(pix));
    }
    void set_time(){
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString str=time.toString("yyyy-MM-dd hh:mm");//设置系统时间显示格式
        if(!str.startsWith("1970")){
            label_time->setText(str);//:ss               //在标签上显示时间
        }
    }
    static bool get_network_status(){
        bool res=false;
        QTcpSocket tcpClient;
        tcpClient.abort();
        tcpClient.connectToHost("115.239.211.112",80);//百度IP
        for(int i=0;i<1;i++){
            if(tcpClient.waitForConnected(2000)){
                res=true;
                break;
            }
        }
        return res;
    }

private:
    int timerId;
    int cnt;

    QLabel *label_mac_id,*label_Temp,*label_time,*label_network;

protected:
    void timerEvent(QTimerEvent *event){
        if(event->timerId() == timerId){
            set_time();
        }
    }

};

#endif // STATUSBAR_H
