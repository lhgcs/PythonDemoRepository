#ifndef NUMERICALKEYBOARD_H
#define NUMERICALKEYBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

class NumericalKeyboard : public QWidget
{
    Q_OBJECT
public:
    explicit NumericalKeyboard(QWidget *parent = 0);
private slots:
    void key_clicked_slot(){
        QObject *pObj = sender();//返回发送信号的对象的指针，返回类型为QObject*          =qobject_cast<QTimeEdit *>(sender());
        for (int i = 0; i < KEY_NUM; i++){
            if (key[i] == pObj){
               get_value(i);
            }
        }
    }

private:
    enum {
        KEY_0, KEY_1, KEY_2, KEY_3,KEY_4, KEY_5, KEY_6, KEY_7,KEY_8, KEY_9,
        KEY_BACK,  //删除
        KEY_ENTER,
        KEY_NUM
    };
    QPushButton *key[KEY_NUM];
    static char keyCode[KEY_NUM];

    void get_value(int i);

    QGridLayout *gl;
};

#endif // NUMERICALKEYBOARD_H
