#ifndef CONTROL_STATE_H
#define CONTROL_STATE_H

#include <QObject>
#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>
#include "my_define.h"

class Control_State : public QWidget
{
    Q_OBJECT
public:
    Control_State(int status,QWidget *parent=0);
    void closeEvent(QCloseEvent *)
    {
        emit sub_widget_close();
    }
    ~Control_State()
    {
        for(int i=0;i<GOODS_TOTAL;i++)
        {
            delete box[i];
        }
        delete total_box;
        delete button_ok;
        delete button_close;
        delete button_invert;
        qDebug()<<"~~~~~~~~~~~Control_State";
    }

signals:
    void sub_widget_close();
    void has_goods(int value);
    void open_box(unsigned char data_h,unsigned char data_l);

private slots:
    void button_ok_slot()
    {
        int value=0;
        for(int i=0;i<GOODS_TOTAL;i++)
        {
            if(box[i]->isChecked())
            {
                value|=(1<<i);
            }
        }
        status=value;
        emit has_goods(value);
        this->close();
    }

    void button_close_slot()
    {
        this->close();
    }

    void total_box_slot()
    {
        if(total_box->isChecked())
        {
            for(int i=0;i<GOODS_TOTAL;i++)
            {
                box[i]->setCheckState(Qt::Checked);
            }
        }
        else
        {
            for(int i=0;i<GOODS_TOTAL;i++)
            {
                box[i]->setCheckState(Qt::Unchecked);
            }
        }
    }

    void button_open_slot()
    {
        unsigned char data_h=0xFF,data_l=0xFF;
        for(int i=0;i<8;i++)
        {
            if(box[i]->isChecked())
            {
                data_l &=~(1<<i);
            }
        }
        for(int i=0;i<4;i++)
        {
            if(box[8+i]->isChecked())
            {
                data_h &=~(1<<i);
            }
        }
        if((data_h==0xFF)&&(data_l==0xFF))
        {
            emit open_box(0xF0,0x00);
        }
        else
        {
            emit open_box(data_h,data_l);
        }
    }

    void button_invert_slot()
    {
        if(flag)
        {
            if(status==0)
            {
                total_box->setCheckState(Qt::Checked);
                for(int i=0;i<GOODS_TOTAL;i++)
                {
                    box[i]->setCheckState(Qt::Checked);
                }
            }
            else
            {
                total_box->setCheckState(Qt::Unchecked);
                for(int i=0;i<GOODS_TOTAL;i++)
                {
                    if(status&(1<<i))
                    {
                        box[i]->setCheckState(Qt::Unchecked);
                    }
                    else
                    {
                        box[i]->setCheckState(Qt::Checked);
                    }
                }
            }
        }
        else
        {
            total_box->setCheckState(Qt::Unchecked);
            for(int i=0;i<GOODS_TOTAL;i++)
            {
                box[i]->setCheckState(Qt::Unchecked);
            }
        }
        flag=!flag;
    }

private:
    QCheckBox *box[GOODS_TOTAL];
    QCheckBox *total_box;
    QPushButton *button_ok;
    QPushButton *button_close;
    QPushButton *button_open_all;
    QPushButton *button_invert;
    int status;
    bool flag;
};

#endif // CONTROL_STATE_H
