#ifndef SERIALBP_H
#define SERIALBP_H

#include <QThread>
#include <QDebug>
#include "serial.h"
#include "math.h"

#define LOOK 1
#define REDUCE 1

class SerialBP: public QThread
{
    Q_OBJECT
public:
    SerialBP();
    void run();
    void look(){
        Serial::send_cmd(fd,LOOK);/////////////////////
    }
    void close_serial(){
        close(fd);
    }
    int send_reduce_bp(uint goods_BP){
        if(fd>0){
            unsigned char str[7]={0};
            for(int i=0;i<7;i++){
                uint temp=pow(10,i);
                str[i]=goods_BP/temp%10;
            }

            unsigned char pack[6+7]={0};
            pack[0]=0xAB;                           //包头AB
            pack[1]=0x01;                           //包标识
            pack[2]=0x08;                           //数据长度
            pack[3]=REDUCE;                         //指令

            unsigned char temp=0;
            for(int i=0;i<7;i++){
                pack[4+i]=str[i];
                temp+=str[i];
            }

            pack[11]=pack[1]+pack[2]+pack[3]+temp;//校验和
            pack[12]=0xBA;                        //包尾BA

            printf("cmd=%x\n",0x0a);
            for(uint i=0;i<sizeof(pack);i++){
                printf("%02x ",pack[i]);
            }

            int len=write(fd, pack, sizeof(pack));
            usleep(100000);
            printf("\n");
            return len;
        }
        return -1;
    }

    void stop_run(){
        flag=false;
        this->wait();
    }

    static QString card;
signals:
    void serial_result(int cmd,unsigned int result);
private:
    int fd;
    bool flag;
    enum {bagHead,bagSign,bagLen,bagCmd,bagData};
    void data_processing(QVector<unsigned char> &data,int len);
};

#endif // SERIALBP_H
