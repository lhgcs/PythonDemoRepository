#include "load.h"
#include <QFile>
#include "httprequest.h"
#include "filehandler.h"

Load::Load(QObject *parent):QThread(parent){
    jsonHandler=new JsonHandler();
    create_Dir("/home/image");
    create_Dir("/home/video");
    NetWork = new  NetWorkManager();
    mSystart=NULL;
}

//获取商品链接
bool Load::load_goods_info(const QString &url)
{
    qDebug()<<"load goods info: "<<url;
    QByteArray byte;
    if(((HttpRequest::get_net_source(url,byte)==false) || (byte.isEmpty()))) {
        qDebug()<<"load goods info faill";
        return false;
    }
    else{
        qDebug()<<"load goods info ok";
        return jsonHandler->get_json_object(byte,JsonArrayInfo);
    }
}

//下载商品图片
bool Load::load_image(const QString &url,const QString &fileName)
{
    qDebug()<<"load goods image: "<<url;
    QByteArray byte;
    if(((HttpRequest::get_net_source(url,byte)==false) || (byte.isEmpty()))) {
        return false;
    }
    else{
        QPixmap pix;
        if(pix.loadFromData(byte)){
            int len=write_File_byte(fileName,byte);
            if(len>0){
                qDebug()<<"save ok";
                return true;
            }
            else{
                qDebug()<<"save failed";
            }
        }
    }
    return false;
}

bool Load::load_video(const QString &url,const QString &fileName)
{
    qDebug()<<"load video: "<<url;
    QByteArray byte;
    if(((HttpRequest::get_net_source(url,byte)==false) || (byte.isEmpty()))) {
        return false;
    }
    else{
        int len=write_File_byte(fileName,byte);
        if(len>0){
            qDebug()<<"save ok";
            return true;
        }
        else{
            qDebug()<<"save failed";
        }
    }
    return false;
}

bool Load::load_user_passwd(const QString &url,QString &user,QString &pass,QString &imgUrl)
{
    user="";
    pass="";
    imgUrl="";
    QByteArray byte;
    if(((HttpRequest::get_net_source(url,byte)==false) || (byte.isEmpty()))) {
        return false;
    }
    else{
        QJsonObject obj;
        if(jsonHandler->get_json_object(byte,obj)){
            user=jsonHandler->get_json_value_string(obj,"user");
            pass=jsonHandler->get_json_value_string(obj,"pass");
            imgUrl=jsonHandler->get_json_value_string(obj,"screen_url");
            //if((!user.isEmpty())&&(!pass.isEmpty())&&(!imgUrl.isEmpty())){
                return true;
            //}
        }
    }
    return false;
}
