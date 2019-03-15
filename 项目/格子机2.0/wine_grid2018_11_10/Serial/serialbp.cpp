#include "serialbp.h"

#define TEMP_BUFF_LEN 30

QString SerialBP::card="";

SerialBP::SerialBP()
{
    char tty[20]="/dev/ttyS3";
    Serial serial;
    this->fd=serial.InitSerialPort(tty);
    qDebug()<<"BP  /dev/ttyS3 fd"<<fd;
    flag=true;
}

void SerialBP::run()
{
    if(this->fd<=0)return;
    QVector<unsigned char> read_buff(0);
    QVector<unsigned char> read_data(0);
    unsigned char temp_buff[TEMP_BUFF_LEN];
    memset(temp_buff,0,TEMP_BUFF_LEN);

    int read_len=0;
    qDebug()<<"ttyS1 read start";
    while(flag)
    {
        QThread::msleep(600);
        read_len=read(this->fd,temp_buff,TEMP_BUFF_LEN);
        if(read_len>0)
        {
            if(read_buff.size()>200){
                read_buff.erase(read_buff.begin(),read_buff.end());
            }
            printf("read Len=%d\nread=",read_len);
            for(int i=0;i<read_len;i++){
                printf("%02x ",temp_buff[i]);
                read_buff.append(temp_buff[i]);
            }
            printf("\n");
            memset(temp_buff,0,TEMP_BUFF_LEN);

            while(!read_buff.isEmpty())
            {
                int result=Serial::data_prser(read_buff,read_data);//提取有效数据
                if(0==result){
                    data_processing(read_data,read_data.size());
                }
                else if(-1==result){
                    printf("no start\n");
                    break;
                }
                else if(-2==result){
                    printf("no end\n");
                    break;
                }
                else if(-3==result){
                    printf("check error\n");
                    break;
                }
            }
        }
    }
}

#include<math.h>
void SerialBP::data_processing(QVector<unsigned char> &data,int len){
    if((len<bagLen)||(len<data.at(bagLen)+bagLen+1)){
        return;
    }
    switch (data.at(bagCmd)) {
        case LOOK:{
            int dataLen=data.at(bagLen)-1;    //数据长度
            unsigned int value=0;
            for(int i=0;i<dataLen;i++){
                value+=data.at(i+bagData)*pow(10,i);
            }
            emit serial_result(LOOK,value);
            break;
        }
//        case REDUCE:{
//            if(data.at(bagLen)==2){
//                emit serial_result(REDUCE,data.at(bagData));
//            }
//            break;
//        }
        default:break;
    }
}
