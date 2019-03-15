#include "tcpthread.h"
#include <QDataStream>

#define MINLEN          7
#define TYPE_CMD        0
#define TYPE_FILE_START 1
#define TYPE_FILE_ING   2
#define TYPE_FILE_END   3

//连接TCP
TCPThread::TCPThread(){
    this->socket=NULL;
    this->clientIP=QHostAddress::Any;
    this->clientPort=6002;
    this->fileName="/home/log/1.txt";
    this->fileSize=0;
    this->openMode=QTcpSocket::ReadWrite;
    this->server=new QTcpServer;
    QObject::connect(this->server,SIGNAL(acceptError(QAbstractSocket::SocketError)),
                     this,SLOT(accept_error_slot(QAbstractSocket::SocketError)));
    QObject::connect(this->server,SIGNAL(newConnection()),this,SLOT(connect_slot()));//新连接信号触发
    listen_to_client();
}

//读取数据
void TCPThread::read_data_slot(){
    if(socket->bytesAvailable()<=0){
        qDebug()<<"no data";
        return;
    }
    QByteArray byte=socket->readAll();
    qDebug()<<byte;
}

bool TCPThread::write_data(int type,QByteArray &byte){
    if(socket==NULL){
        qDebug()<<"socket null";
        return false;
    }
    QByteArray temp;
    char h1=0xAB,h2=0xBA;
    temp.append(h1);
    temp.append(h2);
    temp.append((char)type);
    int packetSize=byte.size();
    temp.append((char)(packetSize>>24));
    temp.append((char)(packetSize>>16));
    temp.append((char)(packetSize>>8));
    temp.append((char)(packetSize>>0));
    temp.append(byte);

    int len=temp.size();

    while (1) {
        int writeSize=socket->write(temp);
//        if(!socket->waitForBytesWritten(10000)){
//            qDebug()<<"write error：";
//            return false;
//        }
        len-=writeSize;
        if(len<=0){
            qDebug()<<temp;
            return true;
        }else{
            temp=temp.right(len);
            qDebug()<<"write error："<<len<<temp.length();
            qDebug()<<temp;
        }
    }
    return true;
}

//while (1) {
//    int writeSize=socket->write(temp);
//    if(!socket->waitForBytesWritten(10000)){
//        qDebug()<<"22222222222222write error：";
//        return false;
//    }
//    len-=writeSize;
//    if(len<=0){
//        qDebug()<<temp;
//        return true;
//    }else{
//        temp=temp.right(len);
//        qDebug()<<"write error："<<len<<temp.length();
//        qDebug()<<temp;
//    }
//}
//    socket->flush();//清空缓冲区

bool TCPThread::send_file(QString name){
    if(socket==NULL){
        return false;
    }
    if(socket->state()!=QAbstractSocket::ConnectedState){
        return false;
    }
    bool res=true;

    QFile f(name);
    if(f.open(QIODevice::ReadOnly)){
        int size=f.size();
        QString str=name+":"+QString::number(size);
        QByteArray str_temp=str.toLatin1();
        write_data(TYPE_FILE_START,str_temp);
        QByteArray temp;
        while(size>0) {
            if(size>=512){
                temp.resize(512);
                temp=f.read(512);
            }
            else{
                temp.resize(size);
                temp=f.read(size);
            }
            while (1) {
                if(!write_data(TYPE_FILE_ING,temp)){
                    qDebug()<<"22222222";
                    continue;
                }
                else{
                    size-=temp.size();
                    temp.clear();
                    break;
                }
            }
        }
        write_data(TYPE_FILE_END,str_temp);
        f.close();
    }
    return res;
}


//while (1) {
//    if(!write_data(TYPE_FILE_ING,temp)){
//        qDebug()<<"22222222";
//        continue;
//    }
//    else{
//        size-=temp.size();
//        temp.clear();
//        break;
//    }
//}


//现在求大神帮忙解决QTCPsocket状态异常的问题。。。怎么检查QTCPsocket与服务器已经断开 这个状态

//    QDataStream in(socket,QIODevice::ReadOnly);//数据流读取数据,只读
//    QString read_data;
//    in >> read_data;                           //输入流把空格作为数据间的分隔符

//实例（向一个流中写二进制数据）：

//    QFile f( "file.dta" );
//    f.open( IO_WriteOnly );
//    QDataStream s( &f );    // 我们将把数据串行化至文件f
//    s << "the answer is";   // 串行化一个字符串
//    s << (Q_INT32)42;       // 串行化一个整数

//实例（从一个流中读二进制数据）：

//    QFile f( "file.dta" );
//    f.open( IO_ReadOnly );
//    QDataStream s( &f );    // 从文件f中读取串行化的数据
//    QString str;
//    Q_INT32 a;
//    s >> str >> a;          // 提取出“the answer is”和42


//QFile f( "file.xxx" );
//f.open( IO_WriteOnly );
//QDataStream s( &f );

//// 写一个含有“幻数”和版本号的头
//s << (Q_UINT32)0xa0b0c0d0;
//s << (Q_INT32)123;

//// 写数据
//s << [lots of interesting data]

//然后这样读：

//QFile f( "file.xxx" );
//f.open( IO_ReadOnly );
//QDataStream s( &f );

//// 读取并检查头
//Q_UINT32 magic;
//s >> magic;
//if ( magic != 0xa0b0c0d0 )
//    return XXX_BAD_FILE_FORMAT;

//// 读取版本号
//Q_INT32 version;
//s >> version;
//if ( version < 100 )
//    return XXX_BAD_FILE_TOO_OLD;
//if ( version > 123 )
//    return XXX_BAD_FILE_TOO_NEW;
//if ( version <= 110 )
//    s.setVersion(1);

//// 读取数据
//s >> [很多有趣的数据];
//if ( version > 120 )
//    s >> [在1.2版中的新数据XXX];
//s >> [其它有趣的数据];



//recvBuffer.append(byte);
//int totalLen=recvBuffer.size();

//uchar packetHead1,packetHead2,packetType,packetSize;

//while (totalLen) {
//    if(totalLen<MINLEN){//小于包头
//        break;
//    }

//    QDataStream packet(recvBuffer);
//    packet.setByteOrder(QDataStream::BigEndian);
//    packet>>packetHead1>>packetHead2;
//    if((packetHead1!=0xAB)||(packetHead2!=0xBA)){//没有包头
//        break;
//    }
////        if(totalLen<packetLen+3){//长度不够
////            break;
////        }

//    packet>>packetType>>packetSize;
//    QByteArray temp;
//    packet>>temp;
//    if(temp.size()==packetSize){
//        qDebug()<<temp;
//    }
//    else{                                       //包不完整
//        break;
//    }

//    switch(packetType) {
//        case TYPE_CMD:

//            break;
//        case TYPE_FILE_START:
//            packet>>fileName>>fileSize;
//            QFile f(fileName);
//            f.open(QIODevice::WriteOnly);//WriteOnly模式下，没有写入操作，则直接清空
//            f.close();
//            break;
//        case TYPE_FILE:
//            packet>>fileName>>fileSize;
//            packet>>temp;
//            save_file(temp);
//            break;
//        default:break;
//    }

//    byte=recvBuffer.right(totalLen-packetLen);//超过一个包的数据
//    totalLen=byte.size();
//    recvBuffer=byte;
//}
