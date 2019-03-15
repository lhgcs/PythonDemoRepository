#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QTimer>
#include <QVector>
#include <QEventLoop>
#include "configure/app.h"
#include "configure/toast.h"
#include <QNetworkRequest>
#include "jsonhandler.h"
#include <QUrlQuery>

#define PUT_OUT_NO    0
#define PUT_OUT_WAIT  1
#define PUT_OUT_ING   2
#define PUT_OUT_YES   3

class HttpRequest: public QObject
{
     Q_OBJECT
public:
    enum{open,UpLoad,temperature,getServerData};
    enum {NONE=0,WEIXIN=1,ALIPAY=2,WEIXIN_AND_ALIPAY=3,GAME=4};

    QString http,mac_id;

    explicit HttpRequest(QObject *parent=0);
    static HttpRequest *mHttpRequest;
    static HttpRequest *getInstance(){
        if(mHttpRequest == NULL){
            mHttpRequest = new HttpRequest;
        }
        return mHttpRequest;
    }

    void send_request_qrcode(int current_payment, int num);
    void send_request();
    int send_result(int result, QString who);

    void get_time();
    void get_temp();
    void get_user();

    void send_status(QMap<int, int> map);
    void send_env(const int &value);
    void send_onlide();

    /**
      加入列表
     * @brief add_to_list
     */
    int add_to_list(int status,int box,int payWay,QString order){
        int res=-1;
        if(order_to_index(order) < 0){
            BuyGoods temp={40,status,box,payWay,order};
            goods.append(temp);
            res=0;
            qDebug()<<"Add"<<temp.box<<temp.cnt<<temp.payWay<<temp.order_num;
        }
        return res;
    }
    /**
      立刻送出
     * @brief put_out
     * @param order
     */
    int put_out(const QString &order){
        int res=-1;
        int index=order_to_index(order);
        if(index >= 0 && index < goods.size()){
            goods[index].cnt=-1;
            if(goods.at(index).status == PUT_OUT_NO){
                if(isBusy){
                    goods[index].status=PUT_OUT_WAIT;
                    qDebug()<<"waitting";
                }else{
                    put_out_ing(index);
                }
           }
            res=0;
       }else{
            qDebug()<<"index error";
       }
        return res;
    }

signals:
    void http_qrencode(int payment,QString qrencode);
    void http_data(int tyte,int data1=0,int data2=0);

private slots:
    void replyFinished(QNetworkReply* reply);
    void slot_timeout(){
        send_request();
    }

private:
    bool isBusy;
    typedef struct{
        int cnt;
        int status;
        int box;
        int payWay;
        QString order_num;
    }BuyGoods;

    QNetworkAccessManager *manager;
    QTimer *timer;
    QString current_put_out;
    QVector<BuyGoods> goods;

    /**
      查找到下标
     * @brief order_to_index
     * @param order
     * @return
     */
    int order_to_index(const QString &order){
        for(int i=0;i<goods.size();i++){
            if(goods[i].order_num == order){
                return i;
            }
        }
        return -1;
    }

    /**
      出下一个
     * @brief put_out_next
     */
    void put_out_next(){
        for(int i=0;i<goods.size();i++){
            if(goods.at(i).status == PUT_OUT_WAIT){
                if(put_out_ing(i) == 0){
                    return;
                }
            }
        }
    }
    /**
      送出操作
     * @brief put_out_ing
     * @param index
     */
    int put_out_ing(const int index){
        int res=-1;
        goods[index].status=PUT_OUT_ING;
        int box=goods.at(index).box;
        if((box >= 1) && (box <= App::goodsTotal)){
            isBusy=true;
            current_put_out=goods.at(index).order_num;
            qDebug()<<"send open"<<box;
            emit http_data(open,box);
            res=0;
        }else{
            current_put_out.clear();
            isBusy=false;
        }
        return res;
    }

    void get(QString &base_url){
        QUrlQuery query;
        QUrl url(base_url);
        url.setQuery(query);
        qDebug()<<url;
        manager->get(QNetworkRequest(url));
    }
    /**
     * @brief get
     * @param url
     */
    void get(QString &base_url,QMap<QString,QString> &map){
        map.insert("mac_id",App::MACID);
        QUrlQuery query;
        for (QMap<QString,QString>::iterator it = map.begin(); it != map.end(); ++it ) {
            query.addQueryItem(it.key(),it.value());
        }
        QUrl url(base_url);
        url.setQuery(query);
        qDebug()<<url;
        manager->get(QNetworkRequest(url));
    }
    /**
     * @brief post
     * @param url
     * @param map
     */
    void post(QString &base_url,QMap<QString,QString> &map){
        QByteArray json = JsonHandler::map_to_json_object(map);
        qDebug()<<base_url<<json;
        QNetworkRequest request(base_url);
        request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
        request.setRawHeader("Authorization","Bearer 49152bfddae0c5b5d492d3d9871f8c11");//服务器要求的数据头部
        manager->post(request,json);
    }
    void post(QString &base_url,QByteArray &json){
        qDebug()<<base_url<<json;
        QNetworkRequest request(base_url);
        request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
        request.setRawHeader("Authorization","Bearer 49152bfddae0c5b5d492d3d9871f8c11");//服务器要求的数据头部
        manager->post(request,json);
    }

public:
    static QString get_net_source(const QString &url,int time_ms=5000){
        qDebug()<<url;
         QString str=NULL;
         QNetworkAccessManager manager;
         QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(url)));
         QEventLoop eventLoop;
         QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
         QTimer timer;
         timer.setSingleShot(true);
         QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
         timer.start(time_ms);
         eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
         if(timer.isActive()){
             timer.stop();
             if(reply->error()==QNetworkReply::NoError){//QNetworkReply::TimeoutError
                 QByteArray data;
                 data = reply->readAll();  //UTF-8编码
                 str=data;
                 qDebug()<<str;
             }
             else{
                 qDebug()<<"load failed:"<<reply->error();
             }
         }else{
             qDebug()<<"load time out";
             QObject::disconnect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
         }
         reply->abort();
         reply->close();
         reply->deleteLater();//并没有将对象立即销毁，而是向主消息循环发送了一个event，下一次主消息循环收到这个event之后才会销毁对象
         reply = NULL;
         return str;
     }

    static bool get_net_source(const QString &url,QByteArray &data,int time_ms=5000){
        qDebug()<<url;
         bool res=false;
         QNetworkAccessManager manager;
         QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(url)));
         QEventLoop eventLoop;
         QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
         QTimer timer;
         timer.setSingleShot(true);
         QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
         timer.start(time_ms);
         eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
         if(timer.isActive()){
             timer.stop();
             if(reply->error()==QNetworkReply::NoError){
                 data = reply->readAll();  //UTF-8编码
                 res=true;
             }
             else{
                 qDebug()<<"load failed:"<<reply->error();
             }
         }else{
             qDebug()<<"load time out";
         }
         QObject::disconnect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
         reply->abort();
         reply->close();
         reply->deleteLater();//并没有将对象立即销毁，而是向主消息循环发送了一个event，下一次主消息循环收到这个event之后才会销毁对象
         reply = NULL;
         return res;
     }

    static QByteArray get_net_source_post(const QString &url,QByteArray json,int time_ms=5000){
        qDebug()<<url<<json;
        QByteArray data = NULL;
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
        request.setRawHeader("Authorization","Bearer 49152bfddae0c5b5d492d3d9871f8c11");//服务器要求的数据头部

        QNetworkAccessManager manager;
        QNetworkReply* reply = manager.post(request,json);
        QEventLoop eventLoop;
        QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        QTimer timer;
        timer.setSingleShot(true);
        QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
        timer.start(time_ms);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
         if(timer.isActive()){
             timer.stop();
             if(reply->error()==QNetworkReply::NoError){//QNetworkReply::TimeoutError
                 data = reply->readAll();  //UTF-8编码
                 qDebug()<<data;
             }
             else{
                qDebug()<<"load failed:"<<reply->error();
                QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                int statusCode = variant.toInt();// 根据状态码做进一步数据处理
                switch (statusCode) {
                    case 403:
                        Toast::getInstance()->set_text("403");
                        break;
                    default:
                        //Toast::getInstance()->set_text("未知错误:"+QString::number(statusCode));
                        break;
                }
             }
         }else{
             qDebug()<<"load time out";
             QObject::disconnect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
         }
         reply->abort();
         reply->close();
         reply->deleteLater();//并没有将对象立即销毁，而是向主消息循环发送了一个event，下一次主消息循环收到这个event之后才会销毁对象
         reply = NULL;
         return data;
     }
    QString getCurrent_put_out() const;
    void setCurrent_put_out(const QString &value);
    void setIsBusy(bool value);
};

#endif // HTTPREQUEST_H



//        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
//        QHttpPart textPart;
//        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"text\""));
//        textPart.setBody("my text");

//        QHttpPart imagePart;
//        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
//        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\""));
//        QFile *file = new QFile("image.jpg");
//        file->open(QIODevice::ReadOnly);
//        imagePart.setBodyDevice(file);
//        file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

//          multiPart->append(textPart);

//          proc_request->setUrl(url);
        //**************自我添加部分****************************
//        QString bd = "fasdlkfjaslkdgj;lkadjglk;";
//        multipart->setBoundary(bd.toLatin1());
//        proc_request->setHeader(QNetworkRequest::ContentTypeHeader,
//        \"multipart/form-data;boundary="+bd);
        //****************自我添加部分*************************
//        proc_manager->post(*proc_request,multiPart);

