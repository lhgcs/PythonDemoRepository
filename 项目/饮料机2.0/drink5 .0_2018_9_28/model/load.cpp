#include "load.h"
#include <QTimer>
#include "util/filehandler.h"
#include "model/httprequest.h"
#include "model/goodinfo.h"
#include "model/tablegoodinfo.h"
#include "model/httpdownfilethread.h"
#include "jsonhandler.h"

Load::Load(QWidget *parent):QThread(parent){
}

int Load::getDownFileNum() const
{
    return downFileNum;
}

int Load::parse_goods_info(const QJsonArray &jsonArray){
    int downFileNum=0;
    if (jsonArray.size()>0){
        TableGoodInfo::getInstence()->start_transaction();
        for(int j=0;j<jsonArray.size();j++){
            QJsonValue arrayValue = jsonArray.at(j);
            if(arrayValue.isObject()){
                QJsonObject obj = arrayValue.toObject();
                if( obj.contains("box")    && obj.contains("name")  &&
                    obj.contains("pic_url")&& obj.contains("price") &&
                    obj.contains("store")  && obj.contains("motonum")){

                    QString box = get_str_by_object(obj,"box");
                    if(box.startsWith("A")){
                        box=box.mid(1);
                    }

                    int id=box.toInt();
                    if(id<=0){
                        continue;
                    }

                    GoodInfo info;
                    TableGoodInfo::getInstence()->table_select_all(App::goodsTableName,id,info);
                    QString timeStamp=info.getTimeStamp();

                    info.setId(id);
                    info.setName(get_str_by_object(obj,"name"));
                    info.setPrice(get_str_by_object(obj,"price"));
                    info.setStock(get_int_by_object(obj,"store"));
                    info.setTimeStamp(get_str_by_object(obj,"utime"));
                    info.setImageUrl(get_str_by_object(obj,"pic_url"));
                    info.setMotor(get_int_by_object(obj,"motonum"));

                    TableGoodInfo::getInstence()->table_replace(App::goodsTableName,info);
                    qDebug()<<info.getId()<<info.getMotor()<<info.getPrice()<<info.getStock()<<info.getTimeStamp()<<info.getImageUrl();

                    if(!info.getImageUrl().isEmpty()){
                        QString fileName=IMAGE_PATH+"/"+QString::number(id);
                        bool flag=false;
                        if(info.getTimeStamp() != timeStamp){//时间戳不相等
                            flag=true;
                            qDebug()<<"time not equal:"<<timeStamp<<info.getTimeStamp();
                        }else{
                            QFile file(fileName);
                            if(file.exists()){           //图片存在
                                QPixmap pix;
                                if(!pix.load(fileName)){ //图片能加载
                                    flag=true;
                                    qDebug()<<"image is error";
                                }
                            }else{
                                flag=true;
                                qDebug()<<"file not exist";
                            }
                        }
                        if(flag){
                            downFileNum++;
                            HttpDownFileThread::dowm_file(info.getImageUrl(),fileName,id,10000);
                        }
                    }
                }
            }
        }
        TableGoodInfo::getInstence()->start_commit();
    }
    return downFileNum;
}

int Load::parse_goods_info_old(const QJsonDocument &jsonDocument){
    int downFileNum=0;

    TableGoodInfo::getInstence()->start_transaction();
    #if JIUHAOCANG
    QJsonArray jsonArray=jsonDocument.array();
    for(int j=0;j<jsonArray.size();j++){
        QJsonValue arrayValue = jsonArray.at(j);
        if(arrayValue.isObject()){
            QJsonObject obj = arrayValue.toObject();
            if( obj.contains("box")  && obj.contains("price")&&
                obj.contains("count")&& obj.contains("name") &&
                obj.contains("ctime")&& obj.contains("url")){

                QString box = get_str_by_object(obj,"box");
                if(box.startsWith("box")){
                    box=box.mid(3);
                }

                int id=box.toInt();
                if(id<=0){
                    continue;
                }

                GoodInfo info;
                TableGoodInfo::getInstence()->table_select_all(App::goodsTableName,id,info);
                QString timeStamp=info.getTimeStamp();

                info.setId(id);
                info.setName(get_str_by_object(obj,"name"));
                info.setPrice(get_str_by_object(obj,"price"));
                info.setStock(get_str_by_object(obj,"count").toInt());
                info.setTimeStamp(get_str_by_object(obj,"ctime"));
                info.setImageUrl(get_str_by_object(obj,"url"));

                TableGoodInfo::getInstence()->table_replace(App::goodsTableName,info);
                qDebug()<<info.getId()<<info.getPrice()<<info.getStock()<<info.getTimeStamp()<<info.getImageUrl();

                if(!info.getImageUrl().isEmpty()){
                    QString fileName=IMAGE_PATH+"/"+QString::number(id);
                    bool flag=false;
                    if(info.getTimeStamp() != timeStamp){//时间戳不相等
                        flag=true;
                        qDebug()<<"time not equal:"<<timeStamp<<info.getTimeStamp();
                    }else{
                        QFile file(fileName);
                        if(file.exists()){      //图片存在
                            QPixmap pix;
                            if(!pix.load(fileName)){ //图片能加载
                                flag=true;
                                qDebug()<<"image is error";
                            }
                        }else{
                            flag=true;
                            qDebug()<<"file not exist";
                        }
                    }
                    if(flag){
                        downFileNum++;
                        HttpDownFileThread::dowm_file(info.getImageUrl(),fileName,id,10000);
                    }
                }

            }else if( obj.contains("username")&&obj.contains("pass")){
                QString user=get_str_by_object(obj,"username");
                QString pass=get_str_by_object(obj,"pass");
                write_File("/home/conf/passwd.txt",user+":"+pass);
            }

        }
    }
    #else
    for(int i=1;i<=App::goodsTotal;i++){
        QString key="box"+QString::number(i);
        QJsonArray jsonArray=get_json_object_value_array(jsonDocument,key);
        if(!jsonArray.isEmpty()){
            int size = jsonArray.size();
            if(size >=4){
                GoodInfo info;
                TableGoodInfo::getInstence()->table_select_all(App::goodsTableName,i,info);
                QString timeStamp=info.getTimeStamp();

                info.setId(i);
                info.setName("000");
                info.setPrice(jsonArray.at(0).toString());
                info.setStock(jsonArray.at(1).toString().toInt());
                info.setTimeStamp(jsonArray.at(2).toString());
                info.setImageUrl(jsonArray.at(3).toString());
                TableGoodInfo::getInstence()->table_replace(App::goodsTableName,info);
                qDebug()<<info.getId()<<info.getPrice()<<info.getStock()<<info.getTimeStamp()<<info.getImageUrl();

                if(!info.getImageUrl().isEmpty()){
                    QString fileName=IMAGE_PATH+"/"+QString::number(i);
                    bool flag=false;
                    if(info.getTimeStamp() != timeStamp){//时间戳不相等
                        flag=true;
                    }else{
                        QFile file(fileName);
                        if(file.exists()){      //图片存在
                            QPixmap pix;
                            if(!pix.load(fileName)){ //图片能加载
                                flag=true;
                            }
                        }else{
                            flag=true;
                        }
                    }
                    if(flag){
                        downFileNum++;
                        HttpDownFileThread::dowm_file(info.getImageUrl(),fileName,i,10000);
                    }
                }
            }
        }
    }

    QString key="video";
    QJsonArray jsonArray=get_json_object_value_array(jsonDocument,key);
    if(!jsonArray.isEmpty()){
        int size = jsonArray.size();
        if(size >=2){
            QString url=jsonArray.at(1).toString();
            qDebug()<<url;
            bool flag=false;
            QString fileName;
            if(!url.isEmpty() && !url.endsWith("/")){
                int index=url.lastIndexOf("/");
                fileName=VIDEO_PATH+"/"+url.mid(index+1);
                qDebug()<<"MP4:"<<fileName;
                QFile file(fileName);
                if(!file.exists()){
                    QDir dir(VIDEO_PATH);
                    dir.setFilter(QDir::Files);
                    int fileCount = dir.count();
                    for (int i = 0; i < fileCount; i++){
                        dir.remove(dir[i]);
                    }
                    flag=true;
                }
            }
            if(flag){
                downFileNum++;
                HttpDownFileThread::dowm_file(url,fileName,0,180000);
            }
        }
    }
    #endif
    TableGoodInfo::getInstence()->start_commit();
    return downFileNum;
}

int Load::parse_video_info(const QJsonArray &jsonArray){
    int downFileNum=0;
    if (jsonArray.size()>0){

        QDir dir(VIDEO_PATH);
        dir.setFilter(QDir::Files);
        QStringList videoList;
        for (uint i = 0; i < dir.count(); i++){
            videoList<<dir[i];
            qDebug()<<"old:"<<dir[i];
        }

        for(int j=0;j<jsonArray.size();j++){
            QJsonValue arrayValue = jsonArray.at(j);
            if(arrayValue.isObject()){
                QJsonObject obj = arrayValue.toObject();
                if(obj.contains("v_url")){
                    QString url = get_str_by_object(obj,"v_url");
                    qDebug()<<url;

                    if(url != "0" && !url.isEmpty() && !url.endsWith("/")){
                        bool f=false;
                        int index=url.lastIndexOf("/");
                        QString name=url.mid(index+1);
                        QString fileName=VIDEO_PATH+"/"+name;
                        qDebug()<<"MP4:"<<fileName;

                        f = true;
                        for(int i=0;i<videoList.size();i++){
                            if(videoList.at(i) == name){
                                videoList.removeAt(i);
                                qDebug()<<"exist:"<<fileName;
                                f = false;
                                break;
                            }
                        }
                        if(f == true){
                            downFileNum++;
                            qDebug()<<"dowm:";
                            qDebug()<<"dowm:"<<url<<fileName;
                            HttpDownFileThread::dowm_file(url,fileName,-1,180000);
                        }
                    }
                }
            }
        }
        for(int i=0;i<videoList.size();i++){
            dir.remove(videoList.at(i));
        }
    }
    return downFileNum;
}

//获取商品链接
int Load::load_goods_info(){
    int downFileNum=0;

    #if NEW_DRINK
    QString url=App::HTTP+"/api/resource/get_info";

    QMap<QString,QString> map;
    map.insert("code","101");
    map.insert("mac_id",App::MACID);
    QByteArray json=JsonHandler::map_to_json_object(map);

    QByteArray data=HttpRequest::get_net_source_post(url,json);
    if(data.isEmpty()){
        qDebug()<<"load goods info: return NULL";
        return -1;
    }

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &error);
    if (error.error == QJsonParseError::NoError) {//检查json是否有错误
        qDebug()<<"json ok";
        if (!(jsonDocument.isNull() || jsonDocument.isEmpty())){
            if(jsonDocument.isObject()){
                QString code = JsonHandler::get_json_object_value_str(jsonDocument,"status",NULL);
                if(code == "1"){
                    QJsonArray jsonArray = JsonHandler::get_json_object_value_array(jsonDocument,"data");
                    downFileNum = parse_goods_info(jsonArray);
                }else{
                    Toast::getInstance()->set_text("数据错误");
                }
            }
        }
    }

    #elif JIUHAOCANG

    QString url=App::HTTP+"/admin/resource/newbottleResource?mac_id="+App::MACID;
    QString str=HttpRequest::get_net_source(url);
    if(str.isEmpty()){
        qDebug()<<"load goods info: return NULL";
        return -1;
    }

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(str.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError) {//检查json是否有错误
        qDebug()<<"json ok";
        if (!(jsonDocument.isNull() || jsonDocument.isEmpty())){
            if(jsonDocument.isArray()){
                downFileNum=parse_goods_info_old(jsonDocument);
            }
        }
    }

    #else

    QString url=App::HTTP+"/admin/drinkresource/index?mac_id="+App::MACID;
    QString str=HttpRequest::get_net_source(url);
    if(str.isEmpty()){
        qDebug()<<"load goods info: return NULL";
        return -1;
    }

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(str.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError) {//检查json是否有错误
        qDebug()<<"json ok";
        if (!(jsonDocument.isNull() || jsonDocument.isEmpty())){
            downFileNum=parse_goods_info_old(jsonDocument);
        }
    }

    #endif

    return downFileNum;
}


int Load::load_video_info(){
    int downFileNum=0;

    #if JIUHAOCANG
    QString url=App::HTTP+"/admin/resource/newvideo?mac_id="+App::MACID;
    qDebug()<<"load video info: "<<url;

    QString str=HttpRequest::get_net_source(url);
    if(str.isEmpty()){
        qDebug()<<"load video info: return NULL";
        return -1;
    }

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(str.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError) {//检查json是否有错误
        qDebug()<<"json ok";
        if (!(jsonDocument.isNull() || jsonDocument.isEmpty())){
            if(jsonDocument.isArray()){
                QJsonArray jsonArray=jsonDocument.array();

                QDir dir(VIDEO_PATH);
                dir.setFilter(QDir::Files);
                QStringList videoList;
                for (int i = 0; i < dir.count(); i++){
                    videoList<<dir[i];
                    qDebug()<<"old:"<<dir[i];
                }

                for(int i=0;i<jsonArray.size();i++){
                    QJsonValue arrayValue = jsonArray.at(i);
                    if(arrayValue.isObject()){
                        QJsonObject obj = arrayValue.toObject();
                        if( obj.contains("type")&&
                            obj.contains("id")&&
                            obj.contains("url")){

                            QString type = get_str_by_object(obj,"type");
                            if(type != "video"){
                                continue;
                            }

                            QString url=get_str_by_object(obj,"url");
                            qDebug()<<url;

                            if(url != "0" && !url.isEmpty() && !url.endsWith("/")){
                                bool f=false;
                                int index=url.lastIndexOf("/");
                                QString name=url.mid(index+1);
                                QString fileName=VIDEO_PATH+"/"+name;
                                qDebug()<<"MP4:"<<fileName;

                                f = true;
                                for(int i=0;i<videoList.size();i++){
                                    if(videoList.at(i) == name){
                                        videoList.removeAt(i);
                                        qDebug()<<"exist:"<<fileName;
                                        f = false;
                                        break;
                                    }
                                }
                                if(f == true){
                                    downFileNum++;
                                    qDebug()<<"dowm:";
                                    qDebug()<<"dowm:"<<url<<fileName;
                                    HttpDownFileThread::dowm_file(url,fileName,-1,180000);
                                }
                            }

                        }
                    }
                }

                for(int i=0;i<videoList.size();i++){
                    dir.remove(videoList.at(i));
                }

            }
        }
    }
    #elif NEW_DRINK
        QString url=App::HTTP+"/api/resource/get_video";

        QMap<QString,QString> map;
        map.insert("code","107");
        map.insert("mac_id",App::MACID);

        QByteArray json=JsonHandler::map_to_json_object(map);
        QByteArray data=HttpRequest::get_net_source_post(url,json);
        if(data.isEmpty()){
            qDebug()<<"load video info: return NULL";
            return -1;
        }

        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &error);
        if (error.error == QJsonParseError::NoError) {//检查json是否有错误
            qDebug()<<"json ok";
            if (!(jsonDocument.isNull() || jsonDocument.isEmpty())){
                if(jsonDocument.isObject()){
                    QString code = JsonHandler::get_json_object_value_str(jsonDocument,"status",NULL);
                    if(code == "1"){
                        QJsonArray jsonArray = JsonHandler::get_json_object_value_array(jsonDocument,"data");
                        downFileNum = parse_video_info(jsonArray);
                    }else{
                        Toast::getInstance()->set_text("数据错误");
                    }
                }
            }
        }
    #endif
    return downFileNum;
}


