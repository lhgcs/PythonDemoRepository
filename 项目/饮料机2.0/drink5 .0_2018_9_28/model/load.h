#ifndef LOAD_H
#define LOAD_H

#include <QCoreApplication>
#include <QTime>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "QJsonArray"
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include <QFile>
#include <QPixmap>
#include <QTcpSocket>

#include "configure/app.h"
#include "model/httprequest.h"
#include <QThread>

class Load:public QThread
{
    Q_OBJECT
public:
    Load(QWidget *parent=0);
    int loading(){
        int res=-1;
        for(int i=0;i<5;i++){
            res=load_goods_info();
            res+=load_video_info();

            if(res >= 0){
                break;
            }
//            QThread::sleep(1);
            //sleep(1);
            mysleep(500);
        }
        downFileNum = res;
        return res;
    }

    int getDownFileNum() const;


    void run(){
        loading();
    }

signals:
    void finish(bool value);

private:

    int downFileNum;

    QJsonArray get_json_object_value_array(const QJsonDocument &jsonDocument,const QString &key){//,QString defaultValue
        QJsonArray valueStr;
        if (jsonDocument.isObject()){
            QJsonObject obj =jsonDocument.object();//取得最外层这个大对象
            if (obj.contains(key)) {               //如果包含key,进行取值
                QJsonValue json_value = obj.take(key);
                if (json_value.isArray()){
                    valueStr=json_value.toArray();
                }
            }else{
                printf("no key\n");
            }
        }
        return valueStr;
    }

    QString get_str_by_object(QJsonObject obj,QString key){
        if(obj.contains(key)){
            QJsonValue value = obj.take(key);
            if(value.isString()){
                return value.toVariant().toString();
            }
        }
        return "0";
    }
    int get_int_by_object(QJsonObject obj,QString key){
        if(obj.contains(key)){
            QJsonValue value = obj.take(key);
            if(value.isDouble()){
                return value.toInt();
            }
        }
        return 0;
    }

    int load_goods_info();
    int load_video_info();
    int parse_goods_info(const QJsonArray &jsonArray);
    int parse_goods_info_old(const QJsonDocument &jsonDocument);
    int parse_video_info(const QJsonArray &jsonArray);
    void mysleep(unsigned int msec){
        QTime dieTime = QTime::currentTime().addMSecs(msec);              //以毫秒为单位的延时
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);  //延时一段时间不阻塞主线程
    }
};

#endif // LOAD_H
