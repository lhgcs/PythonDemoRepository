#ifndef LOAD_H
#define LOAD_H

#include <QCoreApplication>
#include <QTime>
#include <QDebug>
#include <QPixmap>
#include <QThread>
#include <QVector>

#include "Loading/jsonhandler.h"
#include "Sqlite/sqlitegoods.h"
#include "app.h"
#include "network/sysstart.h"
#include "network/networkmanager.h"
#include "subwidget/dialoginstance.h"
#include "filehandler.h"
#include "toast.h"
#include "httprequest.h"

class Load:public QThread
{
    Q_OBJECT
public:
    Load(QObject *parent=0);
    ~Load(){
        delete jsonHandler;
    }
    void set_sqliteGoods(SqliteGoods *_sqliteGoods){
        sqliteGoods=_sqliteGoods;
    }

    LoadingpProgess *mProgess;
    SysStart *mSystart;
    NetWorkManager *NetWork;

    void setSysStartHanlde(SysStart *sysstart){
        mSystart = sysstart;
        mProgess = sysstart->Progess;
    }

signals:
    void finish(bool value);
private:
    SqliteGoods *sqliteGoods;
    JsonHandler *jsonHandler;
    QJsonArray JsonArrayInfo;

    bool parse_goods_info(){
        bool res=true;
        for(int i=0; i<JsonArrayInfo.size();i++) {
            QJsonObject json =JsonArrayInfo.at(i).toObject();
            if((jsonHandler->is_contain_key(json,"box"))&&
                (jsonHandler->is_contain_key(json,"price"))&&
                (jsonHandler->is_contain_key(json,"count"))&&
                (jsonHandler->is_contain_key(json,"ctime"))&&
                (jsonHandler->is_contain_key(json,"url"))
              ){
                res&=insert_goods_record(json);
                if(mSystart){
                    if(mSystart->Progess){
                        mSystart->Progess->setProgessValue(i+1);
                    }
                }
            }
            else if((jsonHandler->is_contain_key(json,"username"))&&
                 (jsonHandler->is_contain_key(json,"pass"))
               ){
                QString user=json.value("username").toString();
                QString pass=json.value("pass").toString();
                if(user.isEmpty()||pass.isEmpty()){
                }
                else{
                    App::user=user;
                    App::password=pass;
                    write_File("/home/conf/passwd.txt",user+":"+pass);
//                    XmlDom xml;
//                    xml.writeXML("/home/conf/conf.xml");
                }
                qDebug()<<user<<pass;
             }
        }
//        while(!JsonArrayInfo.isEmpty()) {
//            JsonArrayInfo.removeLast();
//        }
        return res;
    }
    bool insert_goods_record(const QJsonObject &json){
        if(!sqliteGoods){
            qDebug()<<"NULL";
            return false;
        }
        bool res=true;
        QString id=json.value("box").toString();
        id=id.remove("box");
        QString name=json.value("name").toString();
        QString price=json.value("price").toString();
        QString count=json.value("count").toString();
        QString ctime=json.value("ctime").toString();
        QString url=json.value("url").toString();
        bool img=true;
        qDebug()<<id<<name<<price<<count<<ctime<<url;
        if(is_load_image(id,ctime)){
            img=get_image(url,"/home/image/"+id);//111
        }
        if(img){
            res&=sqliteGoods->table_insert_or_update(id,name,price,count,ctime,url);
        }
        return res&img;
    }

    bool is_load_image(const QString &id,const QString &time){
        QString str=sqliteGoods->get_table_time_data(id);
        if(!str.compare(time)){
            QPixmap pix;
            if(pix.load("/home/image/"+id)){//111
                return false;
            }
        }
        return true;
    }

    bool get_good_info(const QString &url){
        int count=0;
        while(count<5){
            if(load_goods_info(url)){
                return true;
            }
            count++;
        }
        return false;
    }
    bool get_image(const QString &imgUrl,const QString fileName){
        if(!imgUrl.isEmpty()){
            int count=0;
            while(count<3){
                if(load_image(imgUrl,fileName)){
                    return true;
                }
                count++;
            }
        }
        return false;
    }
    void mysleep(unsigned int msec){
        QTime dieTime = QTime::currentTime().addMSecs(msec);           //以毫秒为单位的延时
        while( QTime::currentTime() < dieTime ){
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);//延时一段时间不阻塞主线程
        }
    }

    bool load_goods_info(const QString &url);
    static bool load_image(const QString &url, const QString &fileName);
    bool load_video(const QString &url,const QString &fileName);
    bool load_user_passwd(const QString &url,QString &user,QString &pass,QString &imgUrl);

public:
    void get_backgroup_iomage(){
        QString url=App::ip+"/admin/api/gridbackimg?mac_id="+App::macId;
        qDebug()<<"load get_backgroup_iomage info: "<<url;
        QByteArray byte;
        if(((HttpRequest::get_net_source(url,byte)==false) || (byte.isEmpty()))) {
            qDebug()<<"load get_backgroup_iomage info faill";
        }
        else{
            qDebug()<<"load get_backgroup_iomage info ok";
            QJsonObject obj;
            if(jsonHandler->get_json_object(byte,obj)){
                QString imgUrl = jsonHandler->get_json_value_string(obj,"pic_url");
                if(!imgUrl.isEmpty()){
                //QString str=App::ip+"/background/pic.png";
                    qDebug()<<imgUrl;
                    if(load_image(imgUrl,"/home/ico/wine")){
                        return;
                    }
                }
            }
        }
        Toast::getInstance()->set_text("获取背景图片失败");
    }

public:
    bool loading(){
        QString url=App::ip+"/admin/resource/gridResource?mac_id="+App::macId;
        if(!get_good_info(url)){
            qDebug()<<"load goods info failed";
            return false;
        }
        else{
            return parse_goods_info();
        }
    }
    void run(){
        NetWork->setSystartHandel(mSystart);
        int cnt=0;
        while(NetWork->checkNetWork()&&((cnt++)<20)) {
            sleep(5);
        }
        if(cnt>=20){
            DialogInstance::getInstance()->setText("联网失败");
            emit finish(false);
            return;
        }
        mSystart->Progess->setLoadingItemInfo("开始下载数据");
        cnt=0;
        while (cnt<5) {
            cnt++;
            mSystart->Progess->setProgessRange(0,App::deviceNum*App::oneDeviceGoodsNum);
            if(loading()){
                break;
            }
        }

            Load::get_backgroup_iomage();
        if(cnt<5){
            mSystart->Progess->setLoadingItemInfo("下载数据成功");
            emit finish(true);  //正常启动后关闭窗口
        }
        else{
            mSystart->Progess->setLoadingItemInfo("下载数据失败");
            emit finish(false);
        }
    }
};

#endif // LOAD_H
