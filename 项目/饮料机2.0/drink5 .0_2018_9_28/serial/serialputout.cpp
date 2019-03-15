#include "serialputout.h"

#define TEMP_BUFF_LEN 30

#include "subwidget/dialoginstance.h"
#include "configure/toast.h"

SerialPutOut *SerialPutOut::mSerialPutOut=NULL;

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
    qDebug()<<"read start";
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

            while(!read_buff.isEmpty()){
                int result=1;
                #if JIUHAOCANG
                     result=Serial::data_prser(read_buff,read_data);//提取有效数据
                #else
                     result=Serial::data_prser(read_buff,read_data);//提取有效数据
                #endif

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
                }
                else if(-4==result){
                    printf("data end error\n");
                }
                else if(-5==result){
                    printf("data len error\n");
                }else{
                    printf("data unknow error\n");
                }
            }
        }
    }
}

void SerialPutOut::data_processing(QVector<unsigned char> &data,int len){
    if(len<=bagData){
        return;
    }
    if(data.at(bagSign)==PC_CMD_BACK)      //下位机应答包
    {
        if(data.at(bagLen)==0x02)    {     //数据长度2
            switch (data.at(bagCmd)) {     //指令码
                case TRY:       emit serial_result(TRY,        data.at(bagData));break;
                case OPEN:      emit serial_result(OPEN,       data.at(bagData));break;
                case OPEN_RESET:emit serial_result(OPEN_RESET, data.at(bagData));break;
                case DOOR:      emit serial_result(DOOR,       data.at(bagData));break;
                case DS18B20:   emit serial_result(DS18B20,    data.at(bagData));break;
                case 0x00:      emit serial_result(0x00,0x00);                   break;//误包

                case SET_HEIGHT:
                case SET_SPEED:
                    if(data.at(bagData) == 0){
                        Toast::getInstance()->set_text("参数设置成功");
                        //DialogInstance::getInstance()->setText("参数设置成功");
                    }else{
                        Toast::getInstance()->set_text("参数设置失败");
                        //DialogInstance::getInstance()->setText("参数设置失败");
                    }
                    break;
                case MOTO_TEST:
                    if(data.at(bagData) == 0){
                        Toast::getInstance()->set_text("测试成功");
                        //DialogInstance::getInstance()->setText("测试成功");
                    }else{
                        Toast::getInstance()->set_text("测试失败");
                        //DialogInstance::getInstance()->setText("测试失败");
                    }
                    break;
                case MOTO_RESET:
                    if(data.at(bagData) == 0){
                        Toast::getInstance()->set_text("复位成功");
                        //DialogInstance::getInstance()->setText("复位成功");
                    }else{
                        Toast::getInstance()->set_text("复位失败");
                        //DialogInstance::getInstance()->setText("复位失败");
                    }
                    break;
                default: break;
            }
        }
    }
    else if(data.at(bagSign)==PLC_SEND)     //下位机发送包
    {
        if((data.at(bagLen)==0x02))  {      //数据长度2
            switch (data.at(bagCmd)) {
                case DOOR:emit serial_result(DOOR,data.at(bagData));break;
                case 0x00:emit serial_result(0x00,0x00);            break;
                default: break;
            }
        }
    }
}
