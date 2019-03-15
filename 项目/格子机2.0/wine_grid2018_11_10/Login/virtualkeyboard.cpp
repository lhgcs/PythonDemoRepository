#include "virtualkeyboard.h"

#define  KEY_MARGIN              5
#define  SPECIAL_ROW_OFFSET      35

VirtualKeyboard::VirtualKeyboard(QWidget *parent) : QWidget(parent)//, Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint
{
    for(int i=0;i<10;i++){
        keyCode[i]='0'+i;
    }
    for(int i=0;i<26;i++){
        keyCode[i+KEY_A]='A'+i;
    }
    keyCode[KEY_BACK]='\b'; //删除
    keyCode[KEY_SPACE]=' ';  //空格
    keyCode[KEY_RETURN]=' ';  //退出
    keyCode[KEY_ENTER]='\r'; //回车
    keyCode[KEY_UPPER]=' ';  //大写

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet(
        "QPushButton{"
            "font:32px bold;"
            "color:rgba(255,255,255,255);"
            "background-color:rgba(0,0,0,50);"
            "border:1px solid white;"
        "}"
    );

   // ppbKey= new QPushButton[KEY_NUM];
    for(int i=0; i<KEY_NUM; i++){
        ppbKey[i] = new QPushButton(this);
        ppbKey[i]->setMinimumSize(35,50);
        ppbKey[i]->setFlat(true);
        ppbKey[i]->setFocusPolicy(Qt::NoFocus);
        connect(ppbKey[i], SIGNAL(clicked()), this, SLOT(rxClicked()));
    }

    phLayout = new QHBoxLayout[5];
    for(int i=0;i<5;i++){
         phLayout[i].addSpacing(KEY_MARGIN);
         phLayout[i].addSpacing(KEY_MARGIN);
    }

    for(int i=KEY_0;i<=KEY_9;i++){
         phLayout[0].addWidget(ppbKey[i]);
         phLayout[0].addSpacing(KEY_MARGIN);
    }
    phLayout[0].addSpacing(KEY_MARGIN);

    int arr1[]={KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P};
    for(int i=0;i<10;i++){
         phLayout[1].addWidget(ppbKey[arr1[i]]);
         phLayout[1].addSpacing(KEY_MARGIN);
    }
    phLayout[1].addSpacing(KEY_MARGIN);

    int arr2[]={KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L};
    for(int i=0;i<9;i++){
         phLayout[2].addWidget(ppbKey[arr2[i]]);
         phLayout[2].addSpacing(KEY_MARGIN);
    }
    phLayout[2].addSpacing(KEY_MARGIN);

    int arr3[]={KEY_UPPER,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_BACK};
    for(int i=0;i<9;i++){
         phLayout[3].addWidget(ppbKey[arr3[i]]);
         phLayout[3].addSpacing(KEY_MARGIN);
    }
    phLayout[3].addSpacing(KEY_MARGIN);

    int arr4[]={KEY_RETURN,KEY_SPACE,KEY_ENTER};//
    for(int i=0;i<3;i++){
         phLayout[4].addWidget(ppbKey[arr4[i]]);
         phLayout[4].addSpacing(KEY_MARGIN);
    }
    phLayout[4].addSpacing(KEY_MARGIN);

    pvLayout = new QVBoxLayout(this);
    for(int i=0;i<5;i++){
        pvLayout->addSpacing(KEY_MARGIN);
        pvLayout->addLayout(&phLayout[i]);
    }
    pvLayout->addSpacing(KEY_MARGIN);
    pvLayout->setSpacing(0);
    setLayout(pvLayout);

    this->updateKeyText();
    this->bUpper=false;
    this->show();
}

void VirtualKeyboard::rxClicked()
{
    QObject *pObj = sender();//返回发送信号的对象的指针 qobject_cast<QTimeEdit *>(sender());
    for (int i=0; i<KEY_NUM; i++){
        if (ppbKey[i] == pObj){
            if(i==KEY_UPPER){
                bUpper = !bUpper;
                updateAlphaKeyText(bUpper);
            }
            else{
                get_value(i);
            }
        }
    }
}

#include <QApplication>
#include <QChar>
#include <QKeyEvent>

void VirtualKeyboard::key_press(QWidget *pFocusWidget,int key){
    QKeyEvent keyPress(QEvent::KeyPress, key, Qt::NoModifier, QString());
    QCoreApplication::sendEvent(pFocusWidget, &keyPress);
    QKeyEvent keyRelease(QEvent::KeyRelease, key, Qt::NoModifier, QString());
    QCoreApplication::sendEvent(pFocusWidget, &keyRelease);
}
void VirtualKeyboard::key_press(QWidget *pFocusWidget,int key,QChar ch){
    QKeyEvent keyPress(QEvent::KeyPress, key, Qt::NoModifier, QString(ch));
    QCoreApplication::sendEvent(pFocusWidget, &keyPress);
    QKeyEvent keyRelease(QEvent::KeyRelease, key, Qt::NoModifier, QString());
    QCoreApplication::sendEvent(pFocusWidget, &keyRelease);
}

void VirtualKeyboard::get_value(int i)
{
    QWidget *pFocusWidget = QApplication::focusWidget();
    if (pFocusWidget != NULL){
        switch (i){
            case KEY_RETURN://退出
                emit send_data(0);
            break;
            case KEY_BACK:{ //删除
                key_press(pFocusWidget,Qt::Key_Backspace);
            }
            break;
            case KEY_ENTER:{//回车
                emit send_data(1);
//                key_press(pFocusWidget,Qt::Key_Enter);
            }
            break;
            default:{       // 数字 英文 KEY_SPACE KEY_BACK
                if(i<=KEY_BACK){
                    int keyValue = keyCode[i];
                    if((!bUpper)&&(i>=KEY_A)){
                        keyValue += 32;
                    }
                    QChar ch=keyValue;
                    key_press(pFocusWidget,ch.unicode(),ch);
                }
            }
            break;
        }
    }
}

void VirtualKeyboard::updateAlphaKeyText(bool bUpper)
{
    if (bUpper){
        for(int i=0;i<26;i++){
            ppbKey[KEY_A+i]->setText(QString('A'+i));
        }
        ppbKey[KEY_UPPER]->setText("大写");
    }
    else{
        for(int i=0;i<26;i++){
            ppbKey[KEY_A+i]->setText(QString('a'+i));
        }
        ppbKey[KEY_UPPER]->setText("小写");
    }
}

void VirtualKeyboard::updateKeyText()
{
    updateAlphaKeyText(false);
    for(int i=0;i<10;i++){
        ppbKey[KEY_0+i]->setText(QString('0'+i));
    }
    ppbKey[KEY_BACK]->setText("删除");
    ppbKey[KEY_ENTER]->setText("登录");
//    ppbKey[KEY_SPACE]->setText("空格");
    ppbKey[KEY_RETURN]->setText("退出");
}

