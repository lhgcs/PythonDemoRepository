#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include "StatusBar/statusbar.h"
#include "Sqlite/sqlitegoods.h"
#include "IC/IcCardProtocol.h"
#include "Setting/settingwidget.h"
#include "Loading/load.h"
#include "toast.h"
#include "Login/login.h"
#include "network/sysstart.h"

#include "selectdeviceradio.h"
#include "selectgoods.h"
#include "goodspresent.h"
#include "httprequest.h"
#include "Serial/serialputout.h"
#include "signallight.h"
#include "loadingqrcode.h"
#include "putouttip.h"
#include "replenishment.h"
#include "QPropertyAnimation"
#include <QParallelAnimationGroup>

#include "buttonradiogroup.h"
#include "subwidget/dialoginstance.h"
#include "subwidget/ensuredialog.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void init_widget();
    ~MainWindow();

private:
    StatusBar *statusBar;
    QStackedWidget *stackWidget;
    SerialClient *mIcCardClient;
    IcCardInterface *Interface;

    SysStart *sys;
    Load *load;

    SelectGoods *selectGoods;
    GoodsPresent *goodsPresent;
    SerialPutOut *serialPutOut;
    QTimer *timerPutOut;
    QTimer *timerClickButton;

    int clickButton;
    QButtonGroup *buttonGroup;
    LoadingQrcode *loadingQrcode;
    PutOutTip *putOutTip;

    Replenishment *replenishment;
    QTimer *timerReplenishment;
    Login *login;
    int open_box_id;

    int sarkIndex;
    int pageIndex;
    int loginClick;
    int goodsId;

    EnsureDialog *ensureDialog;

    QTimer *timerFixedCommunication;

private slots:
    void timerReplenishment_slot(){
        if(replenishment != NULL){
            open_box_id=-1;
            HttpRequest::getInstance()->currentPutOut.clear();
            if(!replenishment->open_vector.isEmpty()){
                int value=replenishment->open_vector.at(0);
                replenishment->open_vector.removeAt(0);

                int sark=(value-1)/18;
                int box=value-sark*18;
                sark++;

                serialPutOut->direct_open_box(sark,box);
//                Toast::getInstance()->set_text(QString("%1 %2柜 %3格子")
//                        .arg(QString::number(value),QString(sark+64),QString::number(box)));

                Toast::getInstance()->set_text(QString("%1柜 %2格子")
                    .arg(QString(sark+64),QString::number(box)));

//                DialogInstance::getInstance()->setText(QString("%1\n%2柜 %3格子")
//                 .arg(QString::number(value),QString(sark+64),QString::number(box)));
                return;
            }
        }
        timerReplenishment->stop();
    }

    /**
      选柜
     * @brief group_slot
     * @param num
     */
    void group_slot(int num){
        int index=-1;
        switch (num) {
            case 65:index=0;break;
            case 66:index=1;break;
            case 67:index=2;break;
            case 68:index=3;break;
            case 69:index=4;break;
            default:break;
        }
        if(index >= 0){
            if(index >= stackWidget->count()){
                return;
            }
            sarkIndex=index;
            pageIndex=0;
            if(index == 0){
                if(++loginClick >=3){
                    loginClick=0;
                    if(login == NULL){
                        login=new Login(this);
                        QObject::connect(login,SIGNAL(num(int,int)),this,SLOT(widget_slot(int,int)));
                    }
                    QPropertyAnimation *animation = new QPropertyAnimation(login,"windowOpacity");
                    connect(animation,SIGNAL(finished()), animation, SLOT(deleteLater()));
                    animation->setDuration(500);
                    animation->setStartValue(0);
                    animation->setEndValue(0.9);
                    for(int i=0;i<=9;i+=1){
                        animation->setKeyValueAt(i*0.1,i*0.1);
                    }
                    animation->start();
                    login->show();
                }
            }else{
                 loginClick=0;
            }

            if(sarkIndex != stackWidget->currentIndex()){
                stackWidget->setCurrentIndex(sarkIndex);
            }
        }
    }

    /**
      子窗口返回的数据
     * @brief widget_slot
     * @param layer
     * @param value
     */
    void widget_slot(int layer,int value){
        loginClick=0;
        switch(layer) {
            case App::selectGoodsLayout:{
                App::card.clear();
                if(value>=0){
                    goodsId=value;
                    QString id=QString::number(goodsId);
                    QString img="/home/image/"+id;
                    QString name,price,remain,sales;
                    SqliteGoods::getInstance()->get_table_data(id,name,price,remain,sales);
                    qDebug()<<id<<name<<price<<remain;
                    int n = remain.toInt();
                    if(n<=0){
                        Toast::getInstance()->set_text("此商品已售完");
                        return;
                    }

                    if(goodsPresent == NULL){
                        goodsPresent=new GoodsPresent(this);
                        QObject::connect(goodsPresent,SIGNAL(num(int,int)),this,SLOT(widget_slot(int,int)));
                    }
                    goodsPresent->set_present(img,name,price,remain,QString(sarkIndex+65),QString::number(goodsId));

                    QPropertyAnimation *animation = new QPropertyAnimation(goodsPresent,"windowOpacity");
                    connect(animation,SIGNAL(finished()), animation, SLOT(deleteLater()));
                    animation->setDuration(500);
                    animation->setStartValue(0);
                    animation->setEndValue(1);
                    for(int i=0;i<=10;i+=1){
                        animation->setKeyValueAt(i*0.1,i*0.1);
                    }
                    animation->start();
                    goodsPresent->show();
                }
                break;
            }
            case App::goodsPresentLayout:{
                if(value==3){//退出
                    close_goodsPresent();
                    return;
                }
                if(HttpRequest::getInstance()->isBusy){
                    DialogInstance::getInstance()->setText("请等待上一单交易完成",5000);
                    return;
                }
                int num=SqliteGoods::getInstance()->get_table_remain_data(QString::number(goodsId));
                //loadingQrcode->loading_start();
                if(num>0){
                    switch (value) {
                        case 0:HttpRequest::getInstance()->send_Request(App::buyAlipay,goodsId);break;
                        case 1:HttpRequest::getInstance()->send_Request(App::buyWeixin,goodsId);break;
                        case 2:{
                            if(App::card.isEmpty()){
                                DialogInstance::getInstance()->setText("未检测到会员卡，请放置会员卡",3000);
                            }
                            else{
                                HttpRequest::getInstance()->send_Request(App::buyCard,goodsId);
                                //loadingQrcode->loading_start_card();
                            }
                            break;
                        }
                        default:return;
                    }
                }else{
                    Toast::getInstance()->set_text("此商品已售完");
                }
                break;
            }

            case App::putOutTipLayout:{
                App::card.clear();
                close_putOutTip();
                break;
            }
            case App::replenishmentLayout:{
                App::card.clear();
                switch (value) {
                    case 2:{//打开
                        if(replenishment){
                            if(!timerReplenishment->isActive()){
                                timerReplenishment_slot();
                                timerReplenishment->start();
                            }else{
                                Toast::getInstance()->set_text("正在开锁");
                            }
                        }
                    }
                    break;
                    case 4:{//上传
                        if(App::replenishmentQrcode.isEmpty()){
                            DialogInstance::getInstance()->setText("请扫描对应的二维码再上传");
                        }else{
                            if(replenishment){
                                HttpRequest::getInstance()->send_stauts(replenishment->replenishment_str);
                            }
                            Toast::getInstance()->set_text("正在上传");
                        }
                    }
                    break;
                    case 5:{//下载
                        Toast::getInstance()->set_text("正在下载");
                        Load l;
                        l.set_sqliteGoods(SqliteGoods::getInstance());
                        if(l.loading()){
                            Toast::getInstance()->set_text("下载成功");
                        }else{
                            Toast::getInstance()->set_text("下载失败");
                        }
                    }
                    break;
                    case 6:{//退出
                        close_replenishment();
                        close_login();
                    }
                    break;
                    default:break;
                }
                break;
            }

            case App::loginLayout:{
                App::card.clear();
                if(value==0){
                    if(login != NULL){
                        if(replenishment==NULL){
                            replenishment=new Replenishment(this);
                            QObject::connect(replenishment,SIGNAL(num(int,int)),this,SLOT(widget_slot(int,int)));
                        }
                        replenishment->init_data();
                        replenishment->show();

                        QPropertyAnimation *animation= new QPropertyAnimation(replenishment,"geometry");
                        animation->setDuration(1000);
                        animation->setStartValue(QRect(replenishment->width(),0,
                                                       replenishment->width(), replenishment->height()));
                        animation->setEndValue(QRect(0,0,
                                                     replenishment->width(), replenishment->height()));

                        QPropertyAnimation *animation1= new QPropertyAnimation(login,"geometry");
                        animation1->setDuration(1000);
                        animation1->setStartValue(QRect(0, 0, login->width(), login->height()));
                        animation1->setEndValue(QRect(-login->width(), 0, login->width(), login->height()));

                        QParallelAnimationGroup *group = new QParallelAnimationGroup;
                        group->addAnimation(animation);
                        group->addAnimation(animation1);
                        group->start();
                        connect(group,SIGNAL(finished()), group, SLOT(deleteLater()));
                        connect(animation,SIGNAL(finished()), animation, SLOT(deleteLater()));
                        connect(animation1,SIGNAL(finished()), animation1, SLOT(deleteLater()));
                    }
                }
                else if(value==1){
                    close_login();
                }
                break;
            }
            default: break;
        }
    }

    /**
      查积分
     * @brief send_iccard_slots
     * @param _card
     */
    void send_iccard_slots(QString _card){
        qDebug()<<"card id"<<_card;
        Toast::getInstance()->set_text("已放置会员卡");
        if(goodsPresent != NULL){
            int num=SqliteGoods::getInstance()->get_table_remain_data(QString::number(goodsId));
            if(num>0){
                App::card=_card;
                HttpRequest::getInstance()->send_card(_card);
            }else{
                Toast::getInstance()->set_text("此商品已售完");
            }
        }else{
            App::card.clear();
        }
    }
    /**是否积分兑换
     * @brief ensureDialog_slot
     * @param value
     */
    void ensureDialog_slot(int value){
        if(value == 0){
            HttpRequest::getInstance()->send_Request(App::buyCard,goodsId);
        }else{
            App::card.clear();
        }
        if(ensureDialog != NULL){
            ensureDialog->close();
        }
    }

    void load_finish(bool value){
        if(value){
            //设置窗口的背景
//              QPixmap pixmap("/home/ico/wine");
//              QPalette palette = this->palette();
//              palette.setBrush(this->backgroundRole(),
//                           QBrush(pixmap.scaled(this->size(),
//                                  Qt::IgnoreAspectRatio,
//                                  Qt::SmoothTransformation)));
//              this->setPalette(palette);

//            this->setStylSheet("QDialog{background-image:url()");

            this->setStyleSheet("QWidget#MainWindow {border-image:url(/home/ico/wine);}");
            init_widget();
        }
        if(sys != NULL){
            sys->close();
            sys->deleteLater();
            sys=NULL;
        }
        this->update();
        if(load != NULL){
            load->deleteLater();
            load=NULL;
        }
    }

    void getSeriesData_slots(QString str){
        HttpRequest::getInstance()->send_goods_id(str);
        App::replenishmentQrcode=str;
    }

    void httpRequest_slot(int type,int value){
        switch (type) {
            case HttpRequest::qrcode:{
                if(value==App::buyAlipay || value==App::buyWeixin){
                    if(loadingQrcode == NULL){
                        loadingQrcode=new LoadingQrcode(this);
                    }
                    //loadingQrcode->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
                    if(value==App::buyAlipay){
                        loadingQrcode->set_qrencode(HttpRequest::getInstance()->qrencode,"/home/ico/logo_alipay.png");
                    }
                    else if(value==App::buyWeixin){
                        loadingQrcode->set_qrencode(HttpRequest::getInstance()->qrencode,"/home/ico/logo_weixin.png");
                    }
                    loadingQrcode->raise();
                    loadingQrcode->show();
                }
                break;
            }
            case HttpRequest::open:{
                if(putOutTip == NULL){
                    putOutTip=new PutOutTip(this);
                    QObject::connect(putOutTip,SIGNAL(num(int,int)),this,SLOT(widget_slot(int,int)));
                }
                if(putOutTip != NULL){
                    putOutTip->pay_ok();
                    putOutTip->show();
                }

                close_loadingQrcode();

                open_box_id=value;

                int sark=(value-1)/18;
                int box=value-sark*18;
                sark++;

                QEventLoop eventloop;
                QTimer::singleShot(1000, &eventloop, SLOT(quit()));
                eventloop.exec();
                if((sark>=0)&&(value>0)){
                    serialPutOut->open_box(sark,box);
                    timerPutOut->start(7000);
                    if(putOutTip != NULL){
                        putOutTip->put_out_ing();
                    }
                }
                break;
            }
            case HttpRequest::time:{
                if(statusBar){
                    statusBar->show_time();
                }
                break;
            }
            case HttpRequest::upload:{
                if(replenishment){
                    replenishment->save_to_sql();
                    DialogInstance::getInstance()->setText("上传成功");
                    replenishment->init_data();
                }
                break;
            }
            case HttpRequest::card:{
                DialogInstance::getInstance()->close();
                if(ensureDialog == NULL){
                    ensureDialog=new EnsureDialog();
                    QObject::connect(ensureDialog,SIGNAL(upload(int)),this,SLOT(ensureDialog_slot(int)));
                }
                ensureDialog->setText("会员:"+App::cardName+"\n"
                                      +"积分:"+App::cardIntegration+"\n"
                                      +"是否使用积分兑换?");
                ensureDialog->show();
            }
            default:break;
        }
    }

    /**
      串口
     * @brief serialPutOut_slot
     * @param type
     * @param value
     */
    void serialPutOut_slot(int type,int value){
        qDebug()<<"get serial";
        switch (type) {
            case OPEN:{
                if(value==0x00){
                    timerPutOut->stop();
                    if(open_box_id > 0){
                        QString id=QString::number(open_box_id);
                        put_out_ok();
                        if(putOutTip){
                            putOutTip->put_out_ok();
                        }

                        int remain=SqliteGoods::getInstance()->get_table_remain_data(id);
                        remain=remain>0?remain-1:0;
                        SqliteGoods::getInstance()->table_update(id,QString::number(remain));
                    }
//                    else{
//                        DialogInstance::getInstance()->setText("打开成功");
//                    }
                }
                else if(value==0x01){
                    qDebug()<<"get open 01 ing";
                }
                else{
                    qDebug()<<"get open failed";
                    timerPutOut->stop();
                    if(open_box_id > 0){
                        put_out_failed();
                        if(putOutTip){
                            putOutTip->put_out_failed();
                        }
                    }else{
                        Toast::getInstance()->set_text("打开失败");
                        //DialogInstance::getInstance()->setText("打开失败");
                    }
                }
                break;
            }
            default:break;
        }
    }

    void timerPutOut_slot(){
        put_out_failed("my");
        if(putOutTip != NULL){
            putOutTip->put_out_failed();
        }
    }
    void timerFixedCommunication_slot(){
        HttpRequest::getInstance()->send_FixedCommunication();
    }
private:
    /**关闭窗口
     * @brief close_goodsPresent
     */
    void close_goodsPresent(){
        if(goodsPresent != NULL){
            goodsPresent->close();
            goodsPresent->deleteLater();
            goodsPresent=NULL;
        }
    }
    void close_loadingQrcode(){
        if(loadingQrcode != NULL){
            loadingQrcode->close();
            loadingQrcode->deleteLater();
            loadingQrcode=NULL;
        }
    }
    void close_putOutTip(){
        if(putOutTip!= NULL){
            putOutTip->close();
            putOutTip->deleteLater();
            putOutTip=NULL;
        }
        close_loadingQrcode();
        close_goodsPresent();
    }
    void close_login(){
        if(login != NULL){
            login->close();
            login->deleteLater();
            login=NULL;
        }
    }
    void close_replenishment(){
        if(replenishment != NULL){
            replenishment->close();
            replenishment->deleteLater();
            replenishment=NULL;
        }
    }
    /**
      出货成功
     * @brief put_out_ok
     */
    void put_out_ok(){
        open_box_id=-1;
        timerPutOut->stop();
        HttpRequest::getInstance()->send_Result(0,"liang");
    }
    /**
      出货失败
     * @brief put_out_failed
     * @param who
     */
    void put_out_failed(QString who="liang"){
        open_box_id=-1;
        timerPutOut->stop();
        HttpRequest::getInstance()->send_Result(1,who);
    }
protected:
    void paintEvent(QPaintEvent *){
        QStyleOption  opt;
        opt.init(this);
        QPainter  p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // MAINWINDOW_H

