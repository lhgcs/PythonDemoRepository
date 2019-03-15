#ifndef SHOPPINGCART_H
#define SHOPPINGCART_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>

class ShoppingCart:public QWidget
{
    Q_OBJECT
public:
    ShoppingCart(QWidget *parent=0);
    ~ShoppingCart(){
        delete button_add;
        delete button_sub;
        delete label;
    }
    void paintEvent(QPaintEvent *){
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
    void setText(QString txt){        
        if(txt=="0"){
            label->setStyleSheet("color:rgb(255,0,0);");
        }
        else{
            label->setStyleSheet("color:rgb(0,255,0);");
        }
        label->setText(txt);
    }
    int getText(){
        return label->text().toInt();
    }
private:
    QPushButton *button_add;
    QPushButton *button_sub;
    QLabel *label;

private slots:
    void button_add_slot(){
        int num=label->text().toInt();
        if(num<22){
            num++;
            setText(QString::number(num));
        }
    }

    void button_sub_slot(){
        int num=label->text().toInt();
        if(num>0){
            num--;
            setText(QString::number(num));
        }
    }
};

#endif // SHOPPINGCART_H
