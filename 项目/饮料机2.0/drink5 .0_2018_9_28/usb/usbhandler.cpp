#include "usbhandler.h"

#include "stdio.h"

UsbHandler::UsbHandler()
{
    fd = open ("/dev/usbdev1.6", O_RDWR | O_NOCTTY);  //O_NOCTTY 表示不能把本串口当成控制终端  O_NONBLOCK非阻塞方式
    if (fd < 0){
      printf ("Can't open usb\n");
    }
    else{
        printf ("open usb\n");
    }
}

void UsbHandler::run(){
    if(fd>0){
        char temp_buff[10];
        memset(temp_buff,0,10);
        while (1) {
            QThread::msleep(1000);
            if(read(this->fd,temp_buff,10)>0)
            {
                printf ("xxxxxxx\n");
                printf("id:%s",temp_buff);
//                    QString str=QString(QLatin1String(temp_buff));
                QByteArray temp=temp_buff;
                 QString str=temp;
                    emit data(str);
                    memset(temp_buff,0,10);
            }
        }
    }
}
