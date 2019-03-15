#include "usbwatcher.h"
#include <QFile>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <QMutex>

#include <stdio.h>
#include <fcntl.h>		/* low-level i/o */
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <termios.h>

UsbWatcher::UsbWatcher(QObject *parent) : QObject(parent)
{
    //初始化文件监听器
    fileWatcher = new QFileSystemWatcher();
    fileWatcher->addPath("/dev/input");
    connect(fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(slotFileChanged(QString)));
    //添加或删除一个文件
    bMouse = false;
    gun=new UsbScanerGun(this);
    fd=0;

    if((!bMouse)&&(QFile::exists("/dev/input/event5")||QFile::exists("/dev/input/event6"))){
        qDebug()<<"/dev/input/event6 exists";
       if(gun){
           if(!gun->runFlag){
               int fd=open("/dev/input/event5",O_RDONLY);
               if(fd>0){
                   gun->fd=fd;
                   gun->runFlag=true;
                   gun->start();
                   qDebug()<<"gun start";
               }
               else{
                   gun->runFlag=false;
               }
           }
       }
    }
}

void UsbWatcher::slotFileChanged(const QString& path){
     static QMutex mutex;
     mutex.lock();
     if(path == "/dev/input"){
         if((!bMouse)&&(QFile::exists("/dev/input/event5")||QFile::exists("/dev/input/event6"))){
             qDebug()<<"/dev/input/event6 exists";
            if(gun){
                if(!gun->runFlag){
                    int fd=open("/dev/input/event5",O_RDONLY);
                    if(fd>0){
                        gun->fd=fd;
                        gun->runFlag=true;
                        gun->start();
                        qDebug()<<"gun start";
                    }
                    else{
                        gun->runFlag=false;
                    }
                }
            }
         }
         else{
              bMouse = false;
              gun->runFlag=false;
              qDebug()<<"/dev/input/event6 not exists";
              if(fd>0){
                  close(fd);
                  fd=0;
              }
         }
     }
     mutex.unlock();
}

