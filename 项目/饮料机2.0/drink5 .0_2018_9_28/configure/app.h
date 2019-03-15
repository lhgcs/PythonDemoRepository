#ifndef APP_H
#define APP_H

#include <QString>
#include <QApplication>
#include <QDesktopWidget>
#include <QFontDatabase>
#include <QFile>
#include <QDir>
#include <QMatrix>
#include <QSplashScreen>

#include "util/firmwaredownload.h"
#include "util/filehandler.h"

#include "unistd.h"

#include <QDebug>

#define VIDEO_PATH  QString("/home/video")
#define IMAGE_PATH  QString("/home/image")

//调试输出
#define MY_DEBUG   1

//4G模块
#define YUGA       0     //域格
#define QUECT      1     //移远

//项目
#define MYSELF     0
#define DRINK      0
//三信
#define SANXIN     0
//福袋机
#define ZHONGXIN   0
//酒号仓
#define JIUHAOCANG 0
//饮料机新接口
#define NEW_DRINK  1


class App
{
public:
    App();

    static int screenWidget;
    static int screenHeight;
    static void screen_default(){
        screenWidget=QApplication::desktop()->width();
        screenHeight=QApplication::desktop()->height();
    }

    static QString HTTP;
    static QString MACID;

    static QString dataBaseName;
    static QString goodsTableName;

    static int payType;
    static int goodsTotal;

    /**
      系统变量
     * @brief get_system_confidure
     */
    static void get_system_confidure(){
        HTTP=read_File("/home/conf/http.txt");
        MACID=read_File("/home/conf/mac_id.txt");
    }

    /**
      设置字体
     * @brief set_font
     * @param a
     */
    static void set_font(QApplication &a){
        QDir dir("/home/font");
        if(dir.exists()){
            QStringList stringFilter;
            stringFilter<<"*.ttf"<<"*.ttc";
            QStringList fileList = dir.entryList(stringFilter);
            if(fileList.size()>0){
                //将字体文件名传给addApplicationFont,得到字体的Id
                int index=QFontDatabase::addApplicationFont("/home/font/"+fileList.at(0));
                if(index!= -1){
                    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
                    QStringList strList(QFontDatabase::applicationFontFamilies(index));
                    if (strList.count()>0){
                        QFont fontThis(strList.at(0));
                        fontThis.setBold(false);
                        fontThis.setPointSize(24);
                        a.setFont(fontThis);
                    }
                }
                else{
                    printf("system fnot error!!!\n");
                }
            }
        }
    }

    /**
     * 设置QSS样式
     */
    static void set_qss(QString path){
        QFile qss(path);
        if(qss.open(QFile::ReadOnly)){
          qApp->setStyleSheet(qss.readAll());
          qss.close();
        }
    }

    static void boot_animation(QMatrix &matrix,QString picPath){
        matrix.rotate(90);
        QSplashScreen screen;
        screen.setPixmap(QPixmap(picPath).transformed(matrix,Qt::FastTransformation));
        screen.setAutoFillBackground(true);
        screen.setPalette(QPalette(QColor(0,0,0)));
        screen.setWindowOpacity(0.5);// 设置窗口透明度
        screen.showMessage("网络连接中", Qt::AlignHCenter|Qt::AlignBottom, Qt::red);
        screen.show();
        QCoreApplication::processEvents();

        #if 0
            int delayTime = 5;
            QElapsedTimer timer;
            timer.start();
            while(timer.elapsed() < (delayTime * 1000))
            {
                 app.processEvents();
            }
        #endif

        #if 0
            QDateTime n=QDateTime::currentDateTime();
            QDateTime now;
            do{
                now=QDateTime::currentDateTime();
                a.processEvents();     //使程序在显示启动画面的同时仍能响应鼠标其他事件
            } while (n.secsTo(now)<=3);//5为需要延时的秒数
        #endif
        //QProcess::execute("/home/pppd/start_4g.sh");
 //        QProcess::execute("udhcpc eth0");
 //       QT调用外部程序有下面几种方法：
 //       //1.通过调用linux C 函数
 //       system("opt/myApp");
 //       //2.通过QProcess,阻塞调用
 //       QProcess::execute("/opt/myApp");
 //       //3.通过QProcess,非阻塞调用
 //       QProcess *pro = new QProcess;
 //       pro->start("/opt/myApp");
    }

    static void check_system_version();

};

#endif // APP_H
