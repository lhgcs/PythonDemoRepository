#ifndef PAY_H
#define PAY_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include "qrencode.h"
#include "my_define.h"
#include "serial.h"
#define SHOW_QRENCODE 50000
#define SHOW_CLOSE    30000

#include "model/httprequest.h"

#define SHOW_W 191
#define SHOW_H 270

class Pay:public QWidget
{
    Q_OBJECT
public:
    Pay(int fd, int select, QString *const _price, int *const status, QWidget *parent=0);
    void closeEvent(QCloseEvent *)
    {
        emit sub_widget_close();
    }
    ~Pay(){
        delete button_close;
        delete label;
        delete label_image;
        delete label_price;
        delete button_weixin;
        delete button_alipay;
        qDebug()<<"~~~~~~~~~~~pay";
    }
    void change_image_price(int select)
    {
        this->select=select;
        label_price->setText("价格:"+price_p[select-1]);
        label_stock->setText(QString("<font size=1>货道:%1 &nbsp;库存:%2</font>").arg(QString::number(select)).arg(QString::number(status_p[select-1])));
        show_image(select);
        this->show();
        timer_close->start(SHOW_CLOSE);
    }

    void send_status(QString str)
    {
        httpRequest->send_stauts(str);
    }

    //出货
    void put_out(int index)
    {
        select_pay_ok=index;
        send_open(fd,OPEN,index);
        timer_putout->start(14000);
        label_image->setText("支付成功\n正在出货");
        timer_close->start(SHOW_CLOSE);
    }
//    void send_status_slots()
//    {
//        send_data(0xFF);
//    }
//    enum{UpLoad,};
signals:
    void sub_widget_close();
//    void send_data(int value);
    void data(int type,int data=0);
private slots:

    void http_data_slot(int type,int data1,int data2){
        switch (type) {
        case HttpRequest::UpLoad:
            emit data(0xFF);
            break;
        case HttpRequest::temperature:
            send_temperature(fd,SET_TEMP,data1,data2);
            break;
        case HttpRequest::open:
            put_out(data1);
            break;
        default:
            break;
        }
    }


    //加载二维码
    void send_data_slots()
    {
         if(show_image(select))
         {
             QObject *pObj = sender();
             if(pObj==button_weixin){
                 httpRequest->send_Request(weixin,select);
                 current_payment=weixin;
                 timer_loading->start(TIME_OUT);
                 label_image->setText("正在加载二维码");
//                 label_image->setText("暂不支持微信");
             }
             else if(pObj==button_alipay){
                 current_payment=alipay;
                 httpRequest->send_Request(alipay,select);
                 timer_loading->start(TIME_OUT);
                 label_image->setText("正在加载二维码");
             }
         }
    }
    //加载二维码失败
    void loading_time_out()
    {
        label_image->setText("加载二维码失败");
    }

    //出货失败
    void putOut_time_out()
    {
        timer_putout->stop();
        httpRequest->send_Result(1,"myself");
        //label_working_show("出货失败\n已付款金额将退回原账户");
        label_image->setText("出货失败\n已付款金额\n将退回原账户!");
    }

    void close_time_out()
    {
         this->close();
    }

    void read_serial_result(int cmd,int value)
    {
        switch (cmd){
            case TRY:{
                       //emit send_data(0xA);
                        emit data(TRY);
                    }
                     break;
            case OPEN:
                     timer_putout->stop();
                     if(value==0x00){
                        httpRequest->send_Result(0,"liang");
                        label_image->setText("商品已出货");
                        status_p[select_pay_ok-1]--;
                     }
                     else if(value==0x03){
                         httpRequest->send_Result(1,"liang");
                         label_image->setText("出货失败\n已付款金额\n将退回原账户");
                     }
                     else if(value==0x02){
                         label_image->setText("正在出货中");
                     }
                    if(this->isHidden()){
                        this->show();
                    }
                     break;
            case DS18B20:{
                       httpRequest->send_env(value);
                    }
                    break;
            case DOOR:{
                    emit data(DOOR,value);
//                        if(value==DOOR_OPEN){
//                            emit send_data(0XB);;
//                        }
//                        else if(value==DOOR_CLOSE){
//                            emit send_data(0XC);
//                        }
                    }
                     break;
            case OPEN_RESET:{
//                        if(value==02)//正在工作
//                        {
//                            emit send_data(0XE);
//                        }
//                        else
//                        {
//                            emit send_data(0XD);
//                        }
                        emit data(OPEN_RESET,value);
                    }
                     break;
            default: break;
        }
    }

    void show_qrencode(QString str)
    {
        QRcode *qr = QRcode_encodeString(str.toStdString().c_str(),2,//指定二维码等级,L,M,Q,H四个等级，分别表示7%,15%,25%,30%，容错率越高，可被遮挡的面积越大
                                  QR_ECLEVEL_L,QR_MODE_8,1);
        if(qr != NULL)
        {
            if(timer_loading->isActive())
            {
                timer_loading->stop();
            }
            timer_close->start(SHOW_QRENCODE);
            int width=QRCODE_WIDTH;
            int height=QRCODE_HEIGHT;
            QImage image(width, height, QImage::Format_Mono);
            QPainter painter(&image);
            QColor background(Qt::white);
            painter.setBrush(background);
            painter.setPen(Qt::NoPen);
            painter.drawRect(0, 0, width, height);
            QColor foreground(Qt::black);
            painter.setBrush(foreground);

            const int qr_width = qr->width > 0 ? qr->width : 1;
            double scale_x = width*1.0 / qr_width;//二维码图片的缩放比例
            double scale_y = height*1.0 / qr_width;
            for( int y = 0; y < qr_width; y++)
            {
                for(int x = 0; x < qr_width; x++)
                {
                    unsigned char b = qr->data[y * qr_width + x];//qrcode->data包含了二维码图像的信息
                    if(b & 0x01)//bit0代表了点的颜色,1表示黑色，0表示白色。
                    {
                        QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                        painter.drawRects(&r, 1);
                    }
                }
            }
            QRcode_free(qr);
            //图片合成
            QPixmap logo;
            if(current_payment==weixin)
            {
                logo.load("/home/ico/logo_weixin.png");
            }
            else
            {
                logo.load("/home/ico/logo_alipay.png");
            }
            QPixmap pix=QPixmap::fromImage(image);
            QPainter p(&pix);
            p.setRenderHint(QPainter::SmoothPixmapTransform, true);
            p.drawPixmap((QRCODE_WIDTH-QRCODE_WIDTH*0.2)/2,(QRCODE_HEIGHT-QRCODE_HEIGHT*0.2)/2,QRCODE_WIDTH*0.2,QRCODE_HEIGHT*0.2,logo);
            label_image->setPixmap(pix);
         //   label_working_show("请扫描二维码");
        }
    }
private:
    QPushButton *button_weixin;
    QPushButton *button_alipay;
    QPushButton *button_close;
    QLabel *label;
    QLabel *label_image;
    QLabel *label_price;
    QLabel *label_stock;
    int fd;
    int select;
    int select_pay_ok;
    enum payment current_payment;
    HttpRequest *httpRequest;
    ReadSerialThread *read_serial;
    int *status_p;
    QString *price_p;
    QTimer *timer_putout;
    QTimer *timer_loading;
    QTimer *timer_close;

    void label_working_show(QString str)
    {
        label->setText(str);
    }

    bool show_image(int select)
    {
        bool ok=true;
        QString img=QString("%1/%2.png").arg(IMAGE_PATH,QString::number(select));
        QPixmap pix(img);
        if(pix.isNull()){
           if(pix.load("/home/ico/sellout.png"))
                label_image->setPixmap(pix);
           else
               label_image->setText("无法加载图片");
        }
        else{
            pix=pix.scaled(SHOW_W,SHOW_H);//,Qt::KeepAspectRatio
            if(status_p[select-1]>0){
                label_working_show("请选择付款方式，<br/>并扫描二维码");
            }
            else{
                ok=false;
                label_working_show("<font color=red>商品无货</font>");
                QPainter p(&pix);
                p.setRenderHint(QPainter::SmoothPixmapTransform, true);
                p.drawPixmap(SHOW_W/4,SHOW_H/4,SHOW_W/2,SHOW_H/2,QPixmap("/home/ico/sellout.png").scaled(SHOW_W/2,SHOW_H/2));
            }
            label_image->setPixmap(pix);
        }
        return ok;
    }
};

#endif // PAY_H
