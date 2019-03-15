#include "settingwidget.h"
#include "app.h"

#include "systeminfo.h"

SettingWidget::SettingWidget(SqliteGoods *_sqliteGoods,HttpRequest *_httpRequest, QWidget *parent) : QWidget(parent)
{
    this->setObjectName("SettingWidget");
    this->setFixedSize(App::DeskWidth,App::DeskHeight);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->move(0,0);
    QStringList str;
    str<<"补货"<<"系统信息"<<"重启"<<"退出";
    listWidget = new QListWidget(this);
    listWidget->setTextElideMode(Qt::ElideMiddle);
    listWidget->setFixedSize(200,App::DeskHeight-40);
    listWidget->setFrameShape(QListWidget::NoFrame);//无边款效果
    listWidget->setFocusPolicy(Qt::NoFocus);
    for(int i=0;i<4;i++){
        QListWidgetItem * pItem = new QListWidgetItem(str.at(i),listWidget);
        pItem->setSizeHint(QSize(200, 100));                //每次改变Item的高度   //QIcon("images/data.png")
        pItem->setTextAlignment(Qt::AlignCenter);
        listWidget->insertItem(i,pItem);
    }
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setFixedSize(App::DeskWidth-250-40,App::DeskHeight-40-100);

//    TableView *tableView=new TableView(stackedWidget);
//    stackedWidget->addWidget(tableView);

    addGoods=new AddGoods(stackedWidget);
    stackedWidget->addWidget(addGoods);

    addGoods->sqliteGoods=_sqliteGoods;
    addGoods->httpRequest=_httpRequest;


    SystemInfo *sys=new SystemInfo(stackedWidget);
    stackedWidget->addWidget(sys);

    hl=new QHBoxLayout(this);
    hl->addWidget(listWidget);
    hl->addWidget(stackedWidget);
    hl->setMargin(20);
    this->setLayout(hl);
    this->show();
    QObject::connect(listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(click_slot(int)));
    QObject::connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));
}


//[{"box":"box1","price":"38.00","count":"1432","name":"\u6c60\u5e956\u5e74","ctime":"1513321023","url":"http:\/\/jiu.veiding.com\/Uploads\/drink\/2017-12-15\/5a3369a422d0f.png"},{"box":"box2","price":"68.00","count":"1784","name":"\u6c60\u5e9510\u5e74","ctime":"1513321031","url":"http:\/\/jiu.veiding.com\/Uploads\/drink\/2017-12-15\/5a3369dc12063.png"},{"box":"box3","price":"118.00","count":"0","name":"\u6c60\u5e9520\u5e74","ctime":"1513321036","url":"http:\/\/jiu.veiding.com\/Uploads\/drink\/2017-12-15\/5a3369f335742.png"},
//{"box":"box4","price":"128.00","count":"43","name":"\u6c60\u5e95\u73cd\u85cf","ctime":"1513321041","url":"http:\/\/jiu.veiding.com\/Uploads\/drink\/2017-12-15\/5a336a15b9435.png"},{"box":"box5","price":"16.00","count":"100","name":"\u539f\u6d46\u9152","ctime":"1513321048","url":"http:\/\/jiu.veiding.com\/Uploads\/drink\/2017-12-15\/5a336a30df2d0.png"},{"box":"box6","price":"58.00","count":"89","name":"\u539f\u915253\u5ea6","ctime":"1513321052","url":"http:\/\/jiu.veiding.com\/Uploads\/drink\/2017-12-15\/5a336a47c0310.png"},
//{"username":null,"pass":null}]
