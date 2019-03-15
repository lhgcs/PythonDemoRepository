#ifndef SERIAL_H
#define SERIAL_H

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

#include <QDebug>

#define NONEWARE  0
#define HARDWARE  1
#define SOFTWARE  2

class Serial
{
public:
    Serial();
    int InitSerialPort(char *ttyname,
                       int BaudRate=B9600,
                       char cBits='8',
                       char cCheck='N',
                       char cStops='1',
                       int FlowCtrl=NONEWARE);

    static int data_prser(QVector<uchar> &pack,QVector<uchar> &data);
    static int data_prser2(QVector<uchar> &pack,QVector<uchar> &data);
    static int send_pack(int fd, uchar* pack, int len);
    static int send_cmd(int fd,unsigned char cmd);
    static int send_open(int fd, uchar cmd, uchar data1);
    static int send_open(int fd, uchar cmd, uchar data1,uchar data2);
    static void cmd_package(uchar cmd,uchar pack[]);
    static void open_package(uchar cmd, uchar data, uchar pack[]);
    static void open_package(uchar cmd,uchar data1,uchar data2,uchar pack[]);
    static void printf_hex(uchar *Str, int len);
    static void close_device(int fd){
        close(fd);
    }

private:
    int fd;
};

#endif // SERIAL_H
