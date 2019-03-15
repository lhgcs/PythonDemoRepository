#ifndef REPLENISHMENT_H
#define REPLENISHMENT_H

#include <QWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>
#include "app.h"
#include <QGridLayout>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <Sqlite/sqlitegoods.h>
#include "switchbutton.h"
#include <QMap>
#include "subwidget/dialoginstance.h"
#include "filehandler.h"
#include "toast.h"
#include "filehandler.h"
#include "unistd.h"
#include "Upgrade/firmwaredownload.h"

class SettingVasual : public QWidget
{
    Q_OBJECT
public:
    explicit SettingVasual(QWidget *parent = 0){
        this->setFixedSize(App::DeskWidth,App::DeskHeight);
        this->setAttribute(Qt::WA_DeleteOnClose,true);
        this->move(0,0);
        btnSave=new QPushButton("保存",this);
        btnCancel=new QPushButton("取消",this);
        btnDelete=new QPushButton("清空数据",this);

        btnSave->setFixedSize(App::DeskWidth/6,App::DeskHeight/10);
        btnCancel->setFixedSize(App::DeskWidth/6,App::DeskHeight/10);
        QObject::connect(btnSave,SIGNAL(clicked(bool)),this,SLOT(btnSave_slot()));
        QObject::connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
        btnSave->setObjectName("btnNormal");
        btnCancel->setObjectName("btnNormal");
        btnDelete->setObjectName("btnNormal");

        btnDelete->setFixedSize(App::DeskWidth/6,App::DeskHeight/10);
        QObject::connect(btnDelete,SIGNAL(clicked(bool)),this,SLOT(btnDelete_slot()));

        button=new QCheckBox[App::deviceNum];
        for(int i=0;i<App::deviceNum;i++){
            button[i].setFocusPolicy(Qt::NoFocus);
            button[i].setText(QString(65+i));
        }
        label = new QLabel(this);
        label->setText(FirmWareDownload::mCurentVersion);

        hl=new QHBoxLayout();
        hl->setMargin(0);
        hl->addStretch(1);
        for(int i=0;i<App::deviceNum;i++){
            hl->addWidget(&button[i],0,Qt::AlignVCenter);
            hl->addStretch(1);
        }
        hl2=new QHBoxLayout();
        hl2->setMargin(0);
        hl2->addWidget(btnDelete);
        hl2->addWidget(btnSave);
        hl2->addWidget(btnCancel);
        vl=new QVBoxLayout(this);
        vl->addStretch(2);
        vl->addLayout(hl);
        vl->addStretch(1);
        vl->addLayout(hl2);
        vl->addStretch(2);

        for(int i=0;i<App::deviceNum;i++){
            if(App::deviceVisual[i])
                button[i].setChecked(true);
            else
                button[i].setChecked(false);
        }
        this->setLayout(vl);
        this->show();
    }
    ~SettingVasual(){
        delete []button;
        delete btnSave;
        delete btnCancel;
        delete btnDelete;
        delete label;
        delete hl;
        delete hl2;
        delete vl;
    }
public:
    QCheckBox *button;
    QPushButton *btnSave;
    QPushButton *btnCancel;
    QPushButton *btnDelete;
    QLabel *label;
private:
    QHBoxLayout *hl;
    QHBoxLayout *hl2;
    QVBoxLayout *vl;
private slots:
    void btnSave_slot(){
        if(button[0].isChecked() == false){
            DialogInstance::getInstance()->setText("必须选中A");
            return;
        }

        QJsonObject jsonObject;

        for(int i=0;i<App::deviceNum;i++){
            if(button[i].isChecked()){
                App::deviceVisual[i]=true;
            }else{
                App::deviceVisual[i]=false;
            }
            jsonObject.insert(QString::number(i+1),App::deviceVisual[i]);
        }

        QString str = JsonHandler::get_json_format_string(jsonObject);
        write_File("/home/conf/device.txt",str);

        Toast::getInstance()->set_text("系统将重启");
        system("/bin/sync");
        sleep(3);
        system("/sbin/reboot");

//        XmlDom xml;
//        xml.writeXML("/home/conf/conf.xml");

        this->close();
    }

    void btnDelete_slot(){
        Toast::getInstance()->set_text("系统将重启");
        clear_dir_files("/home/sql");
        clear_dir_files("/home/image");
        system("/bin/sync");
        sleep(3);
        system("/sbin/reboot");
    }
};

class RadioGroup : public QWidget
{
    Q_OBJECT
public:
    explicit RadioGroup(QWidget *parent = 0){
        this->setFixedSize(App::DeskWidth/6-20,App::DeskHeight-100);
        buttonGroup = new QButtonGroup(this);
        button=new QRadioButton[App::deviceNum];
        for(int i=0;i<App::deviceNum;i++){
            button[i].setFocusPolicy(Qt::NoFocus);
            buttonGroup->addButton(&button[i],i);
        }
        QObject::connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));

        button[0].setChecked(true);
        button[0].setText("A柜");
        button[1].setText("B柜");
        button[2].setText("C柜");
        button[3].setText("D柜");
        button[4].setText("E柜");

        vl=new QVBoxLayout(this);
        vl->setMargin(0);
        for(int i=0;i<App::deviceNum;i++){
           vl->addStretch();
           vl->addWidget(&button[i]);
        }
        vl->addStretch();
        this->setLayout(vl);
        this->show();

        timer=new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(300);
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));
    }
    ~RadioGroup(){
        delete []button;
        delete buttonGroup;
        delete vl;
    }
signals:
    void num(int layout,int value);
private:
    QRadioButton *button;
    QButtonGroup *buttonGroup;
    QVBoxLayout *vl;
    QTimer *timer;
    int selectButton;
private slots:
    void buttonJudge(int buttonId){
        timer->start();
        selectButton=buttonId;
    }
    void timer_slot(){
        if((selectButton>=0)&&(selectButton<App::deviceNum)){
            emit num(App::RadioGroupWidget,selectButton);
        }
    }
};


class PushButtonGroup : public QWidget
{
    Q_OBJECT
public:
    explicit PushButtonGroup(QWidget *parent = 0){
        this->setFixedSize(App::DeskWidth/6-20,App::DeskHeight-100);
        btnNum=7;
        buttonGroup = new QButtonGroup(this);
        button=new QPushButton[btnNum];
        for(int i=0;i<btnNum;i++){
            button[i].setFixedSize(this->width(),(this->height()-100)/10);
            button[i].setFocusPolicy(Qt::NoFocus);
            button[i].setObjectName("btnNormal");
            buttonGroup->addButton(&button[i],i);
        }
        QObject::connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));

        button[0].setText("全选");
        button[1].setText("反选");
        button[2].setText("打开");
        button[3].setText("设置");
        button[4].setText("上传");
        button[5].setText("下载");
        button[6].setText("退出");

        vl=new QVBoxLayout(this);
        vl->setMargin(0);
        for(int i=0;i<btnNum;i++){
           vl->addWidget(&button[i],0,Qt::AlignLeft);
        }
        this->setLayout(vl);
        this->show();

        timer=new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(300);
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));
    }
    ~PushButtonGroup(){
        delete []button;
        delete buttonGroup;
        delete vl;
    }
signals:
    void num(int layout,int value);
private:
    QPushButton *button;
    QButtonGroup *buttonGroup;
    QVBoxLayout *vl;
    QTimer *timer;
    int selectButton;
    int btnNum;
private slots:
    void buttonJudge(int buttonId){
        timer->start();
        selectButton=buttonId;
    }
    void timer_slot(){
        if((selectButton>=0)&&(selectButton<btnNum)){
            emit num(App::PushButtonGroupWidget,selectButton);
        }
    }
};


class ReplenishmentItem : public QWidget{
public:
    explicit ReplenishmentItem(QWidget *parent = 0):QWidget(parent){
        this->setFixedSize(App::DeskWidth*0.21,(App::DeskHeight-100)/10);
        checkBox=new QCheckBox(this);
        switchButton=new SwitchButton(this);
        checkBox->setFixedSize(width()*0.4,height());
        switchButton->setFixedSize(width()*0.6,height());
        hl=new QHBoxLayout(this);
        hl->setMargin(0);
        hl->addWidget(checkBox);
        hl->addWidget(switchButton);
        this->setLayout(hl);
    }
    ~ReplenishmentItem(){
        delete checkBox;
        delete switchButton;
        delete hl;
    }

    bool get_check(){
        if(checkBox->checkState() == Qt::Checked){
            return true;
        }
        return false;
    }
    void set_check(int status){
        if(status == true){
            checkBox->setCheckState(Qt::Checked);
        }else{
            checkBox->setCheckState(Qt::Unchecked);
        }
    }
    int get_num(){
        return switchButton->getChecked_num();
    }
    void set_num(int num){
        if(num > 0){
            switchButton->setChecked(true);
        }else{
            switchButton->setChecked(false);
        }
    }
    void set_text(const QString &str){
        checkBox->setText(str);
    }

private:
    QCheckBox *checkBox;
    SwitchButton *switchButton;
    QHBoxLayout *hl;
};


class ReplenishmentItemGroup : public QWidget{
public:
    explicit ReplenishmentItemGroup(QWidget *parent = 0):QWidget(parent){
        this->setFixedSize(App::DeskWidth*2/3,App::DeskHeight-100);
        item=new ReplenishmentItem[18];
        gl=new QGridLayout(this);
        gl->setMargin(0);
        gl->setHorizontalSpacing(0);
        gl->setVerticalSpacing(0);
        for(int i=0;i<6;i++){
            for(int j=0;j<3;j++){
                int num=i*3+j;
                item[num].set_text(QString::number(num+1));
                gl->addWidget(&item[num],i,j,1,1);
            }
        }
        this->setLayout(gl);
    }
    ~ReplenishmentItemGroup(){
        delete []item;
        delete gl;
    }

    bool get_check(int index){
        return item[index].get_check();
    }
    void set_check(int index,int status){
        item[index].set_check(status);
    }
    int get_num(int index){
        return item[index].get_num();
    }
    void set_num(int index,int num){
        item[index].set_num(num);
    }

private:
    ReplenishmentItem *item;
    QGridLayout *gl;
};

#include <QStackedWidget>

class Replenishment : public QWidget
{
    Q_OBJECT
public:
    explicit Replenishment();
    explicit Replenishment(QWidget *parent = 0):QWidget(parent){
        this->setObjectName("Replenishment");
        this->setFixedSize(App::DeskWidth,App::DeskHeight);
        this->move(0,0);
        this->setFocusPolicy(Qt::NoFocus);

        stackWidget=new QStackedWidget;
        stackWidget->setContentsMargins(0,0,0,0);
        stackWidget->setFixedSize(App::DeskWidth*2/3,App::DeskHeight-100);

        group=new ReplenishmentItemGroup[5];
        for(int i=0;i<5;i++){
            stackWidget->addWidget(&group[i]);
        }

        radioGroup=new RadioGroup(this);
        pushButtonGroup=new PushButtonGroup(this);

        QObject::connect(radioGroup,SIGNAL(num(int,int)),this,SLOT(widget_slot(int,int)));
        QObject::connect(pushButtonGroup,SIGNAL(num(int,int)),this,SLOT(widget_slot(int,int)));

        hl=new QHBoxLayout(this);
        hl->setContentsMargins(0,0,0,0);
        hl->addStretch();
        hl->addWidget(stackWidget);
        hl->addStretch();
        hl->addWidget(radioGroup);
        hl->addStretch();
        hl->addWidget(pushButtonGroup);
        hl->addStretch();
        this->setLayout(hl);
    }
    ~Replenishment(){
        delete []group;
        delete radioGroup;
        delete pushButtonGroup;
        delete stackWidget;
        delete hl;
    }

    QString replenishment_str;
    QVector<int> open_vector;
    QMap<int,int> map;

    int sark_index;
    void init_data(){
        replenishment_str.clear();
        open_vector.clear();

        sark_index=0;
        set_check_status();

        App::replenishmentQrcode.clear();
        App::replenishmentTotalNum=-1;
    }

    bool save_to_sql(){
        bool res=true;
        QMap<int, int>::iterator it;
        for(it=map.begin();it!=map.end();it++)  {
            res &=SqliteGoods::getInstance()->table_update(QString::number(it.key()+1),QString::number(it.value()));
        }
        return res;
    }

private:
    ReplenishmentItemGroup *group;
    RadioGroup *radioGroup;
    PushButtonGroup *pushButtonGroup;
    QHBoxLayout *hl;
    QStackedWidget *stackWidget;

    /**
      从数据库获取数据
     * @brief get_table_record
     */
    void set_check_status(){
        QVector<int> vector;
        vector.clear();
        vector.resize(18*5);
        vector.fill(0);
        SqliteGoods::getInstance()->get_table_remain_data(vector,18*5);

        for(int j=0;j<5;j++){
            int base=j*18;
            for(int i=0;i<18;i++){
                group[j].set_check(i,false);
                if(vector.at(base+i)>0){
                    group[j].set_num(i,1);
                }else{
                    group[j].set_num(i,0);
                }
            }
        }
    }

//    void map_add(int k,int v){
//        map.insert(k,v);//若QMap中已经存在key，会插入替换原来的value
//    }
//    void map_remove(int k){
//        if(!map.isEmpty()){
//            QMap<int, int>::iterator it;
//            for(it=map.begin();it!=map.end();it++)  {
//                if(it.key()==k) {
//                    map.erase(it);//it 删除
//                    return;
//                }
//            }
//        }
//    }
//    int map_query(int k){
//        QMap<int, int>::const_iterator it = map.find(k);//查询
//        if( it != map.end()){
//            return it.value();
//        }
//        return -1;
//    }

    QString package_json(QMap<int,int> map){
        QJsonObject json;
        QMap<int, int>::const_iterator it;                  //遍历：
        for( it=map.constBegin(); it!=map.constEnd(); ++it){//QMap的Key会自动按升序排列
            if(it.value()>=0){
                json.insert("box"+QString::number(it.key()+1),QString::number(it.value()));
            }
        }
        if(json.size()<=0){
            DialogInstance::getInstance()->setText("你未更改任何状态");
        }
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        QString str=byte_array;
        return str;
    }
signals:
    void num(int layout,int value);
private slots:
    void widget_slot(int layer,int value){
        switch(layer) {
            case App::RadioGroupWidget:{
                if((value>=0)&&(value<App::deviceNum)){
                    sark_index=value;
                    stackWidget->setCurrentIndex(value);
                }
                break;
            }
            case App::PushButtonGroupWidget:{
                switch (value) {
                    case 0:{//全选
                        int num=0;
                        for(int i=0;i<18;i++){
                            if(group[sark_index].get_check(i) == true){
                                num++;
                            }
                        }
                        if(num == 18){
                            for(int i=0;i<18;i++){
                                group[sark_index].set_check(i,false);
                            }
                        }else{
                            for(int i=0;i<18;i++){
                                group[sark_index].set_check(i,true);
                            }
                        }
                    }
                    break;
                    case 1:{//反选
                        for(int i=0;i<18;i++){
                            if(group[sark_index].get_check(i) == true){
                                group[sark_index].set_check(i,false);
                            }else{
                                group[sark_index].set_check(i,true);
                            }
                        }
                    }
                    break;
                    case 2:{//打开
                        open_vector.clear();
                        open_vector.resize(0);
                        for(int i=0;i<18;i++){
                            if(group[sark_index].get_check(i) == true){
                                open_vector.append(sark_index*18+i+1);
                            }
                        }
                        if(open_vector.isEmpty()){
                            DialogInstance::getInstance()->setText("请先选择格子");
                        }else{
                            emit num(App::replenishmentLayout,2);
                        }
                    }
                    break;
                    case 3:{//保存
                        new SettingVasual(this);
                    }
                    break;
                    case 4:{//上传
                        if(App::replenishmentQrcode.isEmpty()||App::replenishmentTotalNum<0){
                            DialogInstance::getInstance()->setText("请扫描补货二维码");
                            return;
                        }
                        if(App::replenishmentTotalNum==0){
                            DialogInstance::getInstance()->setText("你可补货的数量为0");
                            return;
                        }

                        QMap<int,int> changeStatus;
                        QVector<int> vector;
                        vector.resize(18*5);
                        vector.fill(0);
                        SqliteGoods::getInstance()->get_table_remain_data(vector,18*5);

                        for(int j=0;j<5;j++){
                            int base=j*18;
                            for(int i=0;i<18;i++){
                                int num=group[j].get_num(i);
                                if(vector[base+i] != num){
                                    changeStatus.insert(base+i,num);
                                }
                            }
                        }

                        if(changeStatus.isEmpty()){
                            DialogInstance::getInstance()->setText("你未更改任何状态");
                            return;
                        }

                        int totalNum=0;
                        QMap<int, int>::const_iterator it;                  //遍历：
                        for( it=changeStatus.constBegin(); it!=changeStatus.constEnd(); ++it){//QMap的Key会自动按升序排列
                            if(it.value()>=0){
                                totalNum+=it.value();
                            }
                        }

                        if(App::replenishmentTotalNum < totalNum){
                            DialogInstance::getInstance()->setText("你补货的数量超过的最大允许数量");
                            return;
                        }
                        replenishment_str.clear();
                        replenishment_str=package_json(changeStatus);
                        replenishment_str.remove("\\");
                        map=changeStatus;
                        emit num(App::replenishmentLayout,4);
                    }
                    break;
                    case 5:{//下载
                        emit num(App::replenishmentLayout,5);
                    }
                    break;
                    case 6:{//退出
                        emit num(App::replenishmentLayout,6);
                    }
                    break;
                    default:break;
                }
                break;
            }
            default:break;
        }
    }
protected:
    void paintEvent(QPaintEvent *){
        QStyleOption  opt;
        opt.init(this);
        QPainter  p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};


#endif // REPLENISHMENT_H
