#ifndef SERIALPUTOUT_H
#define SERIALPUTOUT_H

#include <QThread>
#include <QDebug>

#include "serial.h"

#define TRY              0X01
#define OPEN             0X02
#define PUT_OUT_STOP     0X03
#define OPEN_BANKNOTE    0X04
#define BANKNOTE_OPEN    0x01
#define BANKNOTE_CLOSE   0x00
#define GET_MONEY        0X05

#define DOOR             0x08
#define DOOR_OPEN        0x01
#define DOOR_CLOSE       0x00

#define PC_CMD_BACK      0x07
#define PLC_SEND         0x01

//#define PUT_OUT_WEIGHT   0X09

#define DIRECT_OPEN   0X02

class SerialPutOut: public QThread
{
    Q_OBJECT
public:
    SerialPutOut();
    void run();
    void try_connect(){
        Serial::send_cmd(fd,TRY);
    }
    void open_box(int sark,int box){//出货
        box=get_real_box(box);
        Serial::send_open(fd,OPEN,sark,box);
    }
    void open_stop(){//停止出货
        Serial::send_cmd(fd,PUT_OUT_STOP);
    }
    void open_banknote(){//打开纸币器
        Serial::send_open(fd,OPEN_BANKNOTE,BANKNOTE_OPEN);
    }
    void close_banknote(){//关闭纸币器
        Serial::send_open(fd,OPEN_BANKNOTE,BANKNOTE_CLOSE);
    }

    void direct_open_box(int sark,int box){//出货
        box=get_real_box(box);
        Serial::send_open(fd,DIRECT_OPEN,sark,box);
    }

signals:
    void serial_data(int cmd,int result);
private:
    int fd;
    enum {bagHead,bagSign,bagLen,bagCmd,bagData};
    void data_processing(QVector<unsigned char> &data,int len);

    int get_real_box(int box){
//        switch (box) {
//            case  6:box=11;break;
//            case  7:box=12;break;
//            case  8:box=13;break;
//            case  9:box=14;break;
//            case 10:box=15;break;

//            case 11:box=21;break;
//            case 12:box=22;break;
//            case 13:box=23;break;

//            case 14:box=31;break;
//            case 15:box=32;break;
//            case 16:box=33;break;
//            case 17:box=34;break;
//            case 18:box=35;break;
//            default:break;
//        }
        return box;
    }
};

#endif // SERIALPUTOUT_H
