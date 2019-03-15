#ifndef PARAMTERWIDGET_H
#define PARAMTERWIDGET_H

#include <QWidget>
#include "labelandedit.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include "subwidget/basesubwidget.h"
#include "numericalkeyboard.h"
#include "util/filehandler.h"
#include "subwidget/dialoginstance.h"
#include "serial/serialputout.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QStackedWidget>
#include "view/settingtimewidget.h"
#include "configure/toast.h"

#define Y_HEIGHT_FILE ("/home/conf/y_height.txt")
#define Y_SPEED_FILE  ("/home/conf/y_speed.txt")

class ParamterWidget : public BaseSubWidget
{
    Q_OBJECT
public:
    explicit ParamterWidget(int w, int h, QWidget *parent = 0);
    void get_data(){
        if(!is_file_exits(Y_HEIGHT_FILE)){
            Toast::getInstance()->set_text("请输入数值");
            //DialogInstance::getInstance()->setText("请设置参数");
        }else{
            QString str = read_File(Y_HEIGHT_FILE);
            if (str.isEmpty()){
                Toast::getInstance()->set_text("请输入数值");
                //DialogInstance::getInstance()->setText("请设置参数");
            }else{
                parse_json_height(str);
            }
        }
        if(!is_file_exits(Y_SPEED_FILE)){
            Toast::getInstance()->set_text("请输入数值");
            //DialogInstance::getInstance()->setText("请设置参数");
        }else{
            QString str = read_File(Y_SPEED_FILE);
            if (str.isEmpty()){
                Toast::getInstance()->set_text("请输入数值");
                //DialogInstance::getInstance()->setText("请设置参数");
            }else{
                parse_json_speed(str);
            }
        }
        show_data_height();
        show_data_speed();
    }

signals:
    void sub_widget_data(int widget,int value);
public slots:
    void btnTest_slot(){
        QString str=testInput->get_value();
        if(str.isEmpty()){
            Toast::getInstance()->set_text("请输入数值");
            //DialogInstance::getInstance()->setText("请输入数值");
        }else{
            SerialPutOut::getInstance()->moto_test(str.toLongLong());
        }
    }
    void btnReset_slot(){
        SerialPutOut::getInstance()->moto_reset();
    }
    void btnSaveY_slot(){
        for(int i=0;i<6;i++){
            if(yInput[i].get_value().isEmpty()){
                Toast::getInstance()->set_text("请输入数值");
                //DialogInstance::getInstance()->setText("请输入数值");
                return;
            }
        }

        int list[6]={0};
        for(int i=0;i<6;i++){
            list[i]=yInput[i].get_value().toInt();
        }

        SerialPutOut::getInstance()->set_height(list);
        save_height();
    }
    void btnSaveSpeed_slot(){
        for(int i=0;i<4;i++){
            if(speedInput[i].get_value().isEmpty()){
                Toast::getInstance()->set_text("请输入数值");
                //DialogInstance::getInstance()->setText("请输入数值");
                return;
            }
        }

        int list[4]={0};
        for(int i=0;i<4;i++){
            list[i]=speedInput[i].get_value().toInt();
        }

        if(list[0] < 2000 || list[0] > 32000 || list[1] < 2000 || list[1] > 32000){
            DialogInstance::getInstance()->setText("最高速度/复位速度范围:\n2000~32000");
            return;
        }
        if(list[2] < 1 || list[2] > 255 || list[3] < 1 || list[3] > 255){
            DialogInstance::getInstance()->setText("加速度/减速度范围:\n1~255");
            return;
        }
        SerialPutOut::getInstance()->set_speed(list);
        save_speed();
    }
    void btnBack_slot(){
        if(stackedWidget->currentIndex() == 1){
            if(settingTimeWidget->get_record_num() < App::goodsTotal){
                QString str="货道数量不能少于"+App::goodsTotal;
                //DialogInstance::getInstance()->setText(str);
            }
            btnTime->setVisible(true);
            stackedWidget->setCurrentIndex(0);
        }else{
            emit sub_widget_data(4,0);
        }
    }
    void btnTime_slot(){
        if(stackedWidget->currentIndex() != 1){
            stackedWidget->setCurrentIndex(1);
            btnTime->setVisible(false);
            settingTimeWidget->referce();
        }
    }

private:
    void save_height(){
        QString str=get_json_height();
        write_File(Y_HEIGHT_FILE,str);
    }
    void save_speed(){
        QString str=get_json_speed();
        write_File(Y_SPEED_FILE,str);
    }

    QString get_json_height(){
        QJsonObject json;
        for(int i=0;i<6;i++){
            json.insert(QString::number(i),yInput[i].get_value());
        }
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        return byte_array;
    }
    QString get_json_speed(){
        QJsonObject json;
        for(int i=0;i<4;i++){
            json.insert(QString::number(i),speedInput[i].get_value());
        }
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        return byte_array;
    }

    void parse_json_height(QString jsonStr){
        QJsonParseError error;                                                         // 1. 创建 QJsonParseError 对象，用来获取解析结果
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonStr.toUtf8(), &error);// 2. 使用静态函数获取 QJsonDocument 对象
        if (error.error == QJsonParseError::NoError){                                  //检查json是否有错误
            if (!(jsonDocument.isNull() || jsonDocument.isEmpty())) {                  //以 QVariant 的格式进行解析
                if (jsonDocument.isObject()){
                    QJsonObject obj = jsonDocument.object();                           //开始解析json对象

                    for(int i=0;i<6;i++){
                        QString id=QString::number(i);
                        if(obj.contains(id)){
                            QJsonValue temp = obj.take(id);
                            if (temp.isString()){
                                mapHeight.insert(i,temp.toVariant().toString());
                            }
                        }
                    }

                }
            }
        }
    }

    void parse_json_speed(QString jsonStr){
        QJsonParseError error;                                                         // 1. 创建 QJsonParseError 对象，用来获取解析结果
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonStr.toUtf8(), &error);// 2. 使用静态函数获取 QJsonDocument 对象
        if (error.error == QJsonParseError::NoError){                                  //检查json是否有错误
            if (!(jsonDocument.isNull() || jsonDocument.isEmpty())) {                  //以 QVariant 的格式进行解析
                if (jsonDocument.isObject()){
                    QJsonObject obj = jsonDocument.object();                           //开始解析json对象

                    for(int i=0;i<4;i++){
                        QString id=QString::number(i);
                        if(obj.contains(id)){
                            QJsonValue temp = obj.take(id);
                            if (temp.isString()){
                                mapSpeed[i]=temp.toVariant().toString();
                            }
                        }
                    }

                }
            }
        }
    }

    void show_data_height(){
        QMap<int,QString>::Iterator it;
        for(int i=0;i<6;i++){
            it = mapHeight.find(i);
            if(it != mapHeight.end()){
                yInput[i].set_text(mapHeight[i]);
            }else{
                yInput[i].set_text("0");
            }
        }
    }

    void show_data_speed(){
        QMap<int,QString>::Iterator it;
        for(int i=0;i<4;i++){
            it = mapSpeed.find(i);
            if(it != mapSpeed.end()){
                speedInput[i].set_text(mapSpeed[i]);
            }else{
                speedInput[i].set_text("0");
            }
        }
    }

private:
    QLabel *labelTitle;

    LabelAndEdit *testInput;
    LabelAndEdit *yInput;
    LabelAndEdit *speedInput;
    QVBoxLayout *vl;
    QGridLayout *testgl;
    QGridLayout *ygl;
    QGridLayout *speed_gl;

    QMap<int,QString> mapHeight;
    QMap<int,QString> mapSpeed;

    QPushButton *btnTest,*btnReset,*btnSaveY,*btnSaveSpeed;
    QPushButton *btnBack,*btnTime;

    NumericalKeyboard *key;
    SettingTimeWidget *settingTimeWidget;
    QStackedWidget *stackedWidget;
    QVBoxLayout *v;
    QHBoxLayout *hl;
};

#endif // PARAMTERWIDGET_H



//        if(list[0] < 2000 || list[0] > 32000){
//            DialogInstance::getInstance()->setText("最高速度/复位速度范围:2000~32000");
//            QToolTip::showText(speedInput[0].mapToGlobal(QPoint(100, 0)), "范围应该是2000~32000");
//            return;
//        }
//        if(list[1] < 2000 || list[1] > 32000){
//            DialogInstance::getInstance()->setText("最高速度/复位速度范围:2000~32000");
//            QToolTip::showText(speedInput[1].mapToGlobal(QPoint(100, 0)), "范围应该是2000~32000");
//            return;
//        }
//        if(list[2] < 1 || list[2] > 255){
//            DialogInstance::getInstance()->setText("加速度/减速度范围:1~255");
//            QToolTip::showText(speedInput[2].mapToGlobal(QPoint(100, 0)), "范围应该是1~255");
//            return;
//        }
//        if(list[3] < 1 || list[3] > 255){
//            DialogInstance::getInstance()->setText("加速度/减速度范围:1~255");
//            QToolTip::showText(speedInput[3].mapToGlobal(QPoint(100, 0)), "范围应该是1~255");
//            return;
//        }



