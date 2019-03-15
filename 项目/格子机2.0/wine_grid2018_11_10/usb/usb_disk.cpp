#include "usb_disk.h"
#include <QFile>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <QMutex>

USB_Disk::USB_Disk(){
    //初始化文件监听器
    fileWatcher = new QFileSystemWatcher();
    fileWatcher->addPath("/dev/input");
    connect(fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(slotFileChanged(QString)));
    //添加或删除一个文件
    bMouse = false;
}

void USB_Disk::slotFileChanged(const QString& path){
     static QMutex mutex;
     mutex.lock();
     if(path == "/dev/input"){
         if((!bMouse)&&(QFile::exists("/dev/input/event5")||QFile::exists("/dev/input/event6"))){

         }
         else{
              bMouse = false;
         }
     }
     mutex.unlock();
}
