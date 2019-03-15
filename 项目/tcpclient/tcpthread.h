#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>

#define TYPE_CMD        0
#define TYPE_FILE_START 1
#define TYPE_FILE_ING   2
#define TYPE_FILE_END   3

class TCPThread : public QThread
{
    Q_OBJECT
public:
    TCPThread();
    ~TCPThread(){
//        requestInterruption();//是否请求终止
        quit();
        wait();
        delete socket;
    }

//protected:
//    virtual void run() Q_DECL_OVERRIDE { //run()处于次线程
//        while(!isInterruptionRequested())//是否请求终止
//        {

//        }
//    }

private:
    QTcpSocket *socket;
    QString serverIP;
    int serverPort;
    int openMode;

    QString fileName;
    int fileSize;
    QByteArray recvBuffer;

    void connect_to_server(){
        socket->abort();//取消原有的链接
        qDebug()<<"开始连接";
        this->socket->connectToHost(serverIP,serverPort);//链接到目标服务器
        socket->waitForConnected();//等待链接的建立
    }
    void disconnect_to_server(){
        socket->abort();//取消原有的链接
        //this->waitForDisconnected(); //等待链接断开
    }
    void recv_file(QByteArray &byte){
        if((fileSize>0)){
            QFile f(fileName);
            if(f.open(QIODevice::WriteOnly)){
                if(fileSize>1024){
                    f.write(byte);
                    fileSize-=1024;
                }
                else if(fileSize<=1024){
                    f.write(byte);
                    fileSize=0;
                }
                f.close();
            }
        }
    }

    bool save_file(QByteArray &byte){
        QFile f(fileName);
        if(f.open(QIODevice::WriteOnly|QIODevice::Append)){
            f.write(byte);
            f.close();
            return true;
        }
        return false;
    }

    bool is_head(const QByteArray &head){
        qDebug()<<"packetHead:"<<head;
        char packetHead1=head.at(0);
        char packetHead2=head.at(1);
        char packetType=head.at(2);
        char h1=0xAB,h2=0xBA;
        if((packetHead1==h1)&&(packetHead2==h2)){//有包头
            if((packetType==TYPE_CMD)||
               (packetType==TYPE_FILE_START)||
               (packetType==TYPE_FILE_ING)||
               (packetType==TYPE_FILE_END)){
               return true;
            }
        }
        qDebug()<<"no head";
        return false;
    }
    int find_head_index(const QByteArray &byte,int len){
        char h1=0xAB;
        for(int i=1;i<len;i++){//找到包头
            if(byte.at(i)==h1){
                return i;
            }
        }
        return -1;
    }

private slots:
    void connect_slot(){
        qDebug()<<"连接成功";
    }
    void disconnect_slot(){
        qDebug()<<"断开连接";
        //重新连接
        connect_to_server();
    }
    void error_slot(QAbstractSocket::SocketError){
        qDebug()<<"error:"<<socket->errorString();//此处为错误信息
    }
    void read_data_slot();
    bool write_data(QString str);
};

#endif // TCPTHREAD_H
