#ifndef CONNECTTONETWORK_H
#define CONNECTTONETWORK_H

#include "network/module_4g.h"
#include "subwidget/dialoginstance.h"

#include "QDebug"
#include "QObject"
#include "QThread"

#include "network/pingutil.h"
#include "shellutil.h"
#include "checknetworkstatus.h"

#include "model/load.h"
#include "configure/toast.h"

class ConnectToNetwork : public QThread
{
    Q_OBJECT
public:
    ConnectToNetwork();
signals:
    void finish(bool value);
public:
    void run(){
        connect_to_network();
        emit finish(true);
        //get_data();
    }

    /**
      下载数据
     * @brief get_data
     */
    void get_data(){
        Load load;
        while(1){
            if(load.loading()){
                qDebug()<<"loading ok";
                emit finish(true);  //正常启动后关闭窗口
                return;
            }
        }
    }

    /**
      链接网络
     * @brief connect_to_network
     */
    void connect_to_network(){
        while(1){
            Toast::getInstance()->set_text("正在尝试使用以太网");
            //DialogInstance::getInstance()->setText("正在尝试使用以太网");
            QThread::sleep(2);

            int res=ShellUtil::network_by_eth((char *)"eth0");
            if(res == 0){
                CheckNetworkStatus::networkStatus=CheckNetworkStatus::NETWORK_CONNECT_STATUS;
                CheckNetworkStatus::networkWay=CheckNetworkStatus::NETWORK_WAY_ETH;
                return;
            }else{
                CheckNetworkStatus::networkStatus=CheckNetworkStatus::NETWORK_DISCONNECT_STATUS;
            }

            //DialogInstance::getInstance()->setText("正在尝试使用4G网络");
            Toast::getInstance()->set_text("正在尝试使用4G网络");
            res=Module_4G::network_by_4g();
            if(res == 0){
                CheckNetworkStatus::networkStatus=CheckNetworkStatus::NETWORK_CONNECT_STATUS;
                CheckNetworkStatus::networkWay=CheckNetworkStatus::NETWORK_WAY_4G;
                return;
            }else{
                CheckNetworkStatus::networkStatus=CheckNetworkStatus::NETWORK_DISCONNECT_STATUS;
            }
        }
    }

};

#endif // CONNECTTONETWORK_H
