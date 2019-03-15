#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include "subwidget/virtualkeyboard.h"
#include <QStyleOption>
#include <QPainter>
#include <QWidget>
#include "subwidget/basesubwidget.h"
#include <QPushButton>

#include <QToolTip>
#include "subwidget/dialoginstance.h"

class LoginDialog : public BaseSubWidget
{
    Q_OBJECT
public:
    LoginDialog(int w, int h, QWidget *widget=0);
    void set_focus(){
        editUser->setFocus();
    }
private slots:
    void keyBoard_slot(int value){
        if(value==0){
            login_out();
        }
        else if(value==1){
           login();
        }
    }

    void checkBoxUser_slot(){
        if(checkBoxUser->isChecked()){
             editUser->setEchoMode(QLineEdit::Password);
        }else{
             editUser->setEchoMode(QLineEdit::Normal);
        }
    }

    void checkBoxPassWord_slot(){
        if(checkBoxPassWord->isChecked()){
             editPassWord->setEchoMode(QLineEdit::Password);
        }else{
             editPassWord->setEchoMode(QLineEdit::Normal);
        }
    }

    void btnLogin_slot(){
        login();
    }
    void btnCancel_slot(){
        login_out();
    }

signals:
    void sub_widget_data(int widget,int value);

private:
    QLabel *labelTitle;
    QLineEdit *editUser,*editPassWord;
    VirtualKeyboard *keyBoard;
    QVBoxLayout *vl;
    QHBoxLayout *hl1,*hl2;
    QCheckBox *checkBoxUser;
    QCheckBox *checkBoxPassWord;

    QPushButton *btnLogin,*btnCancel;

    void login_out(){
        editUser->clear();
        editPassWord->clear();
        emit sub_widget_data(5,0);
    }

    void login(){
        QString user=editUser->text();
        QString passwd=editPassWord->text();

        if(user.isEmpty()){
            DialogInstance::getInstance()->setText("用户名不能为空");
            editUser->clear();
            editPassWord->clear();
            return;
        }

        if(passwd.isEmpty()){
            DialogInstance::getInstance()->setText("密码不能为空");
            editUser->clear();
            editPassWord->clear();
            return;
        }

        QString str=read_File("/home/conf/passwd.txt");
            QStringList list=str.split(":");
            if(list.size()>=2){
                if((!user.compare(list.at(0)))&&(!passwd.compare(list.at(1)))){
                    editUser->clear();
                    editPassWord->clear();
                    emit sub_widget_data(5,1);
                    return;
                }
            }
            DialogInstance::getInstance()->setText("用户名或密码错误");
            editPassWord->clear();
    }
};

#endif // LOGINDIALOG_H

//        if(user.isEmpty()){
//            QToolTip::showText(editUser->mapToGlobal(QPoint(100, 0)), "用户名不能为空");
//        }

//        if(passwd.isEmpty()){
//            QToolTip::showText(editPassWord->mapToGlobal(QPoint(100, 0)), "密码不能为空");
//        }
