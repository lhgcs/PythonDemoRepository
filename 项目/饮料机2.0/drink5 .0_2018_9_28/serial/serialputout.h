#ifndef SERIALPUTOUT_H
#define SERIALPUTOUT_H

#include <QThread>
#include <QDebug>
#include "serial/serial.h"
#include "configure/app.h"
#include "model/tableputouttimeinfo.h"
#include "configure/toast.h"

#define TRY              0X01   //尝试通讯
#define OPEN             0X02   //打开
#define OPEN_ALL         0x03   //全部打开
#define DS18B20          0X04   //温度传感器
#define AM2301           0x05   //
#define DOOR             0x08   //门
#define OPEN_RESET       0x09   //校正
#define SET_TEMP         0x0b   //设置温度

#define SET_SPEED        0x11   //设置升降速度
#define SET_HEIGHT       0x12   //设置升降高度
#define MOTO_TEST        0x13   //升降测试
#define MOTO_RESET       0x14   //升降复位

#define DOOR_OPEN        0x01   //
#define DOOR_CLOSE       0x00   //
#define PC_CMD_BACK      0x07   //
#define PLC_SEND         0x01   //

#define OPEN_SUCCESS     0X00
#define OPEN_MOTOR_FAIL  0X01
#define OPEN_ING         0X02
#define OPEN_FAIL        0X03

#include "model/tablegoodinfo.h"
#include "subwidget/dialoginstance.h"

class SerialPutOut: public QThread
{
    Q_OBJECT
private:
    SerialPutOut();
    static SerialPutOut *mSerialPutOut;
public:
    static SerialPutOut *getInstance(){
        if(mSerialPutOut == NULL){
            mSerialPutOut=new SerialPutOut;
        }
        return mSerialPutOut;
    }

    void close(){
        Serial::close_device(fd);
        delete mSerialPutOut;
        mSerialPutOut=NULL;
        fd=-1;
    }

    void run();

    void open_box(int box){
        int temp = box;
        box=get_real_box(box);
        qDebug()<<temp<<box;
        if(box <= 0){
            DialogInstance::getInstance()->setText("请设置电机参数");
            return;
        }
    #if JIUHAOCANG
        int time=TablePutOutTimeInfo::getInstence()->table_select_time
                (TablePutOutTimeInfo::getInstence()->getTableName(),temp);
        Serial::send_open(fd,OPEN,box,time/100);
    #else
        Serial::send_open(fd,OPEN,box);
    #endif
    }
   void open_reset(int box){
       int temp = box;
       box=get_real_box(box);
       if(box <= 0){
           DialogInstance::getInstance()->setText("请设置电机参数");
           return;
       }
   #if JIUHAOCANG
       int time=TablePutOutTimeInfo::getInstence()->table_select_time
               (TablePutOutTimeInfo::getInstence()->getTableName(),temp);
       Serial::send_open(fd,OPEN,box,time/100);
   #else
       Toast::getInstance()->set_text(QString("校正货道:%1 ---> %2")
                                      .arg(QString::number(temp),QString::number(box)));
       Serial::send_open(fd,OPEN_RESET,box);
   #endif
   }

   void open_reset(int box,QString str){
       int temp = box;
       box=get_real_box(box);
       if(box <= 0){
           DialogInstance::getInstance()->setText(str+"请设置电机参数");
           return;
       }
   #if JIUHAOCANG
       int time=TablePutOutTimeInfo::getInstence()->table_select_time
               (TablePutOutTimeInfo::getInstence()->getTableName(),temp);
       Serial::send_open(fd,OPEN,box,time/100);
   #else
       Toast::getInstance()->set_text(QString(str+"校正货道:%1 ---> %2")
                                      .arg(QString::number(temp),QString::number(box)));
       Serial::send_open(fd,OPEN_RESET,box);
   #endif
   }

   //"ab 01 02 1e 23 44 ba 00 "
   void send_temperature(unsigned char L,unsigned char H){
       Serial::send_open(fd,SET_TEMP,L,H);
   }
   void get_temp(){
       Serial::send_cmd(fd,DS18B20);
   }

   /*
    *设置升降高度
    */
   void set_height(int *height){
       unsigned char pack[30];
       memset(pack,0,sizeof(pack));
       pack[0]=0xAB;        //包头AB
       pack[1]=0x01;        //包标识
       pack[2]=24+1;        //数据长度
       pack[3]=SET_HEIGHT;  //指令

       int index=4;
       for(int i=0;i<6;i++){
           for(int j=0;j<4;j++){
               pack[index+i*4+j]=(unsigned char)(height[i]>>((3-j)*8));
           }
       }
       pack[28]=0;//校验和
       for(int i=1;i<28;i++){
           pack[28]+=pack[i];
       }
       pack[29]=0xBA;       //包尾BA
       Serial::send_pack(fd,pack,sizeof(pack));
   }
   /*
    * 设置升降参数
    */
   void set_speed(int *speed){
       unsigned char pack[12];
       memset(pack,0,sizeof(pack));
       pack[0]=0xAB;        //包头AB
       pack[1]=0x01;        //包标识
       pack[2]=7;           //数据长度
       pack[3]=SET_SPEED;   //指令

       pack[4]=speed[1]>>8;
       pack[5]=speed[1]>>0;
       pack[6]=speed[0]>>8;
       pack[7]=speed[0]>>0;

       pack[8]=speed[2];
       pack[9]=speed[3];

       pack[10]=0;//校验和
       for(int i=1;i<10;i++){
           pack[10]+=pack[i];
       }
       pack[11]=0xBA;        //包尾BA
       Serial::send_pack(fd,pack,sizeof(pack));
   }

   /*
    * 升降
    */
   void moto_test(long long height){
       unsigned char pack[10];
       memset(pack,0,sizeof(pack));
       pack[0]=0xAB;        //包头AB
       pack[1]=0x01;        //包标识
       pack[2]=5;           //数据长度
       pack[3]=MOTO_TEST;   //指令

       pack[4]=height>>24;
       pack[5]=height>>16;
       pack[6]=height>>8;
       pack[7]=height>>0;

       pack[8]=0;//校验和
       for(int i=1;i<8;i++){
           pack[8]+=pack[i];
       }
       pack[9]=0xBA;        //包尾BA
       Serial::send_pack(fd,pack,sizeof(pack));
   }

   /*
    * 复位
    */
   void moto_reset(){
       Serial::send_cmd(fd,MOTO_RESET);
   }


signals:
    void serial_result(int cmd,int result);
private:
    int fd;
    enum {bagHead,bagSign,bagLen,bagCmd,bagData};
    void data_processing(QVector<unsigned char> &data,int len);
public:
    int get_real_box(int box){
    #if SANXIN
        switch (box) {
            case 14:box=15;break;
            case 15:box=16;break;
            case 16:box=17;break;

            case 17:box=21;break;
            case 18:box=22;break;
            case 19:box=23;break;

            case 20:box=25;break;
            case 21:box=26;break;
            case 22:box=27;break;

            case 23:box=31;break;
            case 24:box=32;break;
            case 25:box=33;break;
            case 26:box=34;break;
            case 27:box=35;break;
            case 28:box=36;break;
            case 29:box=37;break;
            case 30:box=38;break;
            case 31:box=39;break;
            case 32:box=40;break;

            case 33:box=41;break;
            case 34:box=42;break;
            case 35:box=43;break;
            case 36:box=44;break;
            case 37:box=45;break;
            case 38:box=46;break;
            case 39:box=47;break;
            case 40:box=48;break;
            case 41:box=49;break;
            case 42:box=50;break;
            default:break;
        }
    #endif

    #if ZHONGXIN
        switch (box) {
            case 1:box=1;break;
            case 2:box=4;break;
            case 3:box=7;break;
            case 4:box=10;break;

            case 5:box=11;break;
            case 6:box=14;break;
            case 7:box=17;break;
            case 8:box=20;break;

            case 9:box=21;break;
            case 10:box=22;break;
            case 11:box=23;break;
            case 12:box=24;break;
            case 13:box=25;break;
            case 14:box=26;break;
            case 15:box=27;break;
            case 16:box=28;break;

            case 17:box=31;break;
            case 18:box=32;break;
            case 19:box=33;break;
            case 20:box=34;break;
            case 21:box=35;break;
            case 22:box=36;break;
            case 23:box=37;break;
            case 24:box=38;break;

            case 25:box=41;break;
            case 26:box=42;break;
            case 27:box=43;break;
            case 28:box=44;break;
            case 29:box=45;break;
            case 30:box=46;break;
            case 31:box=47;break;
            case 32:box=48;break;
            default:break;
        }
    #endif

    #if JIUHAOCANG
        switch (box) {
            case 7:box=11;break;
            case 8:box=12;break;
            case 9:box=13;break;
            case 10:box=14;break;
            case 11:box=15;break;
            case 12:box=16;break;

            case 13:box=21;break;
            case 14:box=22;break;
            case 15:box=23;break;
            case 16:box=24;break;
            case 17:box=25;break;
            case 18:box=26;break;

            case 19:box=31;break;
            case 20:box=32;break;
            case 21:box=33;break;
            case 22:box=34;break;
            case 23:box=35;break;
            case 24:box=36;break;

            case 25:box=41;break;
            case 26:box=42;break;
            case 27:box=43;break;
            case 28:box=44;break;
            case 29:box=45;break;
            case 30:box=46;break;

            case 31:box=51;break;
            case 32:box=52;break;
            case 33:box=53;break;
            case 34:box=54;break;
            case 35:box=55;break;
            case 36:box=56;break;
            default:break;
        }
        #endif


//#if JIUHAOCANG
//        switch (box) {

//            case 1:box=1;break;
//            case 2:box=3;break;
//            case 3:box=5;break;
//            case 4:box=7;break;
//            case 5:box=9;break;

//            case 6:box=11;break;
//            case 7:box=13;break;
//            case 8:box=15;break;
//            case 9:box=17;break;
//            case 10:box=19;break;

//            case 11:box=21;break;
//            case 12:box=23;break;
//            case 13:box=25;break;
//            case 14:box=27;break;
//            case 15:box=29;break;


//            case 16:box=31;break;
//            case 17:box=33;break;
//            case 18:box=35;break;

//            case 19:box=37;break;
//            case 20:box=39;break;

//            case 21:box=41;break;
//            case 22:box=43;break;
//            case 23:box=45;break;
//            case 24:box=47;break;

//            case 25:box=49;break;
//            case 26:box=51;break;
//            case 27:box=53;break;
//            case 28:box=55;break;
//            case 29:box=57;break;
//            case 30:box=59;break;

//        default:break;
//    }
//    #endif


        #if NEW_DRINK
        box = TableGoodInfo::getInstence()->table_select_motor(
                    TableGoodInfo::getInstence()->getTableName(),
                    box);

        #endif
        return box;
    }

};

#endif // SERIALPUTOUT_H
