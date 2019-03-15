#ifndef XMLDOM_H
#define XMLDOM_H

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSize>

//#define name2str(name) (#name)

#define MAX_DISPLAY_REGION_SIZE         (3)

typedef enum{
    PLAY_MODE_UNKNOW        ,
    PLAY_MODE_IMAGE_ONLY    ,
    PLAY_MODE_VIDEO_ONLY     ,
    PLAY_MODE_COMPLEX       ,
}PlayMode;


typedef enum{
    DISPLAY_MODE_VERTICAL,
    DISPLAY_MODE_HORIZONTAL,
}DISPLAY_MODE;

class XmlDom
{
public:
    XmlDom();

    static QString price;
    static int DisplayMode;                                 //横向/竖向显示模式
    static QString ip;                                      //服务器域名
    static QString port;                                    //服务器端口号
    static QString tcp_ip;                                  //tcp服务器ip
    static QString tcp_port;                                //tcp服务器端口号
    static QString id;                                      //设备ID
    static QString image_path;                              //商品图片路径
    static QString video_path;                              //播放视频路径
    static int screen_width;                                //界面屏幕宽度
    static int screen_height;                               //界面屏幕高度
    static int    RegionSize;                               //显示区域数量
    static QSize  RegionInfo[MAX_DISPLAY_REGION_SIZE];      //显示区域信息
    static int    ChnagePlayMode;                           //
    static int    CurrentPlayMode;

    bool initConfigXml();
private:
    QDomElement add_child_domElement(QDomDocument &doc,QDomElement &root,QString key);

    void add_child_domElement_attribute(QDomDocument &doc,QDomElement &root, const QString attr_key,    \
                              const QString attr_value,const QString key,const QString value);

    void add_child_domElement(QDomDocument &doc,QDomElement &root,const QString key,const QString value);

    void save_file(const QString fileName,const QDomDocument &doc);

    bool read_file(const QString fileName,QDomDocument &doc);

    void parserConfig(QDomNode node);
    bool checkConfigXmlVersion(QDomElement root);

    bool readConfigXml(QDomElement root);
    void writeConfigXml(QString fileName);

    bool readConfigXml(QString fileName);

};

#endif // XMLDOM_H
