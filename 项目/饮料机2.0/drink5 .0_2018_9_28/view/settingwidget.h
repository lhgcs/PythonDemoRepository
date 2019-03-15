#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "subwidget/basesubwidget.h"
#include "subwidget/virtualkeyboard.h"
#include "subwidget/dialoginstance.h"
#include "util/filehandler.h"

#include "model/httprequest.h"
#include "configure/configure.h"
#include "model/tablegoodinfo.h"

class SettingWidget : public BaseSubWidget
{
    Q_OBJECT
public:
    explicit SettingWidget(QWidget *parent = 0);
    void show_data(){
        QString str=read_File("/home/conf/configure.txt");
        if(str != NULL){
            Configure con;
            con.parse_json(str);
            if(con.macIdType == 0){
                groupMacId->button(0)->setChecked(true);
            }else{
                groupMacId->button(1)->setChecked(true);
            }
            switch (con.payType) {
                case HttpRequest::ALIPAY:
                    checkAlipay->setChecked(true);
                    checkWeixin->setChecked(false);
                break;
                case HttpRequest::WEIXIN:
                    checkAlipay->setChecked(false);
                    checkWeixin->setChecked(true);
                break;
                case HttpRequest::WEIXIN_AND_ALIPAY:
                    checkAlipay->setChecked(true);
                    checkWeixin->setChecked(true);
                break;
                default:
                    checkAlipay->setChecked(false);
                    checkWeixin->setChecked(false);
                break;
            }
            lineEditGoodsNum->setText(QString::number(con.goodsNum));
        }else{
            groupMacId->button(0)->setChecked(true);
            checkAlipay->setChecked(false);
            checkWeixin->setChecked(false);
            lineEditGoodsNum->setText("60");
        }
        str=read_File("/home/conf/mac_id.txt");
        lineEditCustomMacId->setText(str);
    }

signals:
    void sub_widget_data(int widget,int value);
private:
    QRadioButton* radioMacId[2];
    QButtonGroup* groupMacId;
    VirtualKeyboard *keyBoard;
    QLabel *label,*labelTitle;
    QLineEdit *lineEditCustomMacId;
    QLabel *labelGoodsNum;
    QLineEdit *lineEditGoodsNum;
    QLabel *labelPay;
    QCheckBox *checkAlipay;
    QCheckBox *checkWeixin;
    QPushButton *btnClose,*btnSave,*btnFactory;
private slots:
    void radioMacId_slot(int value){
        if(value == 1){
            lineEditCustomMacId->setFocus();
        }
    }

    void btnClose_slot(){
        emit sub_widget_data(3,0);
    }

    void btnSave_slot(){
        QString str=lineEditGoodsNum->text();
        if(str.isEmpty() || str.startsWith("0")){
            DialogInstance::getInstance()->setText("请输入商品数量");
            return;
        }

        Configure con;
        QString temp=read_File("/home/conf/configure.txt");
        if(temp != NULL){
            con.parse_json(temp);
        }
        con.goodsNum=str.toInt();

        int type=0;
        if(checkAlipay->isChecked()){
            type+=HttpRequest::ALIPAY;
        }
        if(checkWeixin->isChecked()){
            type+=HttpRequest::WEIXIN;
        }
        con.payType=type;
        App::payType=type;

        if(groupMacId->checkedId() == 1){
            con.macIdType=1;
            QString str=lineEditCustomMacId->text();
            if(str.isEmpty()){
                DialogInstance::getInstance()->setText("请输入机台号\n或者选择默认机台号");
                return;
            }else{
                write_File("/home/conf/mac_id.txt",str);
            }
        }else{
            con.macIdType=0;
        }

        str=con.get_json();
        int res=write_File("/home/conf/configure.txt",str);
        system("sync");
        if(res > 0){
            system("/bin/sync");
            DialogInstance::getInstance()->setText("保存成功,系统将重启",3000);
            system("/sbin/reboot");
        }else{
            DialogInstance::getInstance()->setText("保存失败");
        }
    }

    void btnFactory_slot(){
        DataBaseHelper::getInstence()->database_close();
        clear_dir_files("/home/image");
        clear_dir_files("/home/video");
        clear_dir_files("/home/sql");
        system("/bin/sync");
        DialogInstance::getInstance()->setText("系统将重启",3000);
        system("/sbin/reboot");
    }

    void keyBoard_slot(int value){
        if(value==0){
        }
        else if(value==1){
        }
    }
};

#endif // SETTINGWIDGET_H
