#include "numericalkeyboard.h"

NumericalKeyboard::NumericalKeyboard(QWidget *parent) : QWidget(parent)
{
    for (int i = 0; i < KEY_NUM; i++){
        key[i] = new QPushButton(this);
        key[i]->setFocusPolicy(Qt::NoFocus);
        key[i]->setFlat(true);
        key[i]->setMaximumSize(300,100);
        key[i]->setMinimumSize(50,50);
        connect(key[i], SIGNAL(clicked()), this, SLOT(key_clicked_slot()));
    }
    for (int i = 0; i < KEY_NUM-1; i++){
        key[i]->setText(QString(keyCode[i]));
    }
    key[KEY_NUM-2]->setText("退格");
    key[KEY_NUM-2]->setMaximumHeight(250);
    key[KEY_NUM-1]->setText("回车");
    key[KEY_NUM-1]->setMaximumHeight(250);

    gl = new QGridLayout(this);
    for(int i=0;i<2;i++){
        for(int j=0;j<5;j++){
            gl->addWidget(key[i*5+j],i,j,1,1);
        }
    }
    gl->addWidget(key[KEY_NUM-2],0,5,1,1);
    gl->addWidget(key[KEY_NUM-1],1,5,1,1);
    setLayout(gl);
}

#include <QApplication>
#include <QChar>
#include <QKeyEvent>

char NumericalKeyboard::keyCode[NumericalKeyboard::KEY_NUM] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '\b',' '};

void NumericalKeyboard::get_value(int i){
    QWidget *pFocusWidget = QApplication::focusWidget();
    if (pFocusWidget != NULL){
        switch (i){
            case KEY_BACK:{//删除
                QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, QString());
                QCoreApplication::sendEvent(pFocusWidget, &keyPress);
                QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier, QString());
                QCoreApplication::sendEvent(pFocusWidget, &keyRelease);
            }
            break;
            case KEY_ENTER:{//删除
                QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier, QString());
                QCoreApplication::sendEvent(pFocusWidget, &keyPress);
                QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier, QString());
                QCoreApplication::sendEvent(pFocusWidget, &keyRelease);
            }
            break;
            default:{
                int keyValue = keyCode[i];
                QChar ch=keyValue;
                QKeyEvent keyPress(QEvent::KeyPress, ch.unicode(), Qt::NoModifier, QString(ch));
                QCoreApplication::sendEvent(pFocusWidget, &keyPress);
                QKeyEvent keyRelease(QEvent::KeyRelease, ch.unicode(), Qt::NoModifier, QString());
                QCoreApplication::sendEvent(pFocusWidget, &keyRelease);
            }
            break;
        }
    }
}
