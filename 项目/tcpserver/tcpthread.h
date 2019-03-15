#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QTcpServer>

class TCPThread : public QThread
{
    Q_OBJECT
public:
    TCPThread();
    ~TCPThread(){
        quit();
        wait();
        delete socket;
    }
    bool write_data(int type,QByteArray &byte);
    bool send_file(QString name);

private:
    QTcpServer *server;
    QTcpSocket *socket;
    QHostAddress clientIP;
    int clientPort;
    int openMode;
    int fileSize;
    QString fileName;
    QByteArray recvBuffer;
    bool readyFlag;

    void listen_to_client(){
        server->setMaxPendingConnections(2000);//设置最大允许连接数，不设置的话默认为30
        qDebug()<<"开始侦听";
        if(server->listen(clientIP,clientPort)){//监听任何连上19999端口的ip
            qDebug()<< "listen OK!";//QDBG
        }else{
            qDebug()<< "listen error!";
        }
    }
    void disconnect_to_server(){
        socket->abort();//取消原有的链接
    }

private slots:
    void connect_slot(){
        socket =server->nextPendingConnection(); //得到每个连进来的socket
        QObject::connect(this->socket,SIGNAL(readyRead()),this,SLOT(read_data_slot()));//有可读的信息，触发读函数槽
        QObject::connect(this->socket,SIGNAL(error(QAbstractSocket::SocketError)),
                         this,SLOT(error_slot(QAbstractSocket::SocketError)));
        qDebug()<<"连接成功";
    }
    void disconnect_slot(){
        qDebug()<<"断开连接";
    }
    void error_slot(QAbstractSocket::SocketError){
        qDebug()<<"error:"<<socket->errorString();//此处为错误信息
    }
    void accept_error_slot(QAbstractSocket::SocketError){
        qDebug()<<"error:"<<server->errorString();//此处为错误信息
    }
    void read_data_slot();
};

#endif // TCPTHREAD_H
