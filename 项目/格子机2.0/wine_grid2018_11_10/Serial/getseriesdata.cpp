#include "getseriesdata.h"

#define TEMP_BUFF_LEN 255

GetSeriesData::GetSeriesData(char *tty){
    Serial serial;
    this->fd=serial.InitSerialPort(tty);
    qDebug()<<QLatin1String(tty)<<fd;
    time=600;
}

void GetSeriesData::run()
{
    if(this->fd<=0)return;
    char temp_buff[TEMP_BUFF_LEN];
    memset(temp_buff,0,TEMP_BUFF_LEN);

    int read_len=0;
    qDebug()<<"/dev/ttyS3 read start";
    while(1){
        QThread::msleep(600);
        read_len=read(this->fd,temp_buff,TEMP_BUFF_LEN);
        if(read_len>0){
            qDebug()<<"11111111";
            printf("read Len=%d\nread=",read_len);
            for(int i=0;i<read_len;i++){
                printf("%02x ",temp_buff[i]);
            }
            printf("\n");
            QString str=QString(QLatin1String(temp_buff));
            if(str.endsWith("\r\n")){
                str=str.remove("\r\n");
            }
            qDebug()<<str;
            emit data(str);
            memset(temp_buff,0,TEMP_BUFF_LEN);
        }
    }
}
