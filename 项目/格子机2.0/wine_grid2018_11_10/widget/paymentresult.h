#ifndef PAYMENTRESULT_H
#define PAYMENTRESULT_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QBasicTimer>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMovie>
#include "qrencode.h"
#include "app.h"

#include <QDebug>

#include "Serial/serialputout.h"

#include <QTimer>

class PaymentResult : public QWidget
{
    Q_OBJECT
public:
    explicit PaymentResult(QWidget *parent = 0);

    void loading(){
        btnFlag=noready;
        labelQR->setMovie(movie);
        btn->setText("加载中...");
        movie->start();
        this->raise();
        this->show();
    }
    void qrencode_show(const QString str,const int pay){
        btnFlag=noready;
        movie_stop();
        if(pay==App::buyAlipay){
            set_qrencode(str,"/home/ico/logo_alipay.png");
            btn->setText("请用手机扫码支付");
            this->raise();
            this->show();
        }
        else if(pay==App::buyWeixin){
            set_qrencode(str,"/home/ico/logo_weixin.png");
            btn->setText("请用手机扫码支付");
            this->raise();
            this->show();
        }
        timer->start(60000);
    }
    void card_place(){
        btnFlag=noready;
        labelQR->setText("请放置会员卡");
        btn->setText("");
        this->raise();
        this->show();
    }
    void set_type_weight(int _type,int _weight){
        weight=_weight;
        type=_type;
    }

    void wait_open_ack(){
        btnFlag=noready;
        labelQR->setText("请等待机器准备就绪");
        btn->setText("");
        this->raise();
        this->show();
    }
    void put_out_ready(){
        btnFlag=ready;
        movie_stop();
//        labelQR->setText("<p display=inline-block text-align=left >"//text-align=left align=left padding-left:30px
//                             "<font color=red >"
//                                 "●请打开门<br/>"
//                                 "●请放入容器<br/>"
//                                 "●请关门<br/>"
//                                 "●请点击出酒按钮"
//                             "</font>"
//                         "</p>");

        labelQR->setText(
            "<table align=center color=red > \
            <tr><td align=right>●</td>  <td>请打开门</td></tr> \
            <tr><td align=right>●</td>  <td>请放入容器</td></tr> \
            <tr><td align=right>●</td>  <td>请关门</td></tr> \
            <tr><td align=right>●</td>  <td>请点击出酒按钮</td></tr> \
            </table>"
        );
        btn->setText("");
        this->raise();
        this->show();
    }
    void put_out_ing(){
        btnFlag=ing;
        labelQR->setText("正在出酒，请耐心等待");
        btn->setText("停止出酒");
        this->raise();
        this->show();
    }
    void put_out_stop(){
        btnFlag=noready;
        labelQR->setText("已停止出酒");
        btn->setText("");
        this->raise();
        this->show();
    }
    void put_out_ok(){
        btnFlag=noready;
        QString str=QString("成功出酒%1g，请取走").arg(QString::number(App::weight));
        labelQR->setText(str);
        btn->setText("");
        this->raise();
        this->show();
    }
    void put_out_ok(int ml){
        btnFlag=noready;
        QString str=QString("成功出酒%1ML，请取走").arg(QString::number(ml));
        labelQR->setText(str);
        btn->setText("");
        this->raise();
        this->show();
    }
    void put_out_failed(){
        btnFlag=noready;
        labelQR->setText(
            "<table align=center > \
                <tr><td align=center ><font size=3 >出酒失败</font> </td></tr> \
                <tr><td align=center ><font size=1 >提示:已支付金额将退给您</font> </td></tr> \
            </table>"
                    );
        btn->setText("");
        this->raise();
        this->show();
    }
    void widget_clear(){
        timer->stop();
        movie_stop();
        this->hide();
        App::card="";
    }
    void movie_stop(){
        movie->stop();
        btn->setText("");
    }
    void set_serial(SerialPutOut *_serialPutOut){
        serialPutOut=_serialPutOut;
    }
private slots:
    void btn_slot(){
        switch (btnFlag) {
//            case ready:
//                if(weight>=0){
//                    serialPutOut->open_box(type,weight);
//                }
//                break;
            case ing:
                serialPutOut->open_stop();
                break;
            default:break;
        }
    }

    void timer_slot(){
        widget_clear();
    }

private:
    QPushButton *btnClose;
    QLabel *labelQR;
    QPushButton *btn;
    QMovie *movie;
    QTimer *timer;

    uint weight;
    int type;
    int btnFlag;
    enum {noready,ready,ing,ok};
    SerialPutOut *serialPutOut;

    void set_qrencode(const QString &str,const QString &img){
           //指定二维码等级,L,M,Q,H四个等级，分别表示7%,15%,25%,30%，容错率越高，可被遮挡的面积越大
             QRcode *qr = QRcode_encodeString(str.toStdString().c_str(),2,QR_ECLEVEL_L,QR_MODE_8,1);
             if(qr != NULL){
                 int width=labelQR->width();
                 width=120;//=width<120?120:width*0.9
                 int height=width;
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
                         unsigned char b = qr->data[y * qr_width + x];//qrcode->data包含了二维码图像的信息,bit0代表了点的颜色,1表示黑色，0表示白色。
                         if(b & 0x01) {
                             QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                             painter.drawRects(&r, 1);
                         }
                     }
                 }
                 QRcode_free(qr);
                 QPixmap logo(img);
                 QPixmap pix=QPixmap::fromImage(image);
                 QPainter p(&pix);
                 p.setRenderHint(QPainter::SmoothPixmapTransform, true);
                 p.drawPixmap((width-width*0.25)/2,(width-width*0.25)/2,width*0.25,width*0.25,logo);
                 labelQR->setPixmap(pix);
             }
       }

protected:
    void paintEvent(QPaintEvent *){
         QStyleOption opt;
         opt.init(this);
         QPainter p(this);
         style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
     }
};

#endif // PAYMENTRESULT_H
