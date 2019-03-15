#include "serialputout.h"

#define TEMP_BUFF_LEN 30
#include "app.h"

SerialPutOut::SerialPutOut(){
    char tty[20]="/dev/ttyS2";
    Serial serial;
    this->fd=serial.InitSerialPort(tty);
    qDebug()<<"put out  /dev/ttyS2 fd"<<fd;
}

void SerialPutOut::run()
{
    if(this->fd<=0)return;
    QVector<unsigned char> read_buff(0);
    QVector<unsigned char> read_data(0);
    unsigned char temp_buff[TEMP_BUFF_LEN];
    memset(temp_buff,0,TEMP_BUFF_LEN);

    int read_len=0;
    qDebug()<<"/dev/ttyS2 read start";
    while(1)
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
                    qDebug()<<"data ok";
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

void SerialPutOut::data_processing(QVector<unsigned char> &data,int len){
    if(len<bagData){
        return;
    }
    //ab 07 02 04 00 0b ba
   // ab 07 02 02 00 0b ba
    if(data.at(bagSign)==PC_CMD_BACK){//下位机应答包
        if(data.at(bagLen)==0x02)    {//数据长度2
            switch (data.at(bagCmd)) {//指令码
                case TRY:          emit serial_data(data.at(bagCmd),data.at(bagData));break;
                case OPEN:         emit serial_data(data.at(bagCmd),data.at(bagData));break;
                case OPEN_BANKNOTE:emit serial_data(data.at(bagCmd),data.at(bagData));break;
                case 0x00:         emit serial_data(data.at(bagCmd),0x00);            break;//误包
                default: break;
            }
        }
        else if(data.at(bagLen)==0x03){
            if(data.at(bagCmd)==OPEN){
//                emit serial_result(OPEN,data.at(5));
                emit serial_data(data.at(bagCmd),data.at(bagData+1));
            }
        }
    }
    else if(data.at(bagSign)==PLC_SEND){//下位机发送包
        if((data.at(bagLen)==0x02))    {//数据长度2
            switch (data.at(bagCmd))   {
                case DOOR:     emit serial_data(data.at(bagCmd),data.at(bagData));break;
                case GET_MONEY:emit serial_data(data.at(bagCmd),data.at(bagData));break;
                case 0x00:     emit serial_data(data.at(bagCmd),0x00);            break;
                default: break;
            }
        }
        else{
            //ab 01 08 03   00 00 00 02 04 04 00 16 ba
            if(data.at(bagCmd)==PUT_OUT_STOP)   {//重量
                float w=0;
                int w_len=data.at(bagLen);
                if(4+w_len-3<len){
                    for(int i=4;i<=4+w_len-3;i++){
                        w*=10;
                        w+=data.at(i);
                    }
                    if(w_len+2<len){
                        w+=data.at(w_len+2)*0.1;
                        //App::weight=w;
                        qDebug()<<"PUT_OUT_STOP weight"<<w;
                        emit serial_data(PUT_OUT_STOP,0x00);
                    }
                }
            }
        }

    }
}
