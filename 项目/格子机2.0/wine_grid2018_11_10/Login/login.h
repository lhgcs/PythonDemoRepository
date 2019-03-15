#ifndef LOGIN_H
#define LOGIN_H

#include <QLineEdit>
#include <QFormLayout>
#include "virtualkeyboard.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include "app.h"

#include "Setting/settingwidget.h"
#include "Sqlite/sqlitegoods.h"
#include "toast.h"

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);
    ~Login(){
        delete label;
        delete editUser;
        delete editPassWord;
        delete keyBoard;
        delete pLayout;
        delete vLayout;
    }
    QLineEdit *editUser;
signals:
    void num(int layout,int value);
private slots:
    void keyBoard_slot(int value){
        if(value==0){
            editUser->clear();
            editPassWord->clear();
            emit num(App::loginLayout,1);
            this->close();
        }
        else if(value==1){
            QString user=editUser->text();
            QString passwd=editPassWord->text();
            if(user.isEmpty()){
                Toast::getInstance()->set_text("用户名不能为空");
                return;
            }
            if(passwd.isEmpty()){
                Toast::getInstance()->set_text("密码不能为空");
                return;
            }
            if((!user.compare(App::user))&&(!passwd.compare(App::password))){
                emit num(App::loginLayout,0);
            }
            else{
                Toast::getInstance()->set_text("用户名或密码错误");
            }
            editUser->clear();
            editPassWord->clear();
        }
    }
private:
    QLabel *label;
    QLineEdit *editPassWord;
    VirtualKeyboard *keyBoard;
    QFormLayout *pLayout;
    QVBoxLayout *vLayout;
protected:
    void paintEvent(QPaintEvent *){
        QStyleOption  opt;
        opt.init(this);
        QPainter  p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // LOGIN_H
