#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <qwidget.h>
#include <QObject>
#include <QPushButton>
#include <QBoxLayout>

class VirtualKeyboard : public QWidget {
    Q_OBJECT
public:
    VirtualKeyboard(QWidget *parent = NULL);
    ~VirtualKeyboard(){
        for(int i=0;i<KEY_NUM;i++){
            delete ppbKey[i];
        }
        delete []phLayout;
        delete pvLayout;
    }
signals:
    void send_data(int value);
private slots:
    void rxClicked();
private:
    enum {
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7,KEY_8, KEY_9,
        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,
        KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
        KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
        KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
        KEY_BACK,  //删除
        KEY_SPACE, //空格
        KEY_RETURN,//退出
        KEY_ENTER, //回车
        KEY_UPPER, //大写
        KEY_NUM,
    };
    char keyCode[KEY_NUM];
    QHBoxLayout *phLayout;
    QVBoxLayout *pvLayout;
    QPushButton *ppbKey[KEY_NUM];
    bool bUpper;

    void get_value(int i);
    void key_press(QWidget *pFocusWidget,int key);
    void key_press(QWidget *pFocusWidget,int key,QChar ch);

    void updateKeyText();
    void updateAlphaKeyText(bool bUpper);
};

#endif // VIRTUALKEYBOARD_H
