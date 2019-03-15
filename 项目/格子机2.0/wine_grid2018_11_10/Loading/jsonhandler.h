#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>
#include <QJsonArray>

class JsonHandler
{
public:
    JsonHandler();

public:
    static bool string_to_json(const QString &str,QJsonDocument &jsonDocument){
        qDebug()<<str;
        QJsonParseError error;
        jsonDocument = QJsonDocument::fromJson(str.toUtf8(),&error);
        if(error.error == QJsonParseError::NoError){
            qDebug()<<"json ok";
            if (!(jsonDocument.isNull() || jsonDocument.isEmpty())){
                return true;
            }
            else{
                qDebug()<<"json NULL";
            }
        }
        else{
            qDebug()<<"json error: "<<error.error;
        }
        return false;
    }

    static bool byteArray_to_json(const QByteArray &byte,QJsonDocument &jsonDocument){
        QJsonParseError error;
        jsonDocument = QJsonDocument::fromJson(byte,&error);
        if(error.error == QJsonParseError::NoError){
            qDebug()<<"json ok";
            if (!(jsonDocument.isNull() || jsonDocument.isEmpty())){
                return true;
            }
            else{
                qDebug()<<"json is NULL";
            }
        }
        else{
            qDebug()<<"json error: "<<error.error;
        }
        return false;
    }

    static bool get_json_value_bool(QJsonValue value,bool defaultValue = false){
        if(value.type()==QJsonValue::Bool){
            return value.toBool();
        }
        return defaultValue;
    }

    int get_json_value_int(QJsonValue value){
        if(value.type()==QJsonValue::Double){
            return value.toInt();
        }
        return -1;
    }

    double get_json_value_double(QJsonValue value){
        if(value.type()==QJsonValue::Double){
            return value.toDouble();
        }
        return -1;
    }

    bool get_json_array(QJsonValue &value){
        if(value.type()==QJsonValue::Array){      // JSON 文档为数组  包含一个数组
          // QJsonArray jsonArray=value.toArray();  // 转化为数组
            return true;
        }
        return false;
    }

    static bool get_json_object(QJsonDocument &jsonDocument,QString key,QJsonValue &json_value)
    {
        if(jsonDocument.isObject()){               //一个对象
            QJsonObject obj =jsonDocument.object();//取得最外层这个大对象
            if (obj.contains(key)){                //如果包含key,进行取值
                json_value = obj.take(key);
                return true;
            }
            else{
                printf("no key\n");
            }
        }
        return false;
    }

    static QString get_json_format_string(const QJsonObject &json){
           QJsonDocument document;
           document.setObject(json);
           QByteArray byte = document.toJson(QJsonDocument::Compact);
           QString str(byte);
           return str;
       }


public:

    bool get_json_object(const QByteArray &byte,QJsonObject &obj)
    {
        QJsonDocument jsonDocument;
        if(byteArray_to_json(byte,jsonDocument)){
            if(jsonDocument.isObject()){   //一个对象
                obj =jsonDocument.object();//取得最外层这个大对象
                return true;
            }
        }
        return false;
    }

    bool get_json_object(const QByteArray &byte,QJsonArray &arr)
    {
        QJsonDocument jsonDocument;
        if(byteArray_to_json(byte,jsonDocument)){//QJsonDocument::fromJson(byte_array).array();
            if(jsonDocument.isArray()){          //一个对象
                arr=jsonDocument.array();        // QByteArray转换成QJsonArray
                return true;
            }
        }

        return false;
    }

    bool get_json_key_value(QJsonObject &obj,const QString &key,QJsonValue &json_value)
    {
        if (obj.contains(key)){
            json_value = obj.take(key);
            return true;
        }
        else{
            printf("no key\n");
        }
        return false;
    }

    bool is_contain_key(QJsonObject &obj,const QString key){
        if (obj.contains(key)){
            return true;
        }
        return false;
    }

    QString get_json_value_string(QJsonValue value){
        if(value.type()==QJsonValue::String){
            return value.toString();
        }
        else{
            qDebug()<<"not a string";
        }
        return "";
    }

    QString get_json_value_string(QJsonObject &obj,QString key){
        if (obj.contains(key)){
            QJsonValue temp = obj.take(key);
            if (temp.isString()){
                return temp.toString();
            }
        }
        return "";
    }

    int get_json_value_double(QJsonObject &obj,QString key){
        if (obj.contains(key)){
            QJsonValue temp = obj.take(key);
            if (temp.isDouble()){
                return temp.toDouble();
            }
        }
        return -1;
    }



    static void json_array_add(QJsonArray &array,const QJsonObject &obj){    //构建 Json 数组
        array.append(obj);
    }
    static void json_object_add(QJsonObject &obj,const QString &key,const QString &value){//构建 Json 对象
        obj.insert(key, QJsonValue(value));
    }
    static QString package_json_object(QJsonObject &json){  //构建 Json 文档
        QJsonDocument document;
        document.setObject(json);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);
        QString strJson=byteArray;
        return strJson;
    }
    static QString package_json_array(QJsonArray &json){  //构建 Json 文档
        QJsonDocument document;
        document.setArray(json);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);
        QString strJson=byteArray;
        return strJson;
    }
};

#endif // JSONHANDLER_H
