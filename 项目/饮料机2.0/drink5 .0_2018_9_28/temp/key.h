#ifndef __KEY_H__
#define __KEY_H__

#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include "httprequest.h"
#include "control_state.h"

class QPushButton;
class Key : public QWidget {
    Q_OBJECT
public:
    Key(int status,QWidget *parent = NULL);
    ~Key()
    {
        for(int i=0;i<KEY_NUM;i++)
        {
            delete ppbKey[i];
        }
        delete lineEdit;
        delete con;
         qDebug()<<"~~~~~~~~~~~key";
    }

private:
    void updateKeyText();
    void updateAlphaKeyText(bool bUpper);

    void closeEvent(QCloseEvent *)
    {
        con=NULL;
        emit sub_widget_close();
    }
signals:
    void sub_widget_close();
    void has_goods(int value);
    void open_box(unsigned char data_h,unsigned char data_l);

private slots:
    void rxClicked();
    void send_data(int value)
    {
        emit has_goods(value);
    }
    void send_data(unsigned char data_h,unsigned char data_l)
    {
        emit open_box(data_h,data_l);
    }

private:
     enum {
         KEY_0, KEY_1, KEY_2, KEY_3,KEY_4, KEY_5, KEY_6, KEY_7,KEY_8, KEY_9,
         KEY_A, KEY_B, KEY_C, KEY_D,KEY_E, KEY_F, KEY_G,
         KEY_H, KEY_I, KEY_J, KEY_K, KEY_L,KEY_M, KEY_N,
         KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
         KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

         KEY_BACK,  //删除
         KEY_SPACE, //空格
         KEY_RETURN,//退出
         KEY_ENTER, //回车
         KEY_UPPER, //
         KEY_NUM,
     };
    QPushButton *ppbKey[KEY_NUM];
    QLineEdit *lineEdit;
    Control_State *con;
    bool bUpper;
    static char keyCode[KEY_NUM];
    int status;
};

#endif // KEY_H
