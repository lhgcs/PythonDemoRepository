#ifndef NUMKEYBOARD_H
#define NUMKEYBOARD_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "basesubwidget.h"
#include <QToolButton>

class NumKeyboard:public BaseSubWidget
{
    Q_OBJECT
public:
    NumKeyboard(QWidget *parent=0);
    ~NumKeyboard(){
         delete lineEdit;
         delete []button;
     }
signals:
     void num(int value);
private slots:
     void keyPress(int i);
private:
    QLineEdit *lineEdit;
    QToolButton *button;
};

#endif // NUMKEYBOARD_H
