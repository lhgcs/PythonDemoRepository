#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QEvent>
#include "qrencode.h"

#include <QThread>
#include <QFileInfoList>
#include <QDir>
#include <QProcess>
#include <QStyleOption>
#include <QPropertyAnimation>

#include "model/load.h"
#include "model/httprequest.h"

#include "serial/serialputout.h"

#include <QStackedWidget>

#include "view/basewidget.h"
#include "view/paywidget.h"
#include "view/settingwidget.h"
#include "view/addware.h"

#include "subwidget/numkeyboard.h"
#include "subwidget/statusbar.h"
#include "subwidget/goodsgrid.h"
#include "subwidget/downfileprogress.h"
#include "network/checknetworkstatus.h"

#include "paramterwidget.h"
#include "logindialog.h"
#include "configure/toast.h"

class MainWindow : public BaseWidget , public BtnCallBack
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow(){
        SerialPutOut::getInstance()->close();
    }
    void init_network();
    void init_view();

    //回调函数
    void callback(){
        show_sub_widget(LOGINDIALOG);
        loginDialog->set_focus();
    }

private slots:
    void thread_finished_slot(){
        qDebug()<<"thread finish";
        Toast::getInstance()->set_text("连接网络成功");
        //DialogInstance::getInstance()->setText("连接网络成功");
        statusBar->set_network_status(QString("/home/ico/netYes.png"));

//        CheckNetworkStatus *net=new CheckNetworkStatus();
//        net->start();

        App::check_system_version();
        HttpRequest::getInstance()->get_time();
        HttpRequest::getInstance()->get_temp();
        HttpRequest::getInstance()->get_user();

        get_server_data_start();
        statusBar->set_time();
    }
    void downFileProgress_slot(){
        qDebug()<<"downfile finish";
        goodsGrid->set_image();
        for(int k=0;k<App::goodsTotal;k++){
            GoodInfo info;
            TableGoodInfo::getInstence()->table_select_price_stock(App::goodsTableName,k+1,info);
            goodsGrid->set_stock_price(k,info.getStock(),info.getPrice());
        }
        play_video();
    }

    void show_qrencode(int payment,QString qrcode){
        show_sub_widget(PAYWIDGET);
        payWidget->set_qrcode(payment,qrcode);
    }

    void http_data_slot(int type,int data1,int data2){
        switch (type) {
            case HttpRequest::open:       {
                qDebug()<<"open"<<data1;
                    payWidget->set_payment_logo();
                    SerialPutOut::getInstance()->open_box(data1);
                    #if JIUHAOCANG
                    timer_putout->start(60000);
                    #else
                    timer_putout->start(14000);
                    #endif
                    DialogInstance::getInstance()->setText("支付成功\n请耐心等待");
            }
                break;
            case HttpRequest::temperature:
            qDebug()<<data1<<data2;
                SerialPutOut::getInstance()->send_temperature(data1,data2);
                break;
            case HttpRequest::UpLoad:    {
                addWare->sync_database();
                for(int k=0;k<App::goodsTotal;k++){
                    GoodInfo info;
                    TableGoodInfo::getInstence()->table_select_price_stock(App::goodsTableName,k+1,info);
                    goodsGrid->set_stock_price(k,info.getStock(),info.getPrice());
                }
            }
                break;
            case HttpRequest::getServerData:    {
                Toast::getInstance()->set_text("开始下载");
                get_server_data();
            }
                break;
            default:break;
        }
    }
    /**
      串口
     * @brief serial_data_slot
     * @param cmd
     * @param value
     */
    void serial_data_slot(int cmd,int value){
        switch (cmd){
            case TRY:
                Toast::getInstance()->set_text("尝试通信成功");
                break;
            case OPEN:
                 timer_putout->stop();
                 switch (value) {
                     case OPEN_SUCCESS:{

#if JIUHAOCANG
                         if(HttpRequest::getInstance()->getCurrent_put_out() == "reset"){
                             HttpRequest::getInstance()->setIsBusy(false);
                             reset_next();
                             return;
                         }
#endif
                         int box=HttpRequest::getInstance()->send_result(0,"liang");
                         DialogInstance::getInstance()->setText("请取走商品");
                         show_sub_widget(GOODSWIDGET);
                         if(box > 0 && box <= App::goodsTotal){
                             TableGoodInfo::getInstence()->table_number_reduce(App::goodsTableName,box);
                             int num=TableGoodInfo::getInstence()->table_select_stock(App::goodsTableName,box);
                             goodsGrid->set_stock(box-1,num);
                         }
                     }
                         break;
                     case OPEN_MOTOR_FAIL:
                         DialogInstance::getInstance()->setText("似乎故障了");
                         break;
                     case OPEN_ING:
                         DialogInstance::getInstance()->setText("正在出货中");
                         break;
                     case OPEN_FAIL:
#if JIUHAOCANG
                         if(HttpRequest::getInstance()->getCurrent_put_out() == "reset"){
                             HttpRequest::getInstance()->setIsBusy(false);
                             reset_next();
                             return;
                         }
#endif

                         HttpRequest::getInstance()->send_result(1,"liang");
                         DialogInstance::getInstance()->setText("出货失败\n已付款金额将退回原账户");
                         show_sub_widget(GOODSWIDGET);
                         break;
                     default:
                         break;
                     }
                 break;
            case DS18B20:{
                    HttpRequest::getInstance()->send_env(value);
                    statusBar->set_temp(value);
                }
                break;
            case DOOR:{
                    if(value == 1){
                        show_sub_widget(ADDWIDGET);
                        addWare->get_database_data();
                    }else{
                        show_sub_widget(GOODSWIDGET);
                    }
                }
                 break;
            case OPEN_RESET:{

                if(value == 0){
                    reset_next("校正成功\n");
                }else{
                    reset_next("校正失败("+QString::number(value)+")\n");
                }

                }
                 break;
            default: break;
        }
    }

    /**
      购买
     * @brief get_num_slot
     * @param value
     */
    void get_num_slot(int value){
        if(value<1||value>App::goodsTotal){
            DialogInstance::getInstance()->setText("请输入正确的商品编号");
        }else{

            int res=TableGoodInfo::getInstence()->table_select_stock(App::goodsTableName,value);
            if(res>0){
                show_sub_widget(PAYWIDGET);
                payWidget->set_context(value);
            }else{
                DialogInstance::getInstance()->setText("此商品已售完");
            }

        }
     }

    /**
      子窗口
     * @brief sub_widget_data_slot
     * @param widget
     * @param data
     */
    void sub_widget_data_slot(int widget,int data){
        switch (widget) {
            case PAYWIDGET:
                                show_sub_widget(GOODSWIDGET);
                break;
            case ADDWIDGET:
                if(data == 0){//退出
                    show_sub_widget(GOODSWIDGET);
                    if(timer_reset->isActive()){
                        timer_reset->stop();
                    }
                }else if(data ==1){
                    if(timer_reset->isActive()){
                        Toast::getInstance()->set_text("正在校正，请等待校正完成");
                    }else{
                        current_reset=0;
                        HttpRequest::getInstance()->setCurrent_put_out("reset");
                        HttpRequest::getInstance()->setIsBusy(false);
                        reset_next();
                    }
                }else if(data == 2){
                    Toast::getInstance()->set_text("开始下载");
                    get_server_data();
                }
                else if(data == 3){
                    show_sub_widget(SETTINGWIDGET);
                    settingWidget->show_data();
                    if(timer_reset->isActive()){
                        timer_reset->stop();
                    }
                }
                else if(data == 4){
                    show_sub_widget(PARAMSWIDGET);
                    paramterWidget->get_data();
                }
                break;
            case SETTINGWIDGET:
            case PARAMSWIDGET:
                show_sub_widget(ADDWIDGET);
                addWare->get_database_data();
            break;
            case LOGINDIALOG:
                if(data == 0){
                    show_sub_widget(GOODSWIDGET);
                }else{
                    show_sub_widget(ADDWIDGET);
                    addWare->get_database_data();
                }
            break;
            default:break;
        }
    }
    /**
      校正失败
     * @brief time_out_reset
     */
    void time_out_reset(){
       reset_next();
    }
    /**
      出货失败
     * @brief putOut_time_out
     */
    void putOut_time_out(){
        timer_putout->stop();
        HttpRequest::getInstance()->send_result(1,"myself");
        DialogInstance::getInstance()->setText("出货失败\n已付款金额将退回原账户!");
        show_sub_widget(GOODSWIDGET);
    }
    void time_status_slot(){
        SerialPutOut::getInstance()->get_temp();
        if(StatusBar::get_network_status() == true){
            statusBar->set_network_status(QString("/home/ico/netYes.png"));
        }else{
            statusBar->set_network_status(QString("/home/ico/netNo.png"));
        }
        HttpRequest::getInstance()->send_onlide();
        HttpRequest::getInstance()->get_user();
    }
    void update_view(){
        this->repaint();
    }
    void get_server_data_finish_slot(){
        int downFile = load->getDownFileNum();
        if(downFile < 0){
            Toast::getInstance()->set_text("连接服务器失败");
            downFileNum=0;
            downFileProgress_slot();
        }else if(downFile == 0){
            Toast::getInstance()->set_text("需要下载0个文件");
            downFileNum=0;
            downFileProgress_slot();
        }else{
            Toast::getInstance()->set_text("需要下载"+QString::number(downFile)+"个文件");
            downFileNum=downFile;
            if(downFileProgress != NULL){
                downFileProgress->setTotal(downFileNum);
                downFileProgress->start_timer();
            }
        }
        qDebug()<<"downFileNum:"<<downFileNum;
    }

private:
    QTimer *timer_status,*timer_reset;
    AddWare *addWare;
    Load *load;
    DownFileProgress *downFileProgress;
    QProcess *process;
    GoodsGrid *goodsGrid;
    StatusBar *statusBar;

    QTimer *timer_putout;

    PayWidget *payWidget;
    QStackedWidget *stackedLayout;

    SettingWidget *settingWidget;
    ParamterWidget *paramterWidget;

    LoginDialog *loginDialog;
    NumKeyboard *num_key;

    QThread *loadThread;

    enum {GOODSWIDGET=0,PAYWIDGET=1,ADDWIDGET=2,SETTINGWIDGET=3,PARAMSWIDGET=4,LOGINDIALOG=5};
    int current_reset;
    int downFileNum;

    void show_sub_widget(int index){
        if(stackedLayout->currentIndex() != index && index < stackedLayout->count()){
            stackedLayout->setCurrentIndex(index);
        }
    }

    void play_video(){
        QStringList stringFilter;
        stringFilter<<"*.wmv"<<"*.mp4"<<"*.avi"<<"*flv";//设置过滤类型
        QDir dir("/home/video");
        if(dir.exists()){
            QFileInfoList list =dir.entryInfoList(stringFilter);  //dir.setNameFilters(filters);//设置文件名的过滤
            if(list.count()>0){
                mplayer(list.at(0).filePath());
                //mplayer("/home/video");
                qDebug()<<list.at(0).filePath();
            }
        }
        QTimer::singleShot(2500, this, SLOT(update_view()));
    }

    void get_server_data(){
        if(load == NULL){
            load=new Load(this);
        }

        if (process != NULL){
            process->kill();
            process->deleteLater();
            process = NULL;
            system("/usr/bin/killall ffplay_pos");
        }

        int downFile=load->loading();
        if(downFile < 0){
            Toast::getInstance()->set_text("连接服务器失败");
            downFileNum=0;
            downFileProgress_slot();
        }else if(downFile == 0){
            Toast::getInstance()->set_text("需要下载0个文件");
            downFileNum=0;
            downFileProgress_slot();
        }else{
            Toast::getInstance()->set_text("需要下载"+QString::number(downFile)+"个文件");
            downFileNum=downFile;
            if(downFileProgress != NULL){
                downFileProgress->setTotal(downFileNum);
                downFileProgress->start_timer();
            }
        }
        qDebug()<<"downFileNum:"<<downFileNum;
    }

    void get_server_data2(){
        load->loading();
    }

    void get_server_data_start(){
        //loadThread->start();
        load->start();
    }

    //播放视频
    void mplayer(QString fileName){
        //terminate()只是通知QProcess结束，但QProcess并没有马上结束
        if(process == NULL){
            process = new QProcess(this);
            if(false == process->waitForFinished(5000)){
                qDebug()<<"process is can not finish";
            }

            QStringList args;
            args<<"-x"<<"405"<<"-y"<<"720"<<"-loop"<<"0"<<fileName;
            qDebug()<<args;
            process->start ("/home/app/ffplay_pos",args);
        }
    }

    void reset_next(){
        if(timer_reset->isActive()){
            timer_reset->stop();
        }
        if(!addWare->isVisible()){
            return;
        }
        addWare->reset_ok(current_reset);
        int box=addWare->get_reset_box();
        if(box > 0){
            current_reset=box;
            SerialPutOut::getInstance()->open_reset(box);
#if JIUHAOCANG
        timer_reset->start(30000);

#else
      timer_reset->start(9000);
#endif
        }else{
            qDebug()<<"box"<<box;
            Toast::getInstance()->set_text("已全部校正完成");
        }
    }

    void reset_next(QString str){
        if(timer_reset->isActive()){
            timer_reset->stop();
        }
        if(!addWare->isVisible()){
            return;
        }
        addWare->reset_ok(current_reset);
        int box=addWare->get_reset_box();
        if(box > 0){
            current_reset=box;
            SerialPutOut::getInstance()->open_reset(box,str);
#if JIUHAOCANG
        timer_reset->start(30000);

#else
      timer_reset->start(9000);
#endif
        }else{
            qDebug()<<"box"<<box;
            Toast::getInstance()->set_text(str+"已全部校正完成");
        }
    }

};

#endif // MAINWINDOW_H

//    //播放视频
//    void mplayer(QString fileName)
//    {
//        if (process != NULL)  process->kill();
//        process = new QProcess(this);
//        QStringList args;
//        args<<"-slave"<<"-quiet"<< "-zoom";
//        args<< "-x"<<QString::number(WIDTH);       //图像缩放宽度
//        args<< "-y"<<QString::number(VIDEO_HEIGHT);//图像缩放高度
//        args<< "-vf"<<"rotate=1";                  //,scale=720:405"
//        args<< "-geometry"<<QString("%1:0").arg(QString::number(HEIGHT-VIDEO_HEIGHT));
//        args<< "-vo"<<"fbdev2";
//        //args<< "-ac"<<"mad";
//        args<< "-nosound";   //静音播放
//        args<< "-loop"<<"0"; //无限播放
//        args<< "-framedrop";
////        args<< "-cache"<<"8192";
//        args<< fileName;
//        process->start ("mplayer",args);
//    }


//    //播放视频
//    void mplayer(QString fileName)
//    {
//        if (process != NULL)  process->kill();
//        process = new QProcess(this);
//        QStringList args;
//      //  args<<fileName<<"-slave -quiet -vo fbdev2 -nosound -aspect 16:9 -zoom -x 720 -y 405 -geometry 875:0 -vf rotate=1 -framedrop -loop 0";
// args<<fileName<<"-slave"<<"-quiet"<<"-vo"<<"fbdev2"<<"-nosound"<<"-zoom"<<"-x"<<"720"<<"-y"<<"405"<<"-geometry"<<"875:0"<<"-vf"<<"rotate=1"<<"-loop"<<"0";
////<<"-framedrop"
//        qDebug()<<args;
//        process->start ("mplayer",args);
//    }




