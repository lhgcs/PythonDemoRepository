#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <qwidget.h>
#include <QObject>

class QPushButton;
class VirtualKeyboard : public QWidget {
    Q_OBJECT
public:
    VirtualKeyboard(QWidget *parent = NULL);

private:
    void updateKeyText();
    void updateAlphaKeyText(bool bUpper);

    void closeEvent(QCloseEvent *){
        emit sub_widget_close();
    }
signals:
    void sub_widget_close();
    void send_data(int value);

private slots:
    void rxClicked();

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
    bool bUpper;
    static char keyCode[KEY_NUM];
    void get_value(int i);
};

#endif // VIRTUALKEYBOARD_H
