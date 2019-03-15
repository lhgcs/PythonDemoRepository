#include "tcpthread.h"
#include <QDataStream>

#define MINLEN 7

//连接TCP
TCPThread::TCPThread(){
    this->fileName="";
    this->fileSize=0;
    this->serverIP="127.0.0.1";
    this->serverPort=6002;
    this->socket=new QTcpSocket;
    this->openMode=QTcpSocket::ReadWrite;
    QObject::connect(this->socket,SIGNAL(connected()),   this,SLOT(connect_slot()));
    QObject::connect(this->socket,SIGNAL(disconnected()),this,SLOT(disconnect_slot()));
    QObject::connect(this->socket,SIGNAL(readyRead()),   this,SLOT(read_data_slot()));
    QObject::connect(this->socket,SIGNAL(error(QAbstractSocket::SocketError)),
                     this,SLOT(error_slot(QAbstractSocket::SocketError)));
    connect_to_server();
}
//读取数据
//void TCPThread::read_data_slot(){
//    int availableSize=socket->bytesAvailable();
//    if(availableSize<=0){
//        return;
//    }

//    if(recvBuffer.size()+availableSize<MINLEN){
//        return;
//    }
//    QByteArray tempHead=socket->read(MINLEN);
//    qDebug()<<"packetHead:"<<tempHead;
//    uchar packetHead1,packetHead2;
//    packetHead1=tempHead.at(0);
//    packetHead2=tempHead.at(1);
//    if((packetHead1!=0xAB)||(packetHead2!=0xBA)){//没有包头
//        return;
//    }

//    uchar packetType=tempHead.at(2);
//    int packetSize=((int)tempHead.at(3))<<24|
//                   ((int)tempHead.at(4))<<16|
//                   ((int)tempHead.at(5))<<8|
//                   ((int)tempHead.at(6));
//    qDebug()<<"packetSize:"<<packetSize;

//    QByteArray temp=socket->read(packetSize);
//    qDebug()<<"packetData:"<<temp;

//    if(temp.size()<packetSize){
//        recvBuffer.append(tempHead);
//        recvBuffer.append(temp);
//        return;
//    }
//    else if(temp.size()==packetSize){           //包完整
//        switch(packetType) {
//            case TYPE_CMD:{
//                qDebug()<<"cmd"<<temp;
//                write_data("ok");
//                break;
//            }
//            case TYPE_FILE_START:{
//                fileName.clear();
//                fileSize=0;
//                QString str=temp;
//                QStringList list=str.split(":");
//                if(list.size()!=2){
//                    return;
//                }
//                fileName=list.at(0)+"1";/////////
//                str=list.at(1);
//                fileSize=str.toInt();
//                QFile f(fileName);
//                f.open(QIODevice::WriteOnly);//WriteOnly模式下，没有写入操作，则直接清空
//                f.close();
//                break;
//            }
//            case TYPE_FILE_ING:{
//                qDebug()<<"file:"<<fileName<<fileSize;
//                if(fileName.isEmpty()){
//                    return;
//                }
//                qDebug()<< save_file(temp);
//                break;
//            }
//            case TYPE_FILE_END:{
//                if(fileName.isEmpty()){
//                    return;
//                }
//                QFile f(fileName);
//                if(f.size()==fileSize){
//                    write_data("file ok");
//                }
//                else{
//                    write_data("file error");
//                }
//                break;
//            }
//            default:break;
//        }
//    }

//    if(socket->bytesAvailable()){
//        temp=socket->readAll();
//        recvBuffer.append(temp);
//    }
//}

void TCPThread::read_data_slot(){
    int availableSize;
    qDebug()<<"121212";
    QByteArray temp;
    do{
        availableSize=socket->bytesAvailable();
        qDebug()<<"availableSize"<<availableSize;
        if(availableSize<=0){
            return;
        }
        temp.clear();
        temp.resize(0);
        if(recvBuffer.size()<MINLEN){
            if(recvBuffer.size()+availableSize<MINLEN){
                temp=socket->readAll();
                recvBuffer.append(temp);
                availableSize=0;
                break;
            }
            else{
                int len=MINLEN-recvBuffer.size();
                temp=socket->read(len);///////////////////////
                recvBuffer.append(temp);
                availableSize-=temp.size();
            }
        }

        if(is_head(recvBuffer)){               //是否有头
            uchar packetType=recvBuffer.at(2);
            int packetSize=((int)recvBuffer.at(3))<<24|
                           ((int)recvBuffer.at(4))<<16|
                           ((int)recvBuffer.at(5))<<8|
                           ((int)recvBuffer.at(6));
            qDebug()<<"packetSize:"<<packetSize;
            if(packetSize<=0){
                recvBuffer.clear();
                recvBuffer.resize(0);
                return;
            }
            temp.clear();
            temp.resize(0);
            temp=socket->read(packetSize);
            qDebug()<<"packetData:"<<temp;
            availableSize-=temp.size();
            if(temp.size()<packetSize){       //包没收完整
                qDebug()<<"111111111";
                if(temp.size()>0){
                    recvBuffer.append(temp);
                }
                continue;////////////////////////////////
            }
            else{                             //包完整
                switch(packetType) {
                    case TYPE_CMD:{
                        qDebug()<<"cmd"<<temp;
                        write_data("ok");
                        break;
                    }
                    case TYPE_FILE_START:{
                        fileName.clear();
                        fileSize=0;
                        QString str=temp;
                        QStringList list=str.split(":");
                        if(list.size()!=2){
                            return;
                        }
                        fileName=list.at(0)+"1";/////////
                        str=list.at(1);
                        fileSize=str.toInt();
                        QFile f(fileName);
                        f.open(QIODevice::WriteOnly);//WriteOnly模式下，没有写入操作，则直接清空
                        f.close();
                        break;
                    }
                    case TYPE_FILE_ING:{
                        if(fileName.isEmpty()){
                            return;
                        }
                        qDebug()<< save_file(temp);
                        break;
                    }
                    case TYPE_FILE_END:{
                        if(fileName.isEmpty()){
                            return;
                        }
                        QFile f(fileName);
                        if(f.size()==fileSize){
                            write_data("file ok");
                        }
                        else{
                            write_data("file error");
                        }
                        break;
                    }
                    default:break;
                }
                recvBuffer.clear();
                recvBuffer.resize(0);
            }
        }
        else{
            int index=find_head_index(recvBuffer,recvBuffer.size());
            if(index>0){
                recvBuffer=recvBuffer.mid(index);
            }
            else{
                recvBuffer.clear();
                recvBuffer.resize(0);
            }
//            recvBuffer=recvBuffer.right(recvBuffer.size()-1);
//            recvBuffer.remove(0,MINLEN);
        }
    }while((recvBuffer.size()>MINLEN)||(availableSize>0));
}

bool TCPThread::write_data(QString str){
    //在非正常的情况下断开服务器，例如，拔掉服务器的网线、服务器断电等等这些非正常动作以后，QTCPsocket的状态还是ConnectedState！
    //而且QTCPsocket->wirte()也返回正确的字节数
    if(socket->state()!=QAbstractSocket::ConnectedState){
        disconnect_slot();
    }
    QByteArray byte=str.toLatin1();
    int len=socket->write(byte);
   // waitForBytesWritten(); //等待数据写入socket
    if(len==byte.length()){
        qDebug()<<"write ok";
        return true;
    }
    qDebug()<<"write error："<<len<<byte.length();
    //    socket->flush();//清空缓冲区
    return false;
}


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
