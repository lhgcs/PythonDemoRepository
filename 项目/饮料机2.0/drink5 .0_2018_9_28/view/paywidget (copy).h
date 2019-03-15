#ifndef PAYWIDGET_H
#define PAYWIDGET_H

#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include "qrencode.h"

#include "serial/serial.h"

#include "subwidget/basesubwidget.h"
#include "subwidget/dialoginstance.h"

#include "model/sqlitehandler.h"
#include "model/httprequest.h"

#define SHOW_QRENCODE 120

#define PAY_WIDGET_ACTIVITY_TIME 120

#include <QStackedWidget>
#include "configure/app.h"

class PayWidget : public BaseSubWidget
{
    Q_OBJECT
public:
    explicit PayWidget(QStackedWidget *parent = 0);

    void set_context(int value){
        set_goods_image(value);
        set_payment_logo();
        GoodInfo info;
        SqliteHandler::getInstence()->table_select_price_stock(App::goodsTableName,value,info);
        label_info->setText(QString("<font size=1>(货道:%1 &nbsp;库存:%2 &nbsp;价格:%3元)</font>")
                            .arg(QString::number(value))
                            .arg(QString::number(info.getStock()))
                            .arg(info.getPrice()));
        this->show();
        stop_timer_loading();
        stop_timer_close();
        if(info.getStock() > 0){
            get_qrcode(value);
            timer_loading->start(5000);
            start_close_time(10);
        }else{
            DialogInstance::getInstance()->setText("此商品已售完");
            start_close_time(5);
        }
    }
    //获取二维码
    void get_qrcode(int value){
        switch (App::payType) {
            case HttpRequest::ALIPAY:
                HttpRequest::getInstance()->send_request_qrcode(HttpRequest::ALIPAY,value);
                break;
            case HttpRequest::WEIXIN:
                HttpRequest::getInstance()->send_request_qrcode(HttpRequest::WEIXIN,value);
                break;
            case HttpRequest::WEIXIN_AND_ALIPAY:
                HttpRequest::getInstance()->send_request_qrcode(HttpRequest::ALIPAY,value);
                HttpRequest::getInstance()->send_request_qrcode(HttpRequest::WEIXIN,value);
                break;
            default:
                DialogInstance::getInstance()->setText("未开通移动支付");
                break;
        }
    }
    void set_payment_logo(){
        QPixmap weixin("/home/ico/weixin.png");
        QPixmap alipay("/home/ico/alipay.png");
        int w=label_weixin->width();
        int h=label_weixin->height();
        label_weixin->setPixmap(weixin.scaled(w,h,Qt::KeepAspectRatio));
        label_alipay->setPixmap(alipay.scaled(w,h,Qt::KeepAspectRatio));
    }
    //显示二维码
    void set_qrcode(int payment,QString qrcode){
        show_qrencode(payment,qrcode);
    }

    void stop_timer_loading(){
        if(timer_loading->isActive()){
            timer_loading->stop();
        }
    }
    void stop_timer_close(){
        if(timer_close->isActive()){
            timer_close->stop();
        }
    }
    void start_close_time(int value){
        close_cnt=value;
        label_time->setText(QString::number(value)+"秒");
        timer_close->start(1000);
    }

  signals:
    void sub_widget_data(int widget,int data);
  private slots:
      //加载二维码失败
      void loading_time_out(){
          DialogInstance::getInstance()->setText("加载二维码失败");
          stop_timer_loading();
          //stop_timer_close();
          //timer_close->start(5000);
      }
      void timer_close_slot(){
          if(--close_cnt >= 0){
              label_time->setText(QString::number(close_cnt)+"秒");
          }else{
              stop_timer_loading();
              stop_timer_close();
              emit sub_widget_data(1,0);
          }
      }
      void button_close_slot(){
          stop_timer_loading();
          stop_timer_close();
          emit sub_widget_data(1,0);
      }

      void show_qrencode(int payment,QString str){
          QRcode *qr = QRcode_encodeString(str.toStdString().c_str(),4,//指定二维码等级,L,M,Q,H四个等级，分别表示7%,15%,25%,30%，容错率越高，可被遮挡的面积越大
                                    QR_ECLEVEL_L,QR_MODE_8,1);
          if(qr != NULL){
              stop_timer_loading();
              start_close_time(120);
              int width =150;
              int height=150;
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
              for( int y = 0; y < qr_width; y++){
                  for(int x = 0; x < qr_width; x++){
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
              if(payment==HttpRequest::WEIXIN){
                  logo.load("/home/ico/logo_weixin.png");
              }else{
                  logo.load("/home/ico/logo_alipay.png");
              }
              QPixmap pix=QPixmap::fromImage(image);
              QPainter p(&pix);
              p.setRenderHint(QPainter::SmoothPixmapTransform, true);
              p.drawPixmap((150*0.8)/2,(150*0.8)/2,150*0.2,150*0.2,logo);
              if(payment==HttpRequest::WEIXIN){
                  label_weixin->setPixmap(pix);
              }else{
                  label_alipay->setPixmap(pix);
              }
          }
      }
private:
    QLabel *label_weixin;
    QLabel *label_alipay;
    QPushButton *button_close;
    QLabel *label_image;
    QLabel *label_info;
    QLabel *label_time;

    int current_payment;
    int close_cnt;

    QTimer *timer_loading;
    QTimer *timer_close;

    bool set_goods_image(int select){
        bool ok=true;
        QString img=QString("%1/%2").arg(IMAGE_PATH,QString::number(select));
        QPixmap pix(img);
        int w=label_image->width();
        int h=label_image->height();
        pix=pix.scaled(w,h,Qt::KeepAspectRatio);
        label_image->setPixmap(pix);
        return ok;
    }
};

#endif // PAYWIDGET_H
