#ifndef SELETEMONEYPAYMENT_H
#define SELETEMONEYPAYMENT_H

#include <QWidget>
#include <QLabel>
#include "paymentresult.h"
#include "httprequest.h"
#include "Sqlite/sqlitegoods.h"
#include "dialog.h"

#include "toast.h"

class SeleteMoneyPayment : public QWidget
{
    Q_OBJECT
public:
    explicit SeleteMoneyPayment(QWidget *parent = 0);
    ~SeleteMoneyPayment(){
        delete label;
        delete btnClose;
        delete btn;
        delete vl;
    }

    HttpRequest* httpRequest;
    PaymentResult *pay;
    QString totalMoney;
    SerialPutOut *serialPutOut;

    float price;
    void set_tip(int menoy){
        if(price<=0){
            price=1;
        }
        QString buy=QString::number(menoy/price*500);
        QString text=QString(
            "<p text-align=left >"
                "<font size=3>"
                    "已投币: %1元,可购买%2ml<br/>"
                    "可投币: %3元<br/>"
                "</font>"
                "<font size=2 color=red >"
                    "<br/>"
                    "温馨提示:<br/>"
                    "●投币不要超过最大允许值<br/>"
                    "●投币只支持1/5/10/20/50/100元<br/>"
                    "●投币时请缓慢依次投入<br/>"
                    "●投币完成请点击“支付”按钮"
                "</font>"
            "</p>").arg(QString::number(menoy),buy,totalMoney);
        label->setText(text);
    }
    void set_tip(){
        QString text=QString(
            "<p text-align=left>"
                "<font size=5>"
                    "正在支付%1元,请耐心等待"
                "</font>"
            "</p>").arg(QString::number(App::paperMoney));
        label->setText(text);
    }

signals:
    void destroy_widget();
private:
    QLabel *label;
    QPushButton *btnClose,*btn;
    QVBoxLayout *vl;
private slots:
    void btnClose_slot(){
        this->close();
        serialPutOut->close_banknote();
        emit destroy_widget();
    }
    void btn_slot(){
        if(httpRequest){
            if(App::paperMoney>0){
                httpRequest->send_Request(App::buyMoney);
                set_tip();
                btn->setEnabled(false);
                serialPutOut->close_banknote();
                App::paperMoney=0;
                serialPutOut->close_banknote();
            }
            else{
                new Toast("购买请投币");
            }
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

#endif // SELETEMONEYPAYMENT_H
