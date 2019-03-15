#ifndef LOADINGQRCODE_H
#define LOADINGQRCODE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include <QMovie>
#include "qrencode.h"
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QDialog>

class LoadingQrcode : public QDialog
{
    Q_OBJECT
public:
    explicit LoadingQrcode(QWidget *parent = 0);

    void loading_start(){
        label->setMovie(movie);
        labelTip->setText("正在加载支付二维码");
        labelTip->adjustSize();
        this->raise();
        this->show();
        movie->start();
        timerClose->start(30000);
    }
    void loading_start_card(){
        label->setMovie(movie);
        labelTip->setText("数据加载中");
        labelTip->adjustSize();
        this->raise();
        this->show();
        movie->start();
        timerClose->start(30000);
    }
    void loading_stop(){
        movie->stop();
        timerClose->stop();
    }
private:
    QLabel *label;
    QLabel *labelTip;
    QPushButton *btnClose;
    QVBoxLayout *vl;
    QTimer *timerClose;
    QMovie *movie;

public:
    void set_qrencode(const QString &str,const QString &img){
             //指定二维码等级,L,M,Q,H四个等级，分别表示7%,15%,25%,30%，容错率越高，可被遮挡的面积越大
               QRcode *qr = QRcode_encodeString(str.toStdString().c_str(),2,QR_ECLEVEL_L,QR_MODE_8,1);
               if(qr != NULL){
                   int width=180;
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
                   p.drawPixmap((width-width*0.2)/2,(width-width*0.2)/2,width*0.2,width*0.2,logo);
                   label->setPixmap(pix);
                   labelTip->setText("请用手机扫描二维码支付");
                   timerClose->start(60000);
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

#endif // LOADINGQRCODE_H
