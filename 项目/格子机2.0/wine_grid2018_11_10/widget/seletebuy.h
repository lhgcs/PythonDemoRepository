#ifndef SELETEBUY_H
#define SELETEBUY_H

#include <QWidget>
#include "basewidget.h"
#include <QToolButton>
#include <QHBoxLayout>
#include "app.h"

#include "paymentresult.h"
#include "IC/serialclient.h"

#include "seletemobilepayment.h"
#include "seletemoneypayment.h"
#include "seletecardpayment.h"
#include "Sqlite/sqlitegoods.h"

class SeleteBuy : public QWidget
{
    Q_OBJECT
public:
    explicit SeleteBuy(QWidget *parent = 0);

    PaymentResult *paymentResult;
    SeleteCardPayment *seleteCardPayment;
    SeleteMoneyPayment *seleteMoneyPayment;
    HttpRequest *httpRequest;
    SqliteGoods *sql;
    SerialPutOut *serialPutOut;
    int currentBox;

    void widget_clear(){
        if(seleteMobilePayment){
            seleteMobilePayment->close();
            seleteMobilePayment->deleteLater();
            seleteMobilePayment=NULL;
        }
        if(seleteCardPayment){
            seleteCardPayment->close();
            seleteCardPayment->deleteLater();
            seleteCardPayment=NULL;
        }
        if(seleteMoneyPayment){
            seleteMoneyPayment->close();
            seleteMoneyPayment->deleteLater();
            seleteMoneyPayment=NULL;
        }
        if(paymentResult){
            paymentResult->widget_clear();
        }
    }

    void select_clear(){
        if(seleteMobilePayment){
            seleteMobilePayment->close();
            seleteMobilePayment->deleteLater();
            seleteMobilePayment=NULL;
        }
        if(seleteCardPayment){
            seleteCardPayment->close();
            seleteCardPayment->deleteLater();
            seleteCardPayment=NULL;
        }
        if(seleteMoneyPayment){
            seleteMoneyPayment->close();
            seleteMoneyPayment->deleteLater();
            seleteMoneyPayment=NULL;
        }
    }

signals:
    void data(int layer,int value);
private slots:
    void btnMoney_slot(){
        if(httpRequest->isBusy){
            new Dialog("请等待上一单交易完成再购买");
        }
        else{
            seleteMoneyPayment=new SeleteMoneyPayment(this);
            seleteMoneyPayment->httpRequest=httpRequest;
            seleteMoneyPayment->pay = paymentResult;
            seleteMoneyPayment->serialPutOut=serialPutOut;
            int totalMoney=sql->get_table_total_value_data(currentBox);
            seleteMoneyPayment->totalMoney=QString::number(totalMoney);
            seleteMoneyPayment->price=sql->get_table_price_data(QString::number(currentBox));
            seleteMoneyPayment->set_tip(App::paperMoney);
            QObject::connect(seleteMoneyPayment,SIGNAL(destroy_widget()),this,SLOT(destroy_widget_slot()));
            serialPutOut->open_banknote();
        }
    }
    void btnMobile_slot(){
        if(httpRequest->isBusy){
            new Dialog("请等待上一单交易完成再购买");
        }
        else{
            seleteMobilePayment=new SeleteMobilePayment(this);
            seleteMobilePayment->httpRequest=httpRequest;
            seleteMobilePayment->pay = paymentResult;
            seleteMobilePayment->sql = sql;
            seleteMobilePayment->currentBox = currentBox;
            QObject::connect(seleteMobilePayment,SIGNAL(destroy_widget()),this,SLOT(destroy_widget_slot()));
        }
    }
    void btnCard_slot(){
        if(httpRequest->isBusy){
            new Dialog("请等待上一单交易完成再购买");
        }
        else{
            seleteCardPayment=new SeleteCardPayment(this);
            seleteCardPayment->httpRequest=httpRequest;
            seleteCardPayment->pay = paymentResult;
            seleteCardPayment->sql = sql;
            seleteCardPayment->currentBox = currentBox;
            QObject::connect(seleteCardPayment,SIGNAL(destroy_widget()),this,SLOT(destroy_widget_slot()));
            QObject::connect(httpRequest,SIGNAL(http_card()),seleteCardPayment,SLOT(set_tip()));
        }
    }
    void destroy_widget_slot(){
        widget_clear();
    }

private:
    QToolButton *btnMoney,*btnMobile,*btnCard;
    SeleteMobilePayment *seleteMobilePayment;
    QVBoxLayout *vl;
};

#endif // SELETEBUY_H
