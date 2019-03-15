#ifndef CHECKNETWORKSTATUS_H
#define CHECKNETWORKSTATUS_H

#include <QDebug>
#include <QThread>
#include "module_4g.h"
#include "shellutil.h"
#include "network/pingutil.h"

class CheckNetworkStatus : public QThread
{
public:
    CheckNetworkStatus();
    static int networkStatus;
    static int networkWay;
    enum{
        NETWORK_CONNECT_STATUS=0,
        NETWORK_DISCONNECT_STATUS=1
    };

    enum{
        NETWORK_WAY_ETH=0,
        NETWORK_WAY_4G=1
    };

protected:
    void run(){
        while (1) {
            qDebug()<<"check network ";
            if(ping((char *)"www.baidu.com",2,4) > 0){
                networkStatus=NETWORK_CONNECT_STATUS;
                qDebug()<<"ping connect to www.baidu.com ok";
                QThread::msleep(25000);
                continue;
            }else{
                networkStatus=NETWORK_DISCONNECT_STATUS;
                qDebug()<<"ping connect to www.baidu.com fail";
            }

            if(networkWay == NETWORK_WAY_ETH){
                qDebug()<<"eth0 connect to www.baidu.com";
                int res=ShellUtil::network_by_eth((char *)"eth0");
                if(res == 0){
                    networkStatus=NETWORK_CONNECT_STATUS;
                    qDebug()<<"eth0 connect to www.baidu.com ok";
                    QThread::msleep(3000);
                    continue;
                }else{
                    networkStatus=NETWORK_DISCONNECT_STATUS;
                     qDebug()<<"eth0 connect to www.baidu.com fail";
                }
            }
            if(networkWay == NETWORK_WAY_4G){
                int res=Module_4G::network_by_4g();
                if(res == 0){
                    networkStatus=NETWORK_CONNECT_STATUS;
                    qDebug()<<"ppp0 connect to www.baidu.com ok";
                }else{
                    networkStatus=NETWORK_DISCONNECT_STATUS;
                    qDebug()<<"ppp0 connect to www.baidu.com fail";
                }
                QThread::msleep(3000);
            }
        }
    }


};

#endif // CHECKNETWORKSTATUS_H
