#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>
#include <QDebug>
#include <QJsonArray>

class JsonHandler
{
public:
    JsonHandler();

    /**
      转为json格式
     * @brief get_json_format
     * @param str
     * @return
     */
    static QJsonDocument get_json_format(char *str){
        QByteArray byte(str);
        // 1. 创建 QJsonParseError 对象，用来获取解析结果
        // 2. 使用静态函数获取 QJsonDocument 对象
        // 3. 根据解析结果进行处理
        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(byte, &error);
        if (error.error == QJsonParseError::NoError) {//检查json是否有错误
            qDebug()<<"json ok";
        }else{
            qDebug()<<error.errorString();
        }
        return jsonDocument;
    }
    static QJsonDocument get_json_format(const QByteArray &byte){
        // 1. 创建 QJsonParseError 对象，用来获取解析结果
        // 2. 使用静态函数获取 QJsonDocument 对象
        // 3. 根据解析结果进行处理
        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(byte, &error);
        if (error.error == QJsonParseError::NoError) {//检查json是否有错误
            qDebug()<<"json ok";
        }
        return jsonDocument;
    }
    static QJsonDocument get_json_format(const QString &str){
        QByteArray byte=str.toUtf8();
        // 1. 创建 QJsonParseError 对象，用来获取解析结果
        // 2. 使用静态函数获取 QJsonDocument 对象
        // 3. 根据解析结果进行处理
        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(byte, &error);
        if (error.error == QJsonParseError::NoError) {//检查json是否有错误
            qDebug()<<"json ok";
        }
        return jsonDocument;
    }

    /**
      获取String值
     * @brief get_json_object_value_str
     * @param jsonDocument
     * @param key
     * @param defaultValue
     * @return
     */
    static QString get_json_object_value_str(QJsonDocument jsonDocument,QString key,QString defaultValue=NULL){
        QString value=defaultValue;
        if (!(jsonDocument.isNull() || jsonDocument.isEmpty())){
            if (jsonDocument.isObject()){
                QJsonObject obj =jsonDocument.object();//取得最外层这个大对象
                if (obj.contains(key)) {                //如果包含key,进行取值
                    QJsonValue json_value = obj.take(key);
                    if (json_value.isString()){
                        value=json_value.toVariant().toString();//以 QVariant 的格式进行解析
                    }
                }
            }
        }
        return value;
    }

    static QString get_json_value_string(QJsonObject obj,const QString key,QString defaultValue="NULL"){
        QString value=defaultValue;
        if (obj.contains(key)){//如果包含
            QJsonValue temp = obj.take(key);
            if (temp.isString()){
                value=temp.toVariant().toString();
            }
        }else{
            qDebug()<<"not contains key:"<<key;
        }
        return value;
    }

    static double get_json_value_double(QJsonObject obj,const QString key,double defaultValue=0){
        double value=defaultValue;
        if (obj.contains(key)){
            QJsonValue temp = obj.take(key);
            if (temp.isDouble()){
                value=temp.toVariant().toDouble();
            }
        }else{
            qDebug()<<"not contains key:"<<key;
        }
        return value;
    }

    static int get_json_value_int(QJsonObject obj,const QString key,int defaultValue=0){
        int value=defaultValue;
        if (obj.contains(key)){
            QJsonValue temp = obj.take(key);
            if (temp.isDouble()){
                value=temp.toVariant().toInt();
            }
        }else{
            qDebug()<<"not contains key:"<<key;
        }
        return value;
    }
    /**
     * 数组
     * @brief get_json_object_value_array
     * @param jsonDocument
     * @param key
     * @param defaultValue
     * @return
     */
    static QJsonArray get_json_object_value_array(QJsonDocument jsonDocument,QString key){
        QJsonArray value;
        if (!(jsonDocument.isNull() || jsonDocument.isEmpty())){
            if (jsonDocument.isObject()){
                QJsonObject obj =jsonDocument.object();//取得最外层这个大对象
                if (obj.contains(key)) {               //如果包含key,进行取值
                    QJsonValue json_value = obj.take(key);
                    if (json_value.isArray()){
                        //value=json_value.toVariant().toJsonArray();//以 QVariant 的格式进行解析
                        return json_value.toArray();
                    }
                }
            }
        }
        return value;
    }

    /**
      生成json字符串
     * @brief package_json
     * @param json
     * @return
     */
    static char* package_json(const QJsonObject &json){
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte = document.toJson(QJsonDocument::Compact);
        char *str=byte.data();
        return str;
    }
    static QString get_json_format_string(const QJsonObject &json){
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte = document.toJson(QJsonDocument::Compact);
        QString str(byte);
        return str;
    }

    static char* package_json(QMap<QString,QString> map){
        if(map.isEmpty()){
            return NULL;
        }
        QJsonObject obj;
        for (QMap<QString,QString>::iterator it = map.begin(); it != map.end(); ++it ) {
            obj.insert(it.key(),it.value());//it.key().ascii()
        }

        QJsonDocument document;
        document.setObject(obj);
        QByteArray byte = document.toJson(QJsonDocument::Compact);
        char *p=(char *)malloc(sizeof(char)*byte.size()+1);
        int i=0;
        for(;i<byte.length();i++){
            p[i]=byte.at(i);
        }
        p[i]='\0';
        return p;
    }

    /**
     * 对象形式
     * @brief map_to_json_object
     * @param map
     * @return
     */
    static QByteArray map_to_json_object(QMap<QString,QString> &map){
        if(map.isEmpty()){
            return "";
        }
        QJsonObject obj;
        for (QMap<QString,QString>::iterator it = map.begin(); it != map.end(); ++it ) {
            obj.insert(it.key(),it.value());//it.key().ascii()
        }

        QJsonDocument document;
        document.setObject(obj);
        QByteArray byte = document.toJson(QJsonDocument::Compact);
        return byte;
    }
    /**
     * 数组形式
     * @brief list_to_json_array
     * @param list
     * @return
     */
    static QByteArray list_to_json_array(const QList<QString> &list){
        if(list.isEmpty()){
            return "";
        }
        QJsonArray ar;
        for(int i=0;i<list.size();i++){
            ar.insert(i,list.at(i));
        }
        QJsonDocument document;
        document.setArray(ar);
        QByteArray byte = document.toJson(QJsonDocument::Compact);
        return byte;
    }

//    static char* package_json(QMap<QString,QString> map,char *p){
//        if(map.isEmpty()){
//            return NULL;
//        }
//        QJsonObject obj;
//        for (QMap<QString,QString>::iterator it = map.begin(); it != map.end(); ++it ) {
//                    //it.key().ascii()
//            obj.insert(it.key(),it.value());
//        }

//        QJsonDocument document;
//        document.setObject(obj);
//        QByteArray byte = document.toJson(QJsonDocument::Compact);
//        int i=0;
//        for(;i<byte.length();i++){
//            p[i]=byte.at(i);
//        }
//        p[i]='\0';
//        return p;
//    }

/*
    bool get_json_object_value(QJsonDocument &jsonDocument,QString key,QString &value)
    {
        bool ok=false;
        if (jsonDocument.isObject())
        {
            QJsonObject obj =jsonDocument.object();//取得最外层这个大对象
            if (obj.contains(key))                 //如果包含key,进行取值
            {
                ok=true;
                QJsonValue json_value = obj.take(key);
                if (json_value.isString()){
                    value=json_value.toVariant().toString();
                    printf("str\n");
                }
                else if(json_value.isDouble()){
                    value=QString::number(json_value.toVariant().toDouble());
                    printf("dou\n");
                }
                else if(json_value.isBool()){
                    value=QString::number(json_value.toBool());
                    printf("bool\n");
                }
                else if(json_value.isNull()){
                    value="";
                    printf("null\n");
                }
                else if(json_value.isArray()){
                    QJsonArray jsonArray=json_value.toArray();
                    //value=get_json_array_value(jsonArray);
                }
                else{
                    value="";
                }
            }
            else{
                printf("no key\n");
            }
        }
        return ok;
    }
*/

};

#endif // JSONHANDLER_H
