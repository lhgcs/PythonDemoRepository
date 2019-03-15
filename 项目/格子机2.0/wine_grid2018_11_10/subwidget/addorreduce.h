#ifndef ADDORREDUCE_H
#define ADDORREDUCE_H

#include "basesubwidget.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QCheckBox>

class AddOrReduce : public BaseSubWidget
{
    Q_OBJECT
public:
    explicit AddOrReduce(QWidget *parent = 0);
    ~AddOrReduce(){
        delete btnAdd;
        delete btnSub;
        delete label;
        delete h;
    }
    void set_limit(int min,int max){
        minLimit=min;
        maxLimit=max;
    }

    void set_id(int value){
        id=value;
//        QString str;
//        if(value<10){
//            str=QString("%1 ").arg(QString::number(value));
//        }else{
//            str=QString::number(value);
//        }
        checkBox->setText(QString::number(value));
    }
    int get_id(){
        return checkBox->text().toInt();
    }
    int get_num(){
        return label->text().toInt();
    }
    QString get_num_str(){
        return label->text();
    }
    void set_num_min(){
        label->setText(QString::number(minLimit));
    }
    void set_num_max(){
        label->setText(QString::number(maxLimit));
    }
    void set_num(int value){
        label->setText(QString::number(value));
    }
    bool getCheckBox_status() const{
        if(checkBox->checkState() == Qt::Checked){
            return true;
        }
        return false;
    }
    void setCheckBox_status(bool status){
        if(status){
            checkBox->setCheckState(Qt::Checked);
        }else{
            checkBox->setCheckState(Qt::Unchecked);
        }
    }

    void set_size(int w,int h){
        this->setMaximumSize(w,h);
        btnAdd->setMaximumSize(w/3,h/3);
        btnSub->setMaximumSize(w/3,h/3);
        label->setMaximumSize(w/3,h/3);
    }

    void set_Style(){
        this->setStyleSheet(
        "#AddOrReduce{"
            "background-color:rgba(0,0,0,0);"//232,241,252
            "border:none;"
        "}"
        "QLabel{"
            "font:26px;"
            "color:rgb(0,0,0);"
            "background-color:rgba(0,0,0,0);"
            "border:none;"
        "}"
        "QPushButton{"
            "font:28px;"
            "color:rgb(255,0,0);"
            "background-color:rgba(0,0,0,0);"
            "border:1px solid rgba(0,0,0,200);"//"border-radius:2px;"
        "}"
    );
    }

private:
    int id;
    QPushButton *btnAdd;
    QPushButton *btnSub;
    QLabel *label;
    QCheckBox *checkBox;
    QHBoxLayout *h;
    int minLimit;
    int maxLimit;

private slots:
    void button_add_slot(){
        int num=label->text().toInt();
        if(num<maxLimit){
            num++;
            label->setText(QString::number(num));
        }
    }

    void button_sub_slot(){
        int num=label->text().toInt();
        if(num>minLimit){
            num--;
            label->setText(QString::number(num));
        }
    }
};

#endif // ADDORREDUCE_H
