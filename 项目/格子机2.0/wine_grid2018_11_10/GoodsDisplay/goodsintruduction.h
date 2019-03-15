#ifndef GOODSINTRUDUCTION_H
#define GOODSINTRUDUCTION_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>

#include "seletebuy.h"
#include "httprequest.h"
#include "Serial/serialputout.h"

#define WAIT_NO          0x00
#define WAIT_OPEN_ACK    0x01
#define WAIT_OPEN_FINISH 0x02
#define WAIT_OPEN_START  0x03

class GoodsIntruduction : public QWidget
{
    Q_OBJECT
public:
    explicit GoodsIntruduction(QWidget *parent = 0);
    SeleteBuy *seleteBuy;
    SerialPutOut *serialPutOut;
    HttpRequest* httpRequest;

    ~GoodsIntruduction(){
        delete labelPicture;
        delete labelPresent;
        delete seleteBuy;
        delete hl;
    }
    void setPicture(QString pic){
        labelPicture->setPixmap(QPixmap(pic));
    }
    void setPresent(QString name,QString price,QString remain){//text-align:right border:1px; ,QString sales
        QString str=QString("<table align=center > \
                             <tr><td align=right>酒名:  </td>  <td>%1</td></tr> \
                             <tr><td align=right>单价:  </td>  <td>%2元/500ml</td></tr> \
                             <tr><td align=right>剩余量: </td> <td>%3ml</td></tr> \
                             </table>").arg(name,price,remain);// <tr><td align=right>销售量: </td> <td>%4ml</td></tr>
        labelPresent->setText(str);
        labelPresent->adjustSize();
        seleteBuy->currentBox=box;
    }
    void widget_clear(){
        seleteBuy->widget_clear();
    }
    void start(){
        serialPutOut->start();
    }
    static int box;
    static float d;

signals:
    void updata_info(int ml);

private:
    QLabel *labelPicture;
    QLabel *labelPresent;
    QHBoxLayout *hl;

    QTimer *timer;

    uint weightTemp;
    int typeTemp;

    int timer_flag;
    int timer_loop_cnt;

    void timer_init(){
        timer->stop();
        timer_flag=WAIT_NO;
        timer_loop_cnt=0;
    }
    void timer_start(int flag){
        timer->stop();
        timer_flag=flag;
        timer_loop_cnt=0;
        timer->start();
    }

private slots:
    void httpRequest_slot(int type,int value,int ml,float density){
        switch (type) {
            case HttpRequest::qrcode:{
                if(httpRequest){
                    seleteBuy->paymentResult->qrencode_show(httpRequest->qrencode,value);
                }
                break;
            }
            case HttpRequest::open:
                qDebug()<<"open:"<<value<<ml<<density;
                if(value>0){
                    d=density;
                    weightTemp=ml*density*10;
                    typeTemp=value;
                    serialPutOut->open_box(typeTemp,weightTemp);
                    seleteBuy->paymentResult->wait_open_ack();
                    timer_start(WAIT_OPEN_ACK);
                }
                break;
                if(seleteBuy){
                    seleteBuy->select_clear();
                }
            default:break;
        }
    }

    void timer_slot(){
        timer_loop_cnt++;
        if(timer_flag==WAIT_OPEN_ACK){
            if(timer_loop_cnt>5){
                timer_init();
                App::card="";
                App::weight=0;
                httpRequest->send_Result("my");
                seleteBuy->paymentResult->put_out_failed();

                if(seleteBuy){
                    seleteBuy->select_clear();
                }
            }
            else {
                if(typeTemp>0){
                    serialPutOut->open_box(typeTemp,weightTemp);
                    seleteBuy->paymentResult->wait_open_ack();
                }
                else{
                    timer_loop_cnt=6;
                }
            }
        }
        else if(timer_flag==WAIT_OPEN_START){
            if(timer_loop_cnt>36){
                timer_init();
                App::card="";
                App::weight=0;
                httpRequest->send_Result("no button");
                seleteBuy->paymentResult->put_out_failed();

                if(seleteBuy){
                    seleteBuy->select_clear();
                }
            }
        }
        else if(timer_flag==WAIT_OPEN_FINISH){
            if(timer_loop_cnt>63){
                timer_init();
                App::card="";
                App::weight=0;
                httpRequest->send_Result("my");
                seleteBuy->paymentResult->put_out_failed();

                if(seleteBuy){
                    seleteBuy->select_clear();
                }
            }
        }
        else{
            timer_init();
        }
    }

    void serialPutOut_slot(int type,int value){
        qDebug()<<"get serial";
        switch (type) {
            case OPEN:{
                if(value==0x00){
                    qDebug()<<"get open 00 ok";
                    timer_start(WAIT_OPEN_START);
                    seleteBuy->paymentResult->put_out_ready();
                    typeTemp=0;
                    weightTemp=0;
                }
                else if(value==0x01){
                    qDebug()<<"get open 01 ing";
                    timer_start(WAIT_OPEN_FINISH);
                    seleteBuy->paymentResult->put_out_ing();
                }
                else{
                    qDebug()<<"get open failed";
                    timer_init();
                    seleteBuy->paymentResult->put_out_failed();

                    if(seleteBuy){
                        seleteBuy->select_clear();
                    }
                }
                break;
            }
            case PUT_OUT_STOP:{
                timer_init();
                qDebug()<<"PUT_OUT_STOP";
                httpRequest->send_Result("liang");
                int temp=App::weight/GoodsIntruduction::d+0.5;
                seleteBuy->paymentResult->put_out_ok(temp);
                App::card="";
                emit updata_info(temp);

                if(seleteBuy){
                    seleteBuy->select_clear();
                }

                break;
            }
            case OPEN_BANKNOTE:{
                if(!value){
                    if(seleteBuy->seleteMoneyPayment){
                        seleteBuy->seleteMoneyPayment->set_tip(App::paperMoney);
                    }
                }
                break;
            }
            case GET_MONEY:{
                switch (value) {
                    case 0x40:App::paperMoney+=1;break;
                    case 0x41:App::paperMoney+=5;break;
                    case 0x42:App::paperMoney+=10;break;
                    case 0x43:App::paperMoney+=20;break;
                    case 0x44:App::paperMoney+=50;break;
                    case 0x45:App::paperMoney+=100;break;
                    default:break;
                }
                if(seleteBuy->seleteMoneyPayment){
                    seleteBuy->seleteMoneyPayment->set_tip(App::paperMoney);
                }
                qDebug()<<"money:"<<App::paperMoney;
                break;
            }
            default:break;
        }
    }
};

#endif // GOODSINTRUDUCTION_H
