#include "watchdog.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/watchdog.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <QFile>

WatchDog::WatchDog()
{
    if(QFile::exists("/dev/watchdog")){
        fd = open("/dev/watchdog",O_WRONLY);
        if(fd < 0){
           // perror("/dev/watchdog");//perror函数只是将errno代表的特定的错误报告给用户
            return;
        }
        int timeout = 5;
        int retval = ioctl(fd, WDIOC_SETOPTIONS, WDIOS_ENABLECARD);     //使能看门狗 设置看门狗状态,开启(WDIOS_ENABLECARD)或关闭(WDIOS_DISABLECARD)
        int retval2 = ioctl(fd, WDIOC_SETTIMEOUT,&timeout);             //设置超时时间
        if ((retval == -1)||(retval2 == -1)) {
           // perror("WDIOC_SETTIMEOUT ioctl");
            fd=-1;
        }
    }
    else{
        fd=-1;
    }
}

void WatchDog::run()
{
    if(fd<0){
        return;
    }
    printf("feed wdt \n");
    while(1){
        if (ioctl(fd, WDIOC_KEEPALIVE) == -1) {//喂狗
            perror("WDIOC_KEEPALIVE ioctl");
            exit(errno);
        }
        QThread::sleep(4);
    }
    close(fd);
}


