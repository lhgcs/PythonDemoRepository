#ifndef HTTPDOWNFILETHREAD_H
#define HTTPDOWNFILETHREAD_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

#include <QFile>

#include <QDebug>

#include <QRunnable>
#include <QThreadPool>

#include "model/tablegoodinfo.h"
#include "configure/app.h"

#include <QMutex>
#include <QMutexLocker>

//QThreadPool和QRunnable:重用线程

class HttpDownFileThread : public QRunnable
{
public:
    int id;
    HttpDownFileThread();
    static void dowm_file(const QString &url,const QString fileName,const int id,int time=5000){
        HttpDownFileThread *test = new HttpDownFileThread();
        test->set_data(url,fileName,time);
        test->id=id;
        setDownThreadNum_add_one();
        // QThreadPool 持有test对象并自动delete
        QThreadPool::globalInstance()->start(test);
    }
    void set_data(const QString &_url,const QString _fileName,int _time){
        url=_url;
        fileName=_fileName;
        time=_time;
    }

    static int getDownThreadNum(){
        QMutexLocker locker(&mutex);
        return downThreadNum;
    }

    static void setDownThreadNum(int value){
        QMutexLocker locker(&mutex);
        downThreadNum = value;
    }
    static void setDownThreadNum_add_one(){
        QMutexLocker locker(&mutex);
        downThreadNum++;
        qDebug()<<"downThreadNum:"<<downThreadNum;
    }
    static void setDownThreadNum_dec_one(){
        QMutexLocker locker(&mutex);
        downThreadNum--;
    }

    /*
1、QMutex ()//构造1个mutex
2、lock ()//锁
3、tryLock ()//尝试着锁
4、unlock ()//释放锁


上面的代码真实的揭露了QMutex的无力，因为只要有mutex.lock()，必然要有mutex.unlock()，否则临界区里的资源将再不能被访问；而上面的代码并不能保证QMutex的对象一定会unlock(代码可能从某个地方就走了，再不回来了)。这个时候QMutexLocker就发挥用处了，
因为QMutexLocker一定是以函数内部的局部变量的形式出现的，当它的作用域结束的时候，这个互斥就自然unlock了。

QMutexLocker locker(&mutex);//定义的时候就上锁了
//超出函数的作用域就解锁了
}

虽然互斥的功能保证了临界区资源的安全，但是在某些方面并不符合实际；比如一般情况下，资源可以被并发读！举个实际的例子：有一本书（比如CivilNet BOOK），当某个人读到一页时，另外一个人（或者多个人）也可以过来读；但是，当1个人往上面写笔记时，其他人不能一起写，而且只有这个人把笔记写完了，再让大家一起看。

QReadWriteLock的作用就是保证各个线程能并发的读某个资源，但是要写的话，就得真的lock了（所以，QReadWriteLock适合大量并发读，偶尔会有写的情况）；

QReadWriteLock lock;

lock.lockForRead();
read_file();
lock.unlock();


特别的，对于lock这个全局锁来说：

1、只要有任意一个线程lock.lockForWrite()之后，所有之后的lock.lockForRead()都将会被阻塞；
2、只要有任意一个线程的lock.lockForWrite()动作还在被阻塞着的话，所有之后的lock.lockForRead()都会失败；
3、如果在被阻塞的队列中既有lock.lockForWrite()又有lock.lockForRead()，那么write的优先级比read高，下一个执行的将会是lock.lockForWrite()。

大多数情况下，QReadWriteLock都是QMutex的直接竞争者.和QMutex类似，QReadWriteLock也提供了它的简化类来应付复杂的加锁解锁（也是通过函数作用域的手段），代码如下：
****************************************************************
QReadWriteLock lock;

QByteArray readData()
{
QReadLocker locker(&lock);


void writeData(const QByteArray &data)
{
QWriteLocker locker(&lock);
…
}
*************************************************************

第四部分：QSemaphore 提供了QMutex的通用情况

反过来，QMutex是QSemaphore的特殊情况，QMutex只能被lock一次，而QSemaphore却可以获得多次；当然了，那是因为Semaphores要保护的资源和mutex保护的不是一类；Semaphores保护的一 般是一堆相同的资源； 比如：
1、mutex保护的像是洗手间这样的，只能供1人使用的资源（不是公共场所的洗手间）；
2、Semaphores保护的是像停车场、餐馆这样有很多位子资源的场所；
     *
     */

    void run(){
        qDebug() << "这里子线程" << QThread::currentThread()<< url;
        bool res;
        for(int i=0;i<3;i++){
            res=get_net_source();
            if(res == true){
                break;
            }
        }
        if(res == false){
            if(id>0 && id<App::goodsTotal){
                //SqliteHandler::getInstence()->table_field_clear(App::goodsTableName,id,"time");
                TableGoodInfo::getInstence()->table_update_time(
                            TableGoodInfo::getInstence()->getTableName(),id,"-1"
                            );
            }

            if(is_file_exits(fileName)){
                QFile file(fileName);
                file.remove();
            }
        }
        setDownThreadNum_dec_one();
    }

private:
    QString url;
    QString fileName;
    int time;

    static int downThreadNum;
    static QMutex mutex;

    bool get_net_source(){
        bool res=false;
         QNetworkAccessManager manager;
         QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(url)));
         QEventLoop eventLoop;
         QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
         QTimer timer;
         timer.setSingleShot(true);
         QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
         timer.start(time);
         eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
         if(timer.isActive()){
             timer.stop();
             if(reply->error()==QNetworkReply::NoError){
                 QByteArray data;
                 data = reply->readAll();  //UTF-8编码

                 QFile file(fileName);
                 if(file.open(QIODevice::WriteOnly)){
                     file.resize(0);        //清空文件内容
                     if(file.write(data)>0){
                         file.flush();
                         qDebug()<<fileName<<"save ok";
                         res=true;
                     } else{
                         qDebug()<<fileName<<"save failed";
                     }
                     file.close();
                 }else{
                     qDebug()<<"open failed\n";
                 }
             }
             else{
                 qDebug()<<"load failed:"<<reply->error();
             }
         }
         else{
             qDebug()<<"load time out";
             QObject::disconnect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
         }
         reply->abort();
         reply->close();
         reply->deleteLater();//并没有将对象立即销毁，而是向主消息循环发送了一个event，下一次主消息循环收到这个event之后才会销毁对象
         reply = NULL;
         return res;
     }

};

#endif // HTTPDOWNFILETHREAD_H
