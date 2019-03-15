#include "mainwindow.h"
#include <unistd.h>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>

#include <QMutex>
#include "util/firmwaredownload.h"
#include "util/filehandler.h"

#include "configure/watchdog.h"
#include "configure/app.h"
#include "configure/configure.h"
#include "subwidget/goodsgrid.h"
#include "network/connecttonetwork.h"

#define VIDEO_HEIGHT       405

#include "control/linuxsocket.h"

QMutex mutex;//互斥锁
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg){
#if 0
    QMutexLocker locker(&mutex);
    QString text= QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss ddd] ");      //产生时间

    switch(type){
        case QtWarningMsg: text.append(QString("Warning:"));  break;//警告信息
        case QtCriticalMsg:text.append(QString("Critical:")); break;//严重错误
        case QtFatalMsg:   text.append(QString("Fatal:"));    break;//致命错误
        case QtDebugMsg:   {
//            char stderr[256];
//            memset(stderr,0,256);
//            QByteArray localMsg = msg.toLocal8Bit();
//            sprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            printf("%s\n",stderr);
        }
        break;//调试信息
        default: break;
    }
    text.append(msg);
    QFile file;
    file.setFileName("/home/log/log.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Append)){
        QTextStream text_stream(&file);
        text_stream << text << "\r\n";
        file.flush();
        file.close();
    }
#endif
}

MainWindow::MainWindow(QWidget *parent):BaseWidget(parent){
    this->hide();
    this->setObjectName("MainWindow");

    #if MY_DEBUG
    #else
        qInstallMessageHandler(outputMessage);//注册MessageHandler
    #endif
}

void MainWindow::init_network(){
    this->show();

    Toast::getInstance(this);                            //吐司
    DialogInstance::getInstance("系统初始化中",5000,this); //提示框
    DataBaseHelper::getInstence();                       //数据库
    HttpRequest::getInstance();                          //请求

    for(int i=1;i<=App::goodsTotal;i++){
        bool res = TablePutOutTimeInfo::getInstence()->table_select_id(
                    TablePutOutTimeInfo::getInstence()->getTableName(),i);
        if(res == false){
            TablePutOutTimeInfo::getInstence()->table_insert(
                        TablePutOutTimeInfo::getInstence()->getTableName(),i,0);
        }
    }

    //App::get_system_confidure();
    Configure con;
    App::goodsTotal=con.goodsNum;
    App::payType=con.payType;

    App::HTTP=read_File("/home/conf/http.txt");
    App::MACID=read_File("/home/conf/mac_id.txt");

    HttpRequest::getInstance()->http=App::HTTP;
    HttpRequest::getInstance()->mac_id=App::MACID;

    bool flag=false;
    QString str=read_File("/home/conf/configure.txt");
    if(str != NULL){
        con.parse_json(str);
        App::goodsTotal=con.goodsNum;
        if(con.macIdType != 0){//0 4g   1 macid
            QString mac=read_File("/home/conf/mac_id.txt");
            if(mac != NULL){
                App::MACID=mac;
                flag=true;
            }
        }
        switch (con.payType) {
            case HttpRequest::ALIPAY:
            case HttpRequest::WEIXIN:
            case HttpRequest::WEIXIN_AND_ALIPAY:App::payType=con.payType;
            break;
        default:App::payType=HttpRequest::WEIXIN_AND_ALIPAY;
            break;
        }
    }

    if(flag == false){
        QString temp=Module_4G::get_moudule_4G_id();
        if(temp.isEmpty()){
            DialogInstance::getInstance()->setText("请插入4G模块\n或者手动设置机台号",10000);
        }else{
            App::MACID=temp;
        }
    }

    process = NULL;
    load=NULL;
    downFileProgress=NULL;
    create_Dir("/home/image");
    create_Dir("/home/video");
    create_Dir("/home/sql");
    create_Dir("/home/log");
    create_Dir("/home/upgrade");

    downFileProgress=new DownFileProgress(this);
    QObject::connect(downFileProgress,SIGNAL(finished()),this,SLOT(downFileProgress_slot()));

    ConnectToNetwork *network=new ConnectToNetwork;
    connect(network, SIGNAL(finish(bool)), this, SLOT(thread_finished_slot()));
    network->start();

    init_view();

    if(load == NULL){
        load=new Load();
    }

  //  loadThread = new QThread(this);
  //  load->moveToThread(loadThread);
    //在run()函数执行前会发射信号started()
//    QObject::connect(loadThread, &QThread::started, this, &MainWindow::get_server_data2);
//    QObject::connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()),Qt::QueuedConnection);

  //  QObject::connect(loadThread, &QThread::started, load, &Load::loading,Qt::DirectConnection);
 //   QObject::connect(loadThread,SIGNAL(finished()),this,SLOT(get_server_data_finish_slot()),Qt::DirectConnection);

     QObject::connect(load,SIGNAL(finished()),this,SLOT(get_server_data_finish_slot()));

    get_server_data_start();
    //LinuxSocket *socket=new LinuxSocket;
}

void MainWindow::init_view(){

    statusBar=new StatusBar(this);
    statusBar->set_macId(App::MACID);
    statusBar->set_network_status("/home/ico/netNo.png");
    statusBar->setBtn_callback(this);

    int w = App::screenWidget;
    int h = App::screenHeight*2.0/3-statusBar->height()-4;

    BaseSubWidget *subWidget=new BaseSubWidget;
    subWidget->setFixedSize(w,h);
    QVBoxLayout *vl = new QVBoxLayout(subWidget);
    vl->setContentsMargins(0,0,0,0);
    vl->addStretch(2);

    #if JIUHAOCANG
    goodsGrid=new GoodsGrid(w,h,this);
    QObject::connect(goodsGrid,SIGNAL(mapped(int)),this,SLOT(get_num_slot(int)));
    vl->addWidget(goodsGrid);
    #else
    goodsGrid=new GoodsGrid(w,App::screenHeight*0.4,this);
    QObject::connect(goodsGrid,SIGNAL(mapped(int)),this,SLOT(get_num_slot(int)));
    vl->addWidget(goodsGrid);
    num_key=new NumKeyboard(this);
    QObject::connect(num_key,SIGNAL(num(int)),this,SLOT(get_num_slot(int)));
    vl->addStretch(1);
    vl->addWidget(num_key,0,Qt::AlignHCenter);
    #endif
    vl->addStretch(2);
    subWidget->setLayout(vl);

    stackedLayout=new QStackedWidget(this);
    stackedLayout->setContentsMargins(0,0,0,0);
    stackedLayout->setFixedSize(w,h);

    payWidget=new PayWidget(w,h,stackedLayout);
    addWare=new AddWare(w,h,this);
    settingWidget=new SettingWidget(this);

    stackedLayout->addWidget(subWidget);
    stackedLayout->addWidget(payWidget);
    stackedLayout->addWidget(addWare);
    stackedLayout->addWidget(settingWidget);

    paramterWidget = new ParamterWidget(w,h,this);
    loginDialog=new LoginDialog(w,h,this);
    stackedLayout->addWidget(paramterWidget);
    stackedLayout->addWidget(loginDialog);

    QObject::connect(payWidget,     SIGNAL(sub_widget_data(int,int)),this,SLOT(sub_widget_data_slot(int,int)));
    QObject::connect(addWare,       SIGNAL(sub_widget_data(int,int)),this,SLOT(sub_widget_data_slot(int,int)));
    QObject::connect(settingWidget, SIGNAL(sub_widget_data(int,int)),this,SLOT(sub_widget_data_slot(int,int)));
    QObject::connect(paramterWidget,SIGNAL(sub_widget_data(int,int)),this,SLOT(sub_widget_data_slot(int,int)));
    QObject::connect(loginDialog,   SIGNAL(sub_widget_data(int,int)),this,SLOT(sub_widget_data_slot(int,int)));

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(0,0,0,0);
    v->addSpacing(VIDEO_HEIGHT);
    v->addStretch();
    v->addWidget(stackedLayout);
    v->addStretch();
    v->addWidget(downFileProgress);
    v->addWidget(statusBar);
    setLayout(v);

    QObject::connect(HttpRequest::getInstance(),SIGNAL(http_qrencode(int,QString)),this,SLOT(show_qrencode(int,QString)));
    QObject::connect(HttpRequest::getInstance(),SIGNAL(http_data(int,int,int)),this,SLOT(http_data_slot(int,int,int)));

    stackedLayout->setCurrentIndex(GOODSWIDGET);

    //打开串口
    QObject::connect(SerialPutOut::getInstance(),SIGNAL(serial_result(int,int)),this,SLOT(serial_data_slot(int,int)));

    timer_putout=new QTimer(this);
    timer_putout->setSingleShot(true);
    QObject::connect(timer_putout,SIGNAL(timeout()),this,SLOT(putOut_time_out()));

    timer_reset=new QTimer(this);
    timer_reset->setSingleShot(true);
    QObject::connect(timer_reset,SIGNAL(timeout()),this,SLOT(time_out_reset()));

    timer_status=new QTimer(this);
    QObject::connect(timer_status,SIGNAL(timeout()),this,SLOT(time_status_slot()));

    SerialPutOut::getInstance()->start();
    SerialPutOut::getInstance()->get_temp();
    statusBar->start();
    timer_status->start(120000);

//    WatchDog *watchDog=new WatchDog;
//    watchDog->start();

  //  show_sub_widget(4);
}
