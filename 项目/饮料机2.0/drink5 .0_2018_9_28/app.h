#ifndef APP_H
#define APP_H

#include <QString>
class QString;

class App
{
public:
    static QString AppName;             //应用程序英文名称
    static QString AppPath;             //应用程序路径

    static int DeskWidth;               //运行环境桌面宽度
    static int DeskHeight;              //运行环境桌面高度

    static int IcoMain;                 //左上角图标
    static int IcoMenu;                 //下来菜单图标
    static int IcoMin;                  //最小化图标
    static int IcoMax;                  //最大化图标
    static int IcoNormal;               //正常图标
    static int IcoClose;                //关闭图标

    static bool SaveLog;                //输出日志文件
    static bool UseStyle;               //启用样式
    static QString StyleName;           //应用程序样式
    static int FontSize;                //应用程序字体大小
    static QString FontName;            //应用程序字体名称

    static QString ServerIP;            //服务器IP
    static int ServerPort;              //服务器端口
    static QString SendFile;            //发送文件
    static int ListenPort;              //监听端口

    static void ReadConfig();           //读取配置文件,在main函数最开始加载程序载入
    static void WriteConfig();          //写入配置文件,在更改配置文件程序关闭时调用

    enum{displayWidget,buyWidget,mobileWidget,settingWidget};
    enum{buyNo,buyMoney,buyMobile,buyWeixin,buyAlipay,buyCard}; 
//    enum{selectDeviceLayout,selectGoodsLayout,goodsPresentLayout,putOutTipLayout,replenishmentLayout,loginLayout};
//    enum{CheckGroupWidget,RadioGroupWidget,PushButtonGroupWidget};

    static QString ip;
    static QString port;
    static QString macId;

    static QString user;
    static QString password;

    static QString card;
    static QString cardName;
    static QString cardIntegration;
    static float weight;

    static int paperMoney;

    static int goodsNum;

    static int selectDeviceNum;
    static int selectGoodsNum_page;
    static int selectDevice_GoodsNum;

    // 静态数组
    static int compensate[6];


    static QString replenishmentQrcode;
    static QString replenishmentGoodsName;
    static int replenishmentTotalNum;

};

#endif // APP_H
