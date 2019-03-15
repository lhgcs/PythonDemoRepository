#ifndef UPDATESTATUS_H
#define UPDATESTATUS_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QCheckBox>
#include <QMouseEvent>
#include "shoppingcart.h"
#include <QDebug>

#include <QStyleOption>

#include <QJsonObject>
#include <QJsonDocument>

#include "my_define.h"
#include <QVector>
#include <math.h>

class UpdateStatus : public QWidget//QDialog//QWidget
{
    Q_OBJECT
public:
    UpdateStatus(int *const _status_p, QWidget *parent = 0);//explicit

    void data_sync()
    {
        for(int i=0;i<GOODS_TOTAL;i++)
        {
           status_p[i]=temp_status[i];
        }
    }
    void show_it()
    {
        index=0;
        for(int i=0;i<GOODS_TOTAL;i++)
        {
           temp_status[i]=status_p[i];
        }
        change_text(index);
        this->show();
    }
    void get_reset_vector(QVector<int> &p)//QVector<int> *p
    {
        p.clear();
        for(int i=0;i<vector_reset.size();i++)
        {
            p.append(vector_reset.at(i));
        }
    }
    void set_busy(bool busy)
    {
        isbusy=busy;
    }

signals:
    void send_up_data(QString str);
public slots:
    void button_all_slot()
    {
        for(int i=0;i<20;i++) {
            if(all_flag){
                checkBox[i].setCheckState(Qt::Checked);
            }
            else
            {
                checkBox[i].setCheckState(Qt::Unchecked);
            }
        }
        all_flag=!all_flag;
    }

    void button_top_up_slot()
    {
        for(int i=0;i<20;i++) {
             if(checkBox[i].isChecked()){
                 if(top_up_flag) {
                    button[i].setText("22");
                 }
                 else {
                    button[i].setText("0");
                 }
             }
        }
        top_up_flag=!top_up_flag;
    }

    void button_up_slot()
    {
        int num=index==0?0:pow(2,index)*10;
        for(int i=0;i<20;i++)
        {
            temp_status[num+i]=button[i].getText();
        }
        QJsonObject json;
        for(int i=0;i<GOODS_TOTAL;i++)
        {
           json.insert("box"+QString::number(i+1), temp_status[i]);
        }
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        emit send_up_data(QString(byte_array));
    }

    void button_down_slot()
    {
        emit send_up_data(NULL);
    }

    void button_previous_slot()
    {
        int num=index==0?0:pow(2,index)*10;
        for(int i=0;i<20;i++)
        {
            temp_status[num+i]=button[i].getText();
        }
        if(--index<0)index=2;
        all_flag=true;
        top_up_flag=true;
        change_text(index);
    }

    void button_next_slot()
    {
        int num=index==0?0:pow(2,index)*10;
        for(int i=0;i<20;i++)
        {
            temp_status[num+i]=button[i].getText();
        }
        if(++index>2)index=0;
        all_flag=true;
        top_up_flag=true;
        change_text(index);
    }

    void button_reset_slot()
    {
        if(!isbusy)
        {
            vector_reset.clear();
            for(int i=0;i<20;i++)
            {
                if(checkBox[i].isChecked())
                {
                    QString str=checkBox[i].text();
                    vector_reset.append(str.toInt());
                }
            }
            if(vector_reset.size()>0)
            {
                emit send_up_data("reset");
            }
        }
        else
        {
            emit send_up_data("busy");
        }
    }

private:
    int index;
    int *status_p;
    int temp_status[GOODS_TOTAL];
    QCheckBox* checkBox;
    ShoppingCart* button;
    QPushButton *button_all;
    QPushButton *button_top_up;
    QPushButton *button_close;
    QPushButton *button_reset;
    QPushButton *button_up, *button_down;
    QPushButton *button_previous,*button_next;
    QVector<int> vector_reset;
    bool all_flag;
    bool top_up_flag;
    bool isbusy;

    void change_text(int index)
    {
        int num=index==0?0:pow(2,index)*10;
        for(int i=0;i<20;i++)
        {
             checkBox[i].setText(QString::number(num+i+1));
             if(temp_status[num+i]>0)
             {
                 checkBox[i].setCheckState(Qt::Checked);
             }
             else
             {
                 checkBox[i].setCheckState(Qt::Unchecked);
             }
             button[i].setText(QString::number(temp_status[num+i]));
        }
    }
};

#endif // UPDATESTATUS_H
