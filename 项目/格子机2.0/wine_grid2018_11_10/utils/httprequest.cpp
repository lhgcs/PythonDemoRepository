/*
 * http请求
 * 请求服务器
 * 2017-06-22
 */

#include "httprequest.h"
#include <QMutex>
#include <QUrlQuery>
#include "subwidget/dialoginstance.h"
#include "filehandler.h"
#define CNT    40
#define TIME   3000

HttpRequest *HttpRequest::mHttpRequest=NULL;

HttpRequest::HttpRequest(QObject *parent):QObject(parent)
{
    manager=new QNetworkAccessManager(this);//创建一个管理器
    timer=new QTimer(this);
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(slot_timeout()));
    buy_goods.clear();
    isBusy=false;
    jsonHandler=new JsonHandler;
}
//请求二维码
void HttpRequest::send_Request(const int pay, const int box){
    if(buy_goods.size()<25){
        QUrl url(joint_qrencode_url(pay));
        QUrlQuery query;
        query.addQueryItem("mac_id",App::macId);
        query.addQueryItem("box","box"+QString::number(box));
        if(pay==App::buyCard){
            if(App::card.isEmpty()){
                return;
            }
            query.addQueryItem("card_id",App::card);
        }
        url.setQuery(query);
        qDebug()<<"\n\n\n"<<"HTTP 1: "<<url;
        manager->get(QNetworkRequest(url));

        if(!timer->isActive()){
            timer->start(TIME);
        }
    }
    else{
       DialogInstance::getInstance()->setText("请求过于频繁");
    }
}
//请求支付结果
void HttpRequest::send_Request()
{
    if(buy_goods.isEmpty()){
//        timer->stop();
        return;
    }
    static QMutex mutex;//互斥锁 lock() 就是申请这个锁，如果被正占用，它只能等待(直到占有者调用了m_lock.unlock()它才能继续)。
    mutex.lock();

    for(int i=0;i<buy_goods.size();i++){
        if((buy_goods.at(i).cnt==0)||(buy_goods.at(i).status==putOut_YES)){
            buy_goods.erase(buy_goods.begin()+i);
                i--;
        }
    }

    for(int i=0;i<buy_goods.size();i++){
        if((buy_goods.at(i).status==putOut_NO)&&(buy_goods.at(i).cnt>0)){
            qDebug()<<"send request"<<buy_goods[i].cnt<<buy_goods[i].orderNum;
            QString url=joint_qrencode_url2(buy_goods.at(i).payWay);
            url.append("/orderQuery?out_trade_no=");
            url.append(buy_goods.at(i).orderNum);
            qDebug()<<"HTTP 2: "<<url;
            manager->get(QNetworkRequest(QUrl(url)));
            buy_goods[i].cnt--;
        }
    }
    mutex.unlock();
}
//http回复
void HttpRequest::replyFinished(QNetworkReply* reply)
{
    QByteArray byte=reply->readAll();
    qDebug()<<byte;
    if(byte.isEmpty()){
        return;
    }
    QJsonObject obj;
    if(jsonHandler->get_json_object(byte,obj)){
        if(jsonHandler->is_contain_key(obj,"tip")){
            QString str=jsonHandler->get_json_value_string(obj,"tip").toUtf8();
            if(jsonHandler->is_contain_key(obj,"status")){
                 QString status=jsonHandler->get_json_value_string(obj,"status");
                 if(!status.compare("0")){
                     emit data(HttpRequest::upload,0);
                 }
            }
            if(jsonHandler->is_contain_key(obj,"surplus")){
                QString surplus=jsonHandler->get_json_value_string(obj,"surplus");
                if(jsonHandler->is_contain_key(obj,"goods_name")){
                    QString goods_name=jsonHandler->get_json_value_string(obj,"goods_name").toUtf8();
                    str.clear();
                    str=QString("补货:\n商品:%1\n最大数量:%2").arg(goods_name,surplus);
                    App::replenishmentTotalNum=surplus.toInt();
                }

            }
           DialogInstance::getInstance()->setText(str,5000);
        }
        if(jsonHandler->is_contain_key(obj,"time")){
            QString time=jsonHandler->get_json_value_string(obj,"time");
            time_t tt = time.toInt();
            qDebug()<<time<<tt;
            int r = stime(&tt);
            if (r){
                qDebug()<<"You don't have permission to change system time.";
            }
            else{
                emit data(HttpRequest::time,0);
            }
        }
        if(jsonHandler->is_contain_key(obj,"card_name")){
            QString card_name=jsonHandler->get_json_value_string(obj,"card_name");
            if(jsonHandler->is_contain_key(obj,"card_integration")){
                 QString card_integration=jsonHandler->get_json_value_string(obj,"card_integration");
                 App::cardName=card_name;
                 App::cardIntegration=card_integration;
                 emit data(card,0);
//                DialogInstance::getInstance()->setText("会员:"+card_name+"\n"+"积分:"+card_integration);
            }
        }

        if(jsonHandler->is_contain_key(obj,"username")){
            QString userName=jsonHandler->get_json_value_string(obj,"username");
            QString pass="";
            if(jsonHandler->is_contain_key(obj,"pass")){
                pass=jsonHandler->get_json_value_string(obj,"pass");
                if(App::user.compare(userName)||App::password.compare(pass)){
                    App::user=userName;
                    App::password=pass;
                    write_File("/home/conf/passwd",userName+":"+pass);
//                    XmlDom xml;
//                    xml.writeXML("/home/conf/conf.xml");
                }
            }
        }

       // "{"box":"box18","pay_way":"card","order":"201808048500519998101529"}"
        if(jsonHandler->is_contain_key(obj,"box")){
            QString orderStr=jsonHandler->get_json_value_string(obj,"order");
            if(order_to_index(orderStr)>=0){
                return;
            }

            QString boxStr=jsonHandler->get_json_value_string(obj,"box");
            boxStr=boxStr.remove("box");

            int payWay=App::buyNo;
            QString payWayStr=jsonHandler->get_json_value_string(obj,"pay_way");
            if(!payWayStr.compare("weixin")){
                payWay=App::buyWeixin;
            }
            else if(!payWayStr.compare("alipay")){
                payWay=App::buyAlipay;
            }
            else if(!payWayStr.compare("card")){
                payWay=App::buyCard;
                if(jsonHandler->is_contain_key(obj,"card_integration")){
                     QString card_integration=jsonHandler->get_json_value_string(obj,"card_integration");
                     App::cardIntegration=card_integration;
                     emit http_card();
                }
            }
            else if(!payWayStr.compare("note")){
                payWay=App::buyMoney;
            }

            enum PutOutStatus putOut=putOut_WAIT;
            if(jsonHandler->is_contain_key(obj,"url")){
                putOut=putOut_NO;
                qrencode=jsonHandler->get_json_value_string(obj,"url");
                emit data(qrcode,payWay);
                if(!timer->isActive()){
                    timer->start(TIME);
                }
            }

            struct buyGoods temp={CNT,payWay,boxStr.toInt(),putOut,orderStr};
            buy_goods.append(temp);
//            qDebug()<<"Add"<<buy_goods.last().orderNum;
            if((payWay==App::buyCard)||(payWay==App::buyMoney)){
                ready_open_box();
            }
        }

        //支付结果
        if(jsonHandler->is_contain_key(obj,"result")){
            QString result=jsonHandler->get_json_value_string(obj,"result");
            if(!result.compare("0")){
                QString reply_order=jsonHandler->get_json_value_string(obj,"reply_order");
                int index=order_to_index(reply_order);
                if(index>=0){
                    buy_goods[index].cnt=-1;
                    if(buy_goods.at(index).status==putOut_NO){
                        buy_goods[index].status=putOut_WAIT;
                        ready_open_box();
                    }
                }
             }
        }
     }
}

//返回出货结果
void HttpRequest::send_Result(int result,QString who)
{
    int index=order_to_index(currentPutOut);
    if((index>=0)&&(index<buy_goods.size())){
        buy_goods[index].status=putOut_YES;
        buy_goods[index].cnt=0;
        QUrl url=joint_qrencode_url3(buy_goods.at(index).payWay);
        QUrlQuery query;
        query.addQueryItem("out_trade_no",currentPutOut);
        query.addQueryItem("code",QString::number(result));
        query.addQueryItem("who",who);
        url.setQuery(query);
        qDebug()<<"HTTP 3: "<<url;
        manager->get(QNetworkRequest(QUrl(url)));
    }else{
        qDebug()<<"no this order";
    }
    currentPutOut.clear();
    isBusy=false;
    ready_open_box();
}
void HttpRequest::send_Result(QString who)
{
    int index=order_to_index(currentPutOut);
    if((index>=0)&&(index<buy_goods.size())){
        buy_goods[index].status=putOut_YES;
        buy_goods[index].cnt=0;
        QUrl url=joint_qrencode_url(buy_goods.at(index).payWay)+"/result";
        QUrlQuery query;
        query.addQueryItem("out_trade_no",currentPutOut);
        //query.addQueryItem("code",QString("%1").arg(App::weight));
        query.addQueryItem("who",who);
        url.setQuery(query);
        qDebug()<<"HTTP 3: "<<url;
        manager->get(QNetworkRequest(QUrl(url)));
    }
    currentPutOut.clear();
    isBusy=false;
    ready_open_box();
}
//发送所有状态
void HttpRequest::send_stauts(const QString &status){
    QUrl url=App::ip+"/home/interaction/replenishment_gzj";
    QUrlQuery query;
    query.addQueryItem("mac_id",App::macId);
    query.addQueryItem("code_num",App::replenishmentQrcode);
    query.addQueryItem("status",status);
    url.setQuery(query);
    qDebug()<<"HTTP 4: "<<url;
    manager->get(QNetworkRequest(QUrl(url)));
}
