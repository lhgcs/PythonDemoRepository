/*
 * http请求
 * 请求服务器
 * 2017-06-22
 */

#include "httprequest.h"
#include <QMutex>

#define CNT    40
#define TIME   3000

#include "jsonhandler.h"
#include "util/filehandler.h"
#include "configure/app.h"
#include "subwidget/dialoginstance.h"
#include <QUrlQuery>
#include "control/linuxsocket.h"

//HttpRequest *HttpRequest::mHttpRequest=mHttpRequest=new HttpRequest;
HttpRequest *HttpRequest::mHttpRequest=NULL;

HttpRequest::HttpRequest(QObject *parent):QObject(parent){
    manager = new QNetworkAccessManager(this);//创建一个管理器
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//连接请求结束信号
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slot_timeout()));
    http=App::HTTP;
    mac_id=App::MACID;
    goods.clear();
    isBusy=false;
}

/**
  请求二维码
 * @brief HttpRequest::send_Request_qrcode
 * @param current_payment
 * @param num
 */
void HttpRequest::send_request_qrcode(int currentPayment, int num){
    if(goods.size()<50){
        QString url;
        QMap<QString,QString> map;
    #if DRINK
        if(currentPayment == WEIXIN){
            url=http+"/admin/drinkwx/index";
        }else if(currentPayment == ALIPAY){
            url=http+"/admin/drinkali/index";
        }else{
            printf("1 Request error\n");
            return;
        }
        map.insert("box","box"+QString::number(num));
        get(url,map);
    #elif MYSELF
        if(currentPayment == WEIXIN){
            url=http+"/admin/selfwx/index";
        }else if(currentPayment == ALIPAY){
            url=http+"/admin/selfali/index";
        }else{
            printf("1 Request error\n");
            return;
        }
        map.insert("box","box"+QString::number(num));
        get(url,map);
    #elif JIUHAOCANG
        if(currentPayment == WEIXIN){
            url=http+"/pay/wx/newbottle";
        }else if(currentPayment == ALIPAY){
            url=http+"/pay/ali/newbottleali";
        }else{
            printf("1 Request error\n");
            return;
        }
        map.insert("box","box"+QString::number(num));
        get(url,map);
    #elif NEW_DRINK
        url=http+"/pay/index/index";
        if(currentPayment == WEIXIN){
            map.insert("way","wx");
        }else if(currentPayment == ALIPAY){
            map.insert("way","ali");
        }else{
            printf("1 Request error\n");
            return;
        }
        map.insert("code","103");
        map.insert("mac_id",App::MACID);
        map.insert("box","A"+QString::number(num));
        post(url,map);
    #endif 
    }else{
        DialogInstance::getInstance()->setText("请求过于频繁");
    }
}
/**
  请求支付结果
 * @brief HttpRequest::send_Request
 */
void HttpRequest::send_request()
{
    if(goods.size() > 0){
        static QMutex mutex;//互斥锁  m_lock.lock() 就是申请这个锁，如果被正占用，它只能等待(直到占有者调用了m_lock.unlock()它才能继续)。
        mutex.lock();
        for(int i=0;i<goods.size();i++){
            qDebug()<<"send request"<<goods[i].box
                    <<goods[i].cnt
                    <<goods[i].payWay
                    <<goods[i].order_num;

            if( goods.at(i).cnt > 0 && goods.at(i).status == PUT_OUT_NO){
                goods[i].cnt--;
                QString url;
                QMap<QString,QString> map;

            #if DRINK
                if(goods.at(i).payWay==WEIXIN){
                    url=http+"/admin/drinkwx/orderquery";
                }else if(goods.at(i).payWay==ALIPAY){
                    url=http+"/admin/drinkali/orderquery";
                }else{
                    printf("2 Request error\n");
                    continue;
                }
                map.insert("out_trade_no",goods.at(i).order_num);
                get(url,map);
            #elif MYSELF
                if(goods.at(i).payWay==WEIXIN){
                    url=http+"/admin/selfwx/orderquery;
                }else if(goods.at(i).payWay==ALIPAY){
                    url=http+"/admin/selfali/orderquery;
                }else{
                    printf("2 Request error\n");
                    continue;
                }
                map.insert("out_trade_no",goods.at(i).order_num);
                get(url,map);
            #elif JIUHAOCANG
                if(goods.at(i).payWay==WEIXIN){
                    url=http+"/admin/drinkwx/orderquery";
                }else if(goods.at(i).payWay==ALIPAY){
                    url=http+"/pay/ali/orderQuery";
                }else{
                    printf("2 Request error\n");
                    continue;
                }
                map.insert("out_trade_no",goods.at(i).order_num);
                get(url,map);
            #elif NEW_DRINK
                if(goods.at(i).payWay==WEIXIN){
                    map.insert("way","wx");
                }else if(goods.at(i).payWay==ALIPAY){
                    map.insert("way","ali");
                }else{
                    printf("2 Request error\n");
                    continue;
                }

                url=http+"/pay/index/paycheck";
                map.insert("out_trade_no",goods.at(i).order_num);
                map.insert("code","104");
                post(url,map);
            #endif
            }
        }

        for(QVector<BuyGoods>::iterator it=goods.begin();it!=goods.end();){
            if((it->cnt == 0) || (it->status == PUT_OUT_YES)){
                it=goods.erase(it);//删除元素，返回值指向已删除元素的下一个位置
            }else{
                ++it;              //指向下一个位置
            }
        }
        mutex.unlock();
    }else{
        timer->stop();
    }
}

/**
  返回出货结果
 * @brief HttpRequest::send_result
 * @param result
 * @param who
 * @return
 */
int HttpRequest::send_result(int result,QString who)
{
    if(current_put_out == "reset"){
        return -1;
    }
    int mbox=0;
    int index=order_to_index(current_put_out);
    if((index>=0) && (index<goods.size())){
        mbox=goods.at(index).box;
        if(goods.at(index).payWay==WEIXIN || goods.at(index).payWay==ALIPAY){
                QMap<QString,QString> map;
                QString url;
            #if DRINK
                if(goods.at(index).payWay==WEIXIN){
                    url=http+"/admin/drinkwx/result";
                }else if(goods.at(index).payWay==ALIPAY){
                    url=http+"/admin/drinkali/result";
                }else{
                    printf("3 payment Request error\n");
                }
                map.insert("out_trade_no",current_put_out);
                map.insert("code",QString::number(result));
                map.insert("who",who);
                get(url,map);
            #elif MYSELF
                if(goods.at(index).payWay==WEIXIN){
                    url=http+"/admin/selfwx/result";
                }else if(goods.at(index).payWay==ALIPAY){
                    url=http+"/admin/selfali/result";
                }else{
                    printf("3 payment Request error\n");
                }
                map.insert("out_trade_no",current_put_out);
                map.insert("code",QString::number(result));
                map.insert("who",who);
                get(url,map);
            #elif JIUHAOCANG
                if(goods.at(index).payWay==WEIXIN){
                    url=http+"/pay/wx/newbottleResult";
                }else if(goods.at(index).payWay==ALIPAY){
                    url=http+"/pay/ali/newbottleresult";
                }else{
                    printf("3 payment Request error\n");
                }
                map.insert("out_trade_no",current_put_out);
                map.insert("code",QString::number(result));
                map.insert("who",who);
                get(url,map);
            #elif NEW_DRINK
                url = http+"/pay/index/out";
                if(result == 0){
                    map.insert("status","1");
                }else{
                    map.insert("status","2");
                }
                map.insert("out_trade_no",current_put_out);
                map.insert("code","105");
                map.insert("who",who);
                post(url,map);
            #endif   
        }else if(goods.at(index).payWay==NONE){
            if(result == 0){
                LinuxSocket::putout_ok(current_put_out);
            }else{
                LinuxSocket::putout_failed(current_put_out);
            }
        }
        goods[index].cnt=0;
        goods[index].status=PUT_OUT_YES;
    }else{
        qDebug()<<"HTTP 3: not find this order_num!!!!!!"<<current_put_out;
    }

    current_put_out.clear();
    isBusy=false;
    put_out_next();
    return mbox;
}

//http回复
void HttpRequest::replyFinished(QNetworkReply* reply)
{
    QString byte=reply->readAll();
    qDebug()<<byte;
    if(byte.isEmpty()){
        return;
    }

    QJsonDocument json=JsonHandler::get_json_format(byte);
        if(!(json.isEmpty() || json.isNull())){
            if (json.isObject()){
                QJsonObject obj = json.object();//开始解析json对象

                QString time=JsonHandler::get_json_value_string(obj,"time");
                if(time != "NULL"){
                    time_t tt = time.toInt();
                    qDebug()<<time<<tt;
                    int r = stime(&tt);
                    if (r){
                        qDebug()<<"You don't have permission to change system time.";
                    }
                }
                #if NEW_DRINK

                if(obj.contains("update")){
                    if(JsonHandler::get_json_value_string(obj,"update") == "1"){
                        http_data(getServerData);
                    }
                }

                if(obj.contains("status") && obj.contains("message")&&
                    obj.contains("loginame") && obj.contains("pass")){
                    QString status=JsonHandler::get_json_value_string(obj,"status");
                    if(status == "1"){
                        QString user=JsonHandler::get_json_value_string(obj,"loginame");
                        QString pass=JsonHandler::get_json_value_string(obj,"pass");
                        write_File("/home/conf/passwd.txt",user+":"+pass);
                    }
                }

                if(obj.contains("status") && obj.contains("message")&&
                    obj.contains("min_temp") && obj.contains("max_temp")){
                    QString status=JsonHandler::get_json_value_string(obj,"status");
                    if(status == "1"){
                        QString minStr=JsonHandler::get_json_value_string(obj,"min_temp");
                        QString maxStr=JsonHandler::get_json_value_string(obj,"max_temp");
                        int L=minStr.toInt();
                        int H=maxStr.toInt();
                        if(L>0 && L<100 && H>0 && H<100){
                            http_data(temperature,L,H);
                        }
                    }
                }

                if(obj.contains("status") && obj.contains("message")&&
                    obj.contains("out_trade_no") && obj.contains("codeurl")&&
                    obj.contains("way")){

                    QString status=JsonHandler::get_json_value_string(obj,"status");
                    if(status == "1"){
                        int payWay=NONE;
                        QString payWay_str=JsonHandler::get_json_value_string(obj,"way");
                        if(payWay_str=="wx"){
                            payWay=WEIXIN;
                        }else if(payWay_str=="ali"){
                            payWay=ALIPAY;
                        }else if(payWay_str=="game"){
                            payWay=GAME;
                        }else {
                            return;
                        }

                        QString box_str  =JsonHandler::get_json_value_string(obj,"box");
                        QString url_str  =JsonHandler::get_json_value_string(obj,"codeurl");
                        QString order_str=JsonHandler::get_json_value_string(obj,"out_trade_no");

                        if(url_str == "NULL" || box_str == "NULL" || order_str == "NULL"){
                            return;
                        }

                        emit http_qrencode(payWay,url_str);

                        int box=0;
                        box_str=box_str.mid(1);
                        box=box_str.toInt();

                        add_to_list(PUT_OUT_NO,box,payWay,order_str);
                        timer->start(TIME);
                    }
                 }else if(obj.contains("status") && obj.contains("message")&&
                    obj.contains("out_trade_no")){
                    QString result=JsonHandler::get_json_value_string(obj,"status");
                    QString message=JsonHandler::get_json_value_string(obj,"message");
                    if(result == "1" && message == "success"){
                        QString reply_order=JsonHandler::get_json_value_string(obj,"out_trade_no");
                        if(reply_order != "NULL"){
                            put_out(reply_order);
                        }
                    }
                }else if(obj.contains("status") && obj.contains("message") && obj.contains("good")){
                    QString result=JsonHandler::get_json_value_string(obj,"status");
                    if(result == "1"){
                        QString message=JsonHandler::get_json_value_string(obj,"message");
                        if(message == "the num change success"){
                            http_data(UpLoad);
                            DialogInstance::getInstance()->setText("上传成功");
                        }
                    }else{
                        DialogInstance::getInstance()->setText("上传失败");
                    }
                }
                #else

                if (obj.contains("L")&&obj.contains("H")){
                  QString L_str=JsonHandler::get_json_value_string(obj,"L");
                  QString H_str=JsonHandler::get_json_value_string(obj,"H");
                  if(L_str != "NULL" && H_str != "NULL"){
                      int L=L_str.toInt();
                      int H=H_str.toInt();
                      if(L>0&&L<100&&H>0&&H<100){
                          http_data(temperature,L,H);
                      }
                  }
                }

                QString message=JsonHandler::get_json_value_string(obj,"message");
                if(message != "NULL"){
                    DialogInstance::getInstance()->setText(message);
                }

                message=JsonHandler::get_json_value_string(obj,"tips");
                if(message != "NULL"){
                    DialogInstance::getInstance()->setText(message);
                }

                //发送状态结果  "{"status":0}"
                if (obj.contains("status"))  {//如果包含
                    int res=JsonHandler::get_json_value_int(obj,"status");
                    if(res == 0){
                        http_data(UpLoad);
                        DialogInstance::getInstance()->setText("上传成功");
                    }else{
                        DialogInstance::getInstance()->setText("上传失败");
                    }
                }
                //二维码
                if (obj.contains("box")){
                    int payWay=NONE;
                    QString payWay_str=JsonHandler::get_json_value_string(obj,"pay_way");
                    if(payWay_str=="weixin"){
                        payWay=WEIXIN;
                    }else if(payWay_str=="alipay"){
                        payWay=ALIPAY;
                    }else if(payWay_str=="game"){
                        payWay=GAME;
                    }else {
                        return;
                    }

                    QString url_str  =JsonHandler::get_json_value_string(obj,"url");
                    QString box_str  =JsonHandler::get_json_value_string(obj,"box");
                    QString order_str=JsonHandler::get_json_value_string(obj,"order");

                    if(url_str == "NULL" || box_str == "NULL" || order_str == "NULL"){
                        return;
                    }

                    emit http_qrencode(payWay,url_str);

                    int box=0;
                    box_str=box_str.mid(3);
                    box=box_str.toInt();

                    add_to_list(PUT_OUT_NO,box,payWay,order_str);
                    timer->start(TIME);
                }
                //支付结果
                if (obj.contains("result")){
                    QString result=JsonHandler::get_json_value_string(obj,"result","1");
                    int result2=JsonHandler::get_json_value_int(obj,"result",1);

                    if(result == "0" || result2 == 0){
                        QString reply_order=JsonHandler::get_json_value_string(obj,"reply_order");
                        if(reply_order != "NULL"){
                            put_out(reply_order);
                        }
                     }
                }
            #endif
        }
    }else{
        printf("error\n");
        // 检查错误类型
        //qFatal(error.errorString().toUtf8().constData());
    }
    reply->deleteLater();
    reply=NULL;
}

void HttpRequest::setIsBusy(bool value){
    isBusy = value;
}

void HttpRequest::setCurrent_put_out(const QString &value){
    current_put_out = value;
}

QString HttpRequest::getCurrent_put_out() const{
    return current_put_out;
}

/**
  补货
 * @brief HttpRequest::send_status
 * @param map
 */
void HttpRequest::send_status(QMap<int,int> map){
#if JIUHAOCANG
    QString url=http+"/admin/resource/newbottle_add";
    QJsonObject obj;
    for (QMap<int,int>::iterator it = map.begin(); it != map.end(); ++it ) {
        //obj.insert(QString("box%d").arg(it.key()),QString::number(it.value()));
        obj.insert("box"+QString::number(it.key()),QString::number(it.value()));
    }
    QJsonDocument document;
    document.setObject(obj);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString str=byte_array;
    url+="?status="+str+"&mac_id="+mac_id;
    qDebug()<<"HTTP 4: "<<url;
    manager->get(QNetworkRequest(QUrl(url)));
//    QMap<QString,QString> map1;
//    map1.insert("status",str);
//    get(url,map1);
#elif NEW_DRINK
    QString url=http+"/api/resource/goods_add";

    QJsonArray array;
    int i=0;
    for (QMap<int,int>::iterator it = map.begin(); it != map.end(); ++it ) {
        QJsonObject obj;
        obj.insert("box","A"+QString::number(it.key()));
        obj.insert("store",it.value());
        array.insert(i++,obj);
    }

    QJsonObject json;
    json.insert("content",array);
    json.insert("mac_id",App::MACID);

    QJsonDocument jsDom;
    jsDom.setObject(json);
    QByteArray data=jsDom.toJson(QJsonDocument::Compact);
    post(url,data);
#else
    QString url=http+"/admin/drinkresource/status";
    QJsonObject obj;
    for (QMap<int,int>::iterator it = map.begin(); it != map.end(); ++it ) {
        obj.insert("box"+QString::number(it.key()),QString::number(it.value()));
    }
    QJsonDocument document;
    document.setObject(obj);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString str=byte_array;
    qDebug()<<str;

    url+="?status="+str+"&mac_id="+mac_id;
    qDebug()<<"HTTP 4: "<<url;
    manager->get(QNetworkRequest(QUrl(url)));
#endif
}
/**
  当前温度
 * @brief HttpRequest::send_env
 * @param value
 */
void HttpRequest::send_env(const int &value){
    QString url;
    #if NEW_DRINK
        url = http + "/api/resource/uptemp";
        QMap<QString,QString> map;
        map.insert("code", "109");
        map.insert("temp", QString::number(value));
        map.insert("mac_id", App::MACID);
        post(url,map);
    #elif JIUHAOCANG
    #else
        QJsonObject json;
        json.insert("temp", QString::number(value));
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        QString str=byte_array;

        QMap<QString,QString> map;
        map.insert("env", str);

        url=http+"/admin/drinkresource/monitor";
        get(url,map);
    #endif
}

/**
  获取服务器时间
 * @brief send_time
 */
void HttpRequest::get_time(){
    QString url;
    #if NEW_DRINK
    url = http+"/api/resource/timecheck";
    #else
    url=http+"/time.php";
    #endif
    get(url);
}
/**
 * 获取温度范围
 * @brief HttpRequest::get_temp
 */
void HttpRequest::get_temp(){
    #if NEW_DRINK
        QString url = http + "/api/resource/temp";
        QMap<QString,QString> map;
        map.insert("code", "108");
        map.insert("mac_id", App::MACID);
        post(url,map);
    #endif
}
/**
  登录
 * @brief HttpRequest::get_user
 */
void HttpRequest::get_user(){
    #if NEW_DRINK
        QString url = http + "/api/resource/getpass";
        QMap<QString,QString> map;
        map.insert("code", "110");
        map.insert("mac_id", App::MACID);
        post(url,map);
    #endif
}
/**
  在线
 * @brief HttpRequest::send_onlide
 */
void HttpRequest::send_onlide(){
    #if NEW_DRINK
        QString url = http + "/api/resource/is_online";
        QMap<QString,QString> map;
        map.insert("mac_id", App::MACID);
        post(url,map);
    #endif
}


