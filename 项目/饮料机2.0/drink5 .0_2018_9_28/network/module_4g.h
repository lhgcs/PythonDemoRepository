#ifndef MODULE_4G_H
#define MODULE_4G_H

#include <QString>

#include <fcntl.h>		/* low-level i/o */
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "subwidget/dialoginstance.h"

class Module_4G
{
public:
    Module_4G();
    static QString imeiStr;
    enum {
        MODULE_4G_NO_IMEI=1      ,
        MODULE_4G_NO_SIM_CARD    ,
        MODULE_4G_FAILED_REGISTER,
        MODULE_4G_FAILED_PPPD    ,
        MODULE_4G_FAILED_DNS     ,
        MODULE_4G_FAILED_PING
    };
    int module_4G_open(const char *const device);
    int module_4G_check_status(const int fd);
    int cmd_get_IMEI(int fd);
    void module_4G_close(int fd){
        if(fd >0){
            close(fd);
        }
    }

    int check_network_connect(char *device);
    int check_network_connect2(char *device);

    static int network_by_4g(int cnt=5){
        Module_4G module;
        char *device=NULL;
        #if YUGA
            device=(char *)"/dev/ttyUSB1";//ttyUSB0 --> modem   ttyUSB1 --> AT
        #endif
        #if QUECT
            device=(char *)"/dev/ttyUSB2";
        #endif

        int fd=module.module_4G_open(device);
        //DialogInstance::getInstance()->setText("正在尝试使用4G网络");
        if(fd > 0){
            system("/usr/bin/killall udhcpc");
            for(int i=0;i<cnt;i++){
                int res=module.module_4G_check_status(fd);
                switch (res) {
                    case 0:return 0;break;
                    case Module_4G:: MODULE_4G_NO_SIM_CARD:
                        DialogInstance::getInstance()->setText("请插入SIM卡");
                        break;
                    case Module_4G:: MODULE_4G_FAILED_REGISTER:
                        DialogInstance::getInstance()->setText("请检查天线或者充值话费");
                        break;
                    case Module_4G:: MODULE_4G_FAILED_PPPD:
                        DialogInstance::getInstance()->setText("请检查天线或者充值话费");
                        break;
                    case Module_4G:: MODULE_4G_FAILED_DNS:
                        DialogInstance::getInstance()->setText("获取DNS失败");
                        break;
                    case Module_4G:: MODULE_4G_FAILED_PING:
                        DialogInstance::getInstance()->setText("连接网络失败");
                        break;
                    default:break;
                }
            }
            module.module_4G_close(fd);
        }
        return 1;
    }

    static QString get_moudule_4G_id(){
        QString imei=NULL;
        Module_4G module;
        char *device=NULL;
        #if YUGA
            device=(char *)"/dev/ttyUSB1";
        #endif
        #if QUECT
            device=(char *)"/dev/ttyUSB2";
        #endif
        int fd=module.module_4G_open(device);
        if(fd > 0){
            for(int i=0;i<20;i++){
                if(module.cmd_get_IMEI(fd) == 0){
                    imei=imeiStr;
                    break;
                }
            }
            module.module_4G_close(fd);
        }
        return imei;
    }
};



#endif // MODULE_4G_H
