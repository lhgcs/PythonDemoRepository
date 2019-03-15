#ifndef ADDWARE_H
#define ADDWARE_H

#include <QPushButton>
#include <QGridLayout>
#include <QSignalMapper>
#include <QHBoxLayout>
#include <QVector>
#include <QStackedWidget>

#include "serial/serialputout.h"
#include "util/filehandler.h"
#include "view/basewidget.h"
#include "configure/app.h"

#include "subwidget/dialoginstance.h"
#include "subwidget/basesubwidget.h"
#include "subwidget/addorreduce.h"

#include "model/load.h"
#include "model/httprequest.h"
#include "model/tablegoodinfo.h"
#include "configure/toast.h"

class AddWare : public BaseSubWidget
{
    Q_OBJECT
public:
    explicit AddWare(int _w,int _h,QWidget *parent = 0);
    ~AddWare(){
        delete[] btn;
        delete[] addOrReduce;
        delete signalMapper;
        delete hl;
    }
    void sync_database(){
        for(int i=0;i<App::goodsTotal;i++){
            QString str=addOrReduce[i].get_num_str();
            TableGoodInfo::getInstence()->start_transaction();
            TableGoodInfo::getInstence()->table_update_stock(App::goodsTableName,i+1,str);
            TableGoodInfo::getInstence()->start_commit();
        }
    }
    void get_database_data(){
        for(int i=0;i<App::goodsTotal;i++){
            int num=TableGoodInfo::getInstence()->table_select_stock(App::goodsTableName,i+1);
            addOrReduce[i].set_num(num);
            if(num >0){
                addOrReduce[i].setCheckBox_status(true);
            }else{
                addOrReduce[i].setCheckBox_status(false);
            }
        }
    }

    /**
      获取校正位置
     * @brief get_reset_box
     * @return
     */
    int get_reset_box(){
        int box=0;
        int start_index=index*20;
        for(int i=0;i<20;i++){
           if(start_index+i >= App::goodsTotal){
               break;
           }
           if(addOrReduce[start_index+i].getCheckBox_status()){
               box=start_index+i+1;
               break;
           }
        }
        return box;
    }

    void reset_ok(int id){
        id--;
        qDebug()<<"id:"<<id;
        if(id>=0 && id<App::goodsTotal){
            addOrReduce[id].setCheckBox_status(false);
        }
    }

private slots:
    void btn_press(int value){
        int start_index=index*20;
        int remain_index=App::goodsTotal-start_index>20?20:App::goodsTotal-start_index;
        switch (value) {
            case 0: {//全选
                int num=0;
                for(int i=0;i<20;i++){
                   if(start_index+i >= App::goodsTotal){
                       break;
                   }
                   if(addOrReduce[start_index+i].getCheckBox_status()){
                       num+=1;
                   }
                }

                for(int i=0;i<20;i++){
                    if(start_index+i >= App::goodsTotal){
                        break;
                    }
                    if(num == remain_index){//已经是全选
                        addOrReduce[start_index+i].setCheckBox_status(false);
                    }else{
                        addOrReduce[start_index+i].setCheckBox_status(true);
                    }
                }
            }
            break;
            case 1: {//补货
                int num=0;
                for(int i=0;i<20;i++){
                   if(start_index+i >= App::goodsTotal){
                       break;
                   }
                   if(addOrReduce[start_index+i].getCheckBox_status()){
                       num+=1;
                   }
                }
                if(num == 0){
                    Toast::getInstance()->set_text("请选择需要补货的货道");
                    //DialogInstance::getInstance()->setText("请选择需要补货的货道");
                }else{
                    for(int i=0;i<20;i++){
                        if(start_index+i >= App::goodsTotal){
                            break;
                        }
                        if(addOrReduce[start_index+i].getCheckBox_status()){
                            if(top_up_flag){
                                addOrReduce[start_index+i].set_num_max();
                            }else{
                                addOrReduce[start_index+i].set_num_min();
                            }
                        }
                    }
                    if(top_up_flag){
                        top_up_flag=false;
                    }else{
                        top_up_flag=true;
                    }
                }
            break;
            }
            case 2: {//上一页
                if(--index<0){
                    index=stackedLayout->count()-1;
                }
                stackedLayout->setCurrentIndex(index);
            }
            break;
            case 3: {//下一页
                if(++index>stackedLayout->count()-1){
                    index=0;
                }
                stackedLayout->setCurrentIndex(index);
            }
            break;
            case 4: {//上传
                HttpRequest::getInstance()->send_status(package_json());
                Toast::getInstance()->set_text("正在上传数据");
                //DialogInstance::getInstance()->setText("正在上传数据");
            }
            break;
            case 5: {//下载
                emit sub_widget_data(2,2);
            }
            break;
            case 6: {//校正
                int num=0;
                for(int i=0;i<20;i++){
                   if(start_index+i >= App::goodsTotal){
                       break;
                   }
                   if(addOrReduce[start_index+i].getCheckBox_status()){
                       num+=1;
                   }
                }
                if(num ==0){
                    Toast::getInstance()->set_text("请选择需要校正的货道");
                    //DialogInstance::getInstance()->setText("请选择需要校正的货道");
                }else{
                    emit sub_widget_data(2,1);
                }
            }
            break;
            case 7: {//设置
                sub_widget_data(2,3);
            }
            break;
            case 8: {//参数
                sub_widget_data(2,4);
            }
            break;
            case 9: {//退出
                sub_widget_data(2,0);
            }
            break;
            default:break;
        }
    }
signals:
    void sub_widget_data(int widget,int value);
private:
    int index;
    bool top_up_flag;
    QPushButton *btn;
    QSignalMapper *signalMapper;
    AddOrReduce *addOrReduce;
    QHBoxLayout *hl;

    QStackedWidget *stackedLayout;

//    QString package_json(){
//        QJsonObject json;
//        for(int i=0;i<App::goodsTotal;i++){
//            json.insert("box"+QString::number(i+1),addOrReduce[i].get_num_str());
//        }
//        QJsonDocument document;
//        document.setObject(json);
//        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
//        QString str=byte_array;
//        return str;
//    }


    QMap<int,int> package_json(){
        QMap<int,int> map;
        for(int i=0;i<App::goodsTotal;i++){
            map.insert(i+1,addOrReduce[i].get_num());
        }
        return map;
    }

};

#endif // ADDWARE_H
