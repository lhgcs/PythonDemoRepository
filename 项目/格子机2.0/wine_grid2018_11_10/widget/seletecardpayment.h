#ifndef SELETECARDPAYMENT_H
#define SELETECARDPAYMENT_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include "numinputdialog.h"
#include <QPushButton>

#include <QStyleOption>
#include <QPainter>

#include <QAction>

#include "dialog.h"
#include "paymentresult.h"
#include "httprequest.h"
#include "toast.h"

#include "Sqlite/sqlitegoods.h"

#include "app.h"

class SeleteCardPayment : public QWidget
{
    Q_OBJECT
public:
    explicit SeleteCardPayment(QWidget *parent = 0);
    ~SeleteCardPayment(){
        delete pTrailingAction;
        delete btnClose;
        delete numInputDialog;
        delete label;
        delete vl;
    }

    HttpRequest* httpRequest;
    PaymentResult *pay;
    QLineEdit *lineEdit;

    SqliteGoods *sql;
    int currentBox;

signals:
    void destroy_widget();
private slots:
    void set_tip(){
        QString str=QString("会员:%1<br/>积分:%2").arg(App::cardName,App::cardIntegration);
        label->setText(str);
    }
    void numInput_slot(int value){
        if(httpRequest->isBusy){
            new Dialog("请等待上一单交易完成再购买");
        }
        else{
            switch (value){
                case 11:{
                    lineEdit->setFocus();
                    if(lineEdit->text().length()<2){
                        new Dialog("购买量10ML起");
                        return;
                    }
                    int remain=0;
                    if(sql){
                        remain=sql->get_table_remain_data(QString::number(currentBox));
                    }
                    QString buy=lineEdit->text();
                    if(buy.toInt()>remain){
                        QString str=QString("已超过最大购买量%1ML\n请重新输入").arg(QString::number(remain));
                        new Dialog(str);
                        return;
                    }
                    if(App::card.isEmpty()){
                        new Toast("请放置会员卡");
                    }
                    else{
                        pay->loading();
                        if(httpRequest){
                            httpRequest->send_Request(App::buyCard,lineEdit->text(),App::card);
                        }
                        label->setText("请刷卡");
                        App::card.clear();
                    }
                    break;
                }
                case 10:{
                    QString str=this->lineEdit->text();
                    this->lineEdit->setText(str.left(str.length()-1));//chop(1)
                    break;
                }
                default:this->lineEdit->setText(this->lineEdit->text().append(value+48));
                        break;
            }
        }
    }
    void btnClose_slot(){
        App::card="";
        this->close();
        emit destroy_widget();
    }
    void btn_slot(){
        if(lineEdit->text().isEmpty()){
            new Dialog("请输入购买酒的数量");
            return;
        }
    }
    void clear_slot(bool value){
        lineEdit->clear();
        qDebug()<<value;
    }
private:
    QPushButton *btnClose;
    NumInputDialog *numInputDialog;
    QLabel *label;
    QVBoxLayout *vl;

    QAction *pTrailingAction;

protected:
    void paintEvent(QPaintEvent *){
         QStyleOption opt;
         opt.init(this);
         QPainter p(this);
         style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
     }
};

#endif // SELETECARDPAYMENT_H
