#ifndef CONFIGURE_H
#define CONFIGURE_H

//C语言中将变量名转换成字符串使用的是#符号，来看下边两个宏定义：
#define name2str(name) (#name)
#define print_val(x) printf(#x" = %d\n", x)

#include <QString>
#include <stdio.h>

#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>

class Configure
{
public:
    Configure();

    QString http;
    QString macId;
    int goodsNum;
    int payType;
    int macIdType;
    int putoutTimeout;

//    QString strToJson(){
//        char str[2048];
//        memset(str,0,sizeof(str));
//        sprintf(str,"{\"%s\":\"%s\","
//                   "\"%s\":\"%s\","
//                   "\"%s\":%d,"
//                   "\"%s\":%d"
//                   "}",
//        name2str(http),http,
//        name2str(macId),macId,
//        name2str(goodsNum),goodsNum,
//        name2str(payType),payType);
//        return QString(QLatin1String(str));
//    }

//    QString jsonToStr(QString jsonStr){
////        QByteArray ba = str.toLatin1(); //含中文时乱码
////        char*ch=ba.data();  //不可以 str.toLatin1().data()
//        std::string temp = jsonStr.toStdString();
//        const char* str = temp.c_str();

//        int start=-1;
//        int end=-1;

//        int size=strlen(str);
//        for (int i=0;i<size;i++){
//            if(str[i] == '{'){
//                start=i;
//            }else if(str[i] == '}'){
//                end=i;
//            }
//        }
//    }


    QString get_json(){
        QJsonObject json;
        json.insert(name2str(http),     http);
        json.insert(name2str(macId),    macId);
        json.insert(name2str(goodsNum), goodsNum);
        json.insert(name2str(payType),  payType);
        json.insert(name2str(macIdType),macIdType);
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        return byte_array;
    }

    void parse_json(QString jsonStr){
        QJsonParseError error;                                                // 1. 创建 QJsonParseError 对象，用来获取解析结果
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonStr.toUtf8(), &error);// 2. 使用静态函数获取 QJsonDocument 对象
        if (error.error == QJsonParseError::NoError){               //检查json是否有错误
            if (!(jsonDocument.isNull() || jsonDocument.isEmpty())) {//以 QVariant 的格式进行解析
                if (jsonDocument.isObject()){
                    QJsonObject obj = jsonDocument.object();         //开始解析json对象
                    if(obj.contains(name2str(http))){
                        QJsonValue temp = obj.take(name2str(http));
                        if (temp.isString()){
                            http=temp.toVariant().toString();
                        }
                    }
                    if(obj.contains(name2str(macId))){
                        QJsonValue temp = obj.take(name2str(macId));
                        if (temp.isString()){
                            macId=temp.toVariant().toString();
                        }
                    }
                    if (obj.contains(name2str(goodsNum))){
                        QJsonValue temp = obj.take(name2str(goodsNum));
                        if (temp.isDouble()){
                            goodsNum=temp.toVariant().toInt();
                        }
                    }
                    if (obj.contains(name2str(payType))){
                        QJsonValue temp = obj.take(name2str(payType));
                        if (temp.isDouble()){
                            payType=temp.toVariant().toInt();
                        }
                    }
                    if (obj.contains(name2str(macIdType))){
                        QJsonValue temp = obj.take(name2str(macIdType));
                        if (temp.isDouble()){
                            macIdType=temp.toVariant().toInt();
                        }
                    }
                }
            }
        }
    }

};

#endif // CONFIGURE_H
