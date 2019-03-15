#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QCheckBox>
#include <QVector>
#include "shoppingcart.h"
#include <QPushButton>
#include "my_define.h"
class Update:public QTableWidget
{
    Q_OBJECT
public:
    Update(int *_status_p,QWidget *parent=0);
    void closeEvent(QCloseEvent *)
    {
        emit sub_widget_close();
    }
    ~Update()
    {
        for(int i=0;i<GOODS_TOTAL;i++)
        {
            delete checkBox[i];
            delete button[i];
        }
        delete button_all;
        delete button_up;
        delete button_down;
        delete button_close;
    }
signals:
    void sub_widget_close();
    void send_up_data();
    void send_down_data();

private slots:
    void button_all_slot()
    {
        static bool flag=true;
        for(int i=0;i<GOODS_TOTAL;i++)
        {
            if(flag)
            {
                checkBox[i]->setCheckState(Qt::Checked);
            }
            else
            {
                checkBox[i]->setCheckState(Qt::Unchecked);
            }
        }
        flag=!flag;
    }
    void button_up_slot()
    {
        emit send_up_data();
    }
    void button_down_slot()
    {
        emit send_down_data();
    }

private:
    int *status_p;
    QVector<QCheckBox*> checkBox;
    QVector<ShoppingCart*> button;
    QPushButton *button_all;
    QPushButton *button_up;
    QPushButton *button_down;
    QPushButton *button_close;
};

#endif // UPDATE_H
