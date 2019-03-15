#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QDebug>

class SwitchButton : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchButton(QWidget *parent = 0);
    //设置是否选中
    void setChecked(bool _checked){
//        if (this->checked!=_checked) {
            this->checked=_checked;
            this->update();
//        }
    }
    bool getChecked()const{
        return checked;
    }
    int getChecked_num(){
        if(checked){
            return 1;
        }
        return 0;
    }
private:
    bool checked;           //是否选中
    static QString imageOff;//关闭时显示的图片
    static QString imageOn; //打开时显示的图片
protected:
    void paintEvent(QPaintEvent *){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);//绘制准备工作,启用反锯齿
        drawImage(&painter);                          //绘制图片
    }
    void mousePressEvent(QMouseEvent *){
        checked = !checked;
        this->update();
    }
private:
    void drawImage(QPainter *painter){
//        painter->save();
        QPixmap pix;
        if(checked) {
            pix = QPixmap(imageOn);
        } else {
            pix = QPixmap(imageOff);
        }
        //自动等比例平滑缩放居中显示
//        int targetWidth = pix.width();
//        int targetHeight = pix.height();
//        pix = pix.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        int pixX = rect().center().x() - targetWidth / 2;
//        int pixY = rect().center().y() - targetHeight / 2;
//        QPoint point(pixX, pixY);
//        painter->drawPixmap(point, pix);
//        painter->restore();

        painter->drawPixmap(0,0,pix);
    }
};

#endif // SWITCHBUTTON_H
