#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QVector>
#include <QUrlQuery>
#include "app.h"
#include "Loading/jsonhandler.h"

class HttpRequest: public QObject
{
     Q_OBJECT
public:

    QString currentPutOut;
    bool isBusy;

    explicit HttpRequest(QObject *parent=0);
    static HttpRequest *mHttpRequest;
    static HttpRequest *getInstance(){
        if(mHttpRequest == NULL){
            mHttpRequest=new HttpRequest;
        }
        return mHttpRequest;
    }

    void send_Request(const int pay, const int box);
    void send_Result(const int result, const QString who);
    void send_Result(QString who);
    void send_stauts(const QString &status);
    void send_time(){
        QString url=App::ip+"/time.php";
        qDebug()<<"\n\n\n"<<"HTTP 1: "<<url;
        manager->get(QNetworkRequest(QUrl(url)));
    }
    void send_card(QString cardId){
        QUrl url=App::ip+"/pay/card/card_check";
        QUrlQuery query;
        query.addQueryItem("card_id",cardId);
        url.setQuery(query);
        qDebug()<<"HTTP card: "<<url;
        manager->get(QNetworkRequest(url));
    }
    void send_goods_id(const QString str){
        QUrl url=App::ip+"/home/interaction/check_log_num_grid";
        QUrlQuery query;
        query.addQueryItem("code_num",str);
        query.addQueryItem("mac_id",App::macId);
        url.setQuery(query);
        qDebug()<<"HTTP goods_id: "<<url;
        manager->get(QNetworkRequest(url));
    }
    void send_login(){
        QUrl url=App::ip+"/admin/resource/check_login";
        QUrlQuery query;
        query.addQueryItem("user",App::user);
        query.addQueryItem("pass",App::password);
        query.addQueryItem("mac_id",App::macId);
        url.setQuery(query);
        qDebug()<<"HTTP login: "<<url;
        manager->get(QNetworkRequest(url));
    }
    int get_position(){
        for(int i=0;i<buy_goods.size();i++){
            if(buy_goods.at(i).orderNum==currentPutOut){
                return buy_goods.at(i).box;
            }
        }
        return -1;
    }
    void send_status2(QString str){
        QUrl url=App::ip+"/home/interaction/replenishment_gzj";
        QUrlQuery query;
        query.addQueryItem("mac_id",App::macId);
        query.addQueryItem("boxs",str);
        url.setQuery(query);
        qDebug()<<"HTTP login: "<<url;
        manager->get(QNetworkRequest(url));
    }
    void send_FixedCommunication(){
        QString url=App::ip+"/home/interaction/grid_checkname?mac_id="+App::macId;
        manager->get(QNetworkRequest(QUrl(url)));
        qDebug()<<url;
    }
public:
    static bool get_net_source(const QString &url,QByteArray &data,int time_ms=5000){
        qDebug()<<url;
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
                 reply->abort();
                 reply->close();
                 reply->deleteLater();
                 reply = NULL;
                 return true;
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
         return false;
     }

    QString qrencode;
    enum{qrcode,open,upload,time,card};

signals:
    void data(int type,int pay_or_box);
    void status_signal();
    void http_card();
    void http_time();

private slots:
    void replyFinished(QNetworkReply* reply);
    void slot_timeout(){
        send_Request();
    }

private:
    void send_Request();
    JsonHandler *jsonHandler;
    QNetworkAccessManager *manager;
    QTimer *timer;
    bool replyFlag;

    enum PutOutStatus{
        putOut_NO,
        putOut_WAIT,
        putOut_ING,
        putOut_YES
    };
    struct buyGoods{
        int cnt;
        int payWay;
        int box;
        PutOutStatus status;
        QString orderNum;
    };

    QVector<struct buyGoods> buy_goods;

    int order_to_index(QString &order){
        for(int i=0;i<buy_goods.size();i++){
            if(buy_goods[i].orderNum==order)return i;
        }
        return -1;
    }
    QString joint_qrencode_url(const int &pay){
        QString url=App::ip+"/pay/";
        switch (pay) {
            case App::buyAlipay:url.append("ali/gridali"); break;
            case App::buyWeixin:url.append("wx/gridwx");  break;
            case App::buyCard:  url.append("card/gridcard");break;
            case App::buyMoney: url.append("bnote");break;
            default:break;
        }
        return url;
    }
    QString joint_qrencode_url2(const int &pay){
        QString url=App::ip+"/pay/";
        switch (pay) {
            case App::buyAlipay:url.append("ali/"); break;
            case App::buyWeixin:url.append("wx/");  break;
            case App::buyCard:  url.append("card/");break;
            case App::buyMoney: url.append("bnote");break;
            default:break;
        }
        return url;
    }


    QString joint_qrencode_url3(const int &pay){
        QString url=App::ip+"/pay/";
        switch (pay) {
            case App::buyAlipay:url.append("ali/grid_result"); break;
            case App::buyWeixin:url.append("wx/grid_result");  break;
            case App::buyCard:  url.append("card/grid_result");break;
            case App::buyMoney: url.append("bnote");break;
            default:break;
        }
        return url;
    }

    bool open_box(const int index){
        if(buy_goods.at(index).status==putOut_WAIT){
            int box=buy_goods.at(index).box;
            if((box>=1)&&(box<=App::deviceNum*App::oneDeviceGoodsNum)){
                isBusy=true;
                currentPutOut=buy_goods.at(index).orderNum;
                buy_goods[index].status=putOut_ING;
                emit data(open,box);
                qDebug()<<"http send open";
                return true;
            }
        }
        return false;
    }   
    void ready_open_box(){
        for(int i=0;i<buy_goods.size();i++){
            qDebug()<<i;
            if(!isBusy){//(buy_goods.at(i).status==putOut_WAIT)
                if(open_box(i)){
                    qDebug()<<"now put out:"<<currentPutOut;
                    return;
                }
                else{
                    isBusy=false;
                }
            }
            else{
                qDebug()<<"http busy";
            }
        }
    }
};

#endif // HTTPREQUEST_H
