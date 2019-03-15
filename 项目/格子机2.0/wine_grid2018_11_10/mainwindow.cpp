#include "mainwindow.h"
#include "Serial/getseriesdata.h"
#include "Upgrade/firmwaredownload.h"

#include "buttonradiogroup.h"
#include "Loading/jsonhandler.h"
#include "Setting/xmldom.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setObjectName("MainWindow");

//    if(is_file_exits("/home/conf/conf.xml")){
//        XmlDom xml;
//        if(!xml.readXML("/home/conf/conf.xml")){
//            DialogInstance::getInstance()->setText("配置文件出错");
//        }else{
//            write_File("/home/conf/http.txt",App::ip);
//            write_File("/home/conf/mac_id.txt",App::macId);
//            remove_File("/home/conf/conf.xml");
//        }
//    }

    App::ip = read_File("/home/conf/http.txt");
    App::macId = read_File("/home/conf/mac_id.txt");

    QString str = read_File("/home/conf/passwd.txt");
    QStringList list = str.split(":");
    if(list.size() >= 2){
        App::user = list.at(0);
        App::password = list.at(1);
    }

    QString ss = read_File("/home/conf/device.txt");

    QJsonDocument jsonDocument;
    if(JsonHandler::string_to_json(ss,jsonDocument)){
        for(int i=0;i<App::deviceNum;i++){
            QJsonValue jsonValue;
            if(JsonHandler::get_json_object(jsonDocument,QString::number(i+1),jsonValue)){
                App::deviceVisual[i]=JsonHandler::get_json_value_bool(jsonValue,true);
            }
        }
    }

    qDebug()<<App::ip<<App::macId
    <<App::user<<App::password
    <<App::DeskWidth<<App::DeskHeight;

//    App::macId="gz182709";

    this->setFixedSize(App::DeskWidth,App::DeskHeight);
    this->show();

    Toast::getInstance();
    DialogInstance::getInstance();
    SqliteGoods::getInstance();

    sys=new SysStart();
    load=new Load();
    load->setSysStartHanlde(sys);
    load->set_sqliteGoods(SqliteGoods::getInstance());
    connect(load, SIGNAL(finish(bool)), this, SLOT(load_finish(bool)));
    load->start();
//    init_widget();
}

void MainWindow::init_widget(){
    Toast::getInstance()->set_text("正在检查系统版本，请勿操作",5000);
    QString url=App::ip+"/update_version/grid2/check.php";
    qDebug()<<url;
    FirmWareDownload firmWareDownload;

    QString version_new;
    firmWareDownload.checkVersion(url,version_new);

    if(firmWareDownload.isUpperServerVerison()){
        Toast::getInstance()->set_text("检查到新版本,开始下载固件",3000);
        if(firmWareDownload.startDownloadFirmWare(version_new)){
            Toast::getInstance()->set_text("固件下载成功,系统将重启",3000);
            QCoreApplication::processEvents();
            QCoreApplication::processEvents();
            sleep(5);
            system("reboot");
        }else{
            Toast::getInstance()->set_text("固件下载失败");
        }
    }else{
        Toast::getInstance()->set_text("系统是最新版本");
    }


    goodsPresent = NULL;
    login = NULL;
    loginClick=0;
    loadingQrcode=NULL;
    putOutTip=NULL;
    replenishment=NULL;
    ensureDialog = NULL;

    statusBar=new StatusBar(this);
    stackWidget = new QStackedWidget(this);
    stackWidget->setFixedSize(App::DeskWidth,App::DeskHeight*2/3);

    selectGoods=new SelectGoods[5];
    for(int i=0;i<5;i++){
        selectGoods[i].init_content(i*18,18);
        QObject::connect(&selectGoods[i],SIGNAL(num(int,int)),this,SLOT(widget_slot(int,int)));
        stackWidget->addWidget(&selectGoods[i]);
    }

    ButtonRadioGroup *group=new ButtonRadioGroup(this);
    QObject::connect(group,SIGNAL(click(int)),this,SLOT(group_slot(int)));
    group_slot(65);
    group->set_check(0);
    for(int i=0;i<5;i++){
        if(App::deviceVisual[i] == true){
            group->set_visiable(i,true);
        }else{
            group->set_visiable(i,false);
        }
    }

    QVBoxLayout *hl2=new QVBoxLayout(this);
    hl2->setMargin(0);
    hl2->addWidget(statusBar,0,Qt::AlignRight);
    hl2->addWidget(stackWidget);
    hl2->addWidget(group);

    this->setLayout(hl2);
    this->show();

    mIcCardClient = new SerialClient("/dev/ttyS1", NULL);
    Interface = new IcCardInterface(mIcCardClient->getFd());
    mIcCardClient->setParams(BITRATE_9600, DATA_8BIT, STOP_1BIT, NO_ANY_FLOW_CONTROL, PARITY_NONE);
    mIcCardClient->setInterface(Interface);
//    读卡器推送数据
    QObject::connect(Interface->getNotifyHandle(),SIGNAL(icCardNumberReadable(QString)),
                     this,SLOT(send_iccard_slots(QString)));

    char tty[20]="/dev/ttyS3";
    GetSeriesData *getSeriesData=new GetSeriesData(tty);
    QObject::connect(getSeriesData,SIGNAL(data(QString)),this,SLOT(getSeriesData_slots(QString)));
    getSeriesData->time=600;

    QObject::connect(HttpRequest::getInstance(), SIGNAL(data(int,int)),this,SLOT(httpRequest_slot(int,int)));
    serialPutOut=new SerialPutOut;
    QObject::connect(serialPutOut, SIGNAL(serial_data(int,int)),this,SLOT(serialPutOut_slot(int,int)));
    timerPutOut=new QTimer(this);
    QObject::connect(timerPutOut, SIGNAL(timeout()),this,SLOT(timerPutOut_slot()));

//    buttonGroup = new QButtonGroup;
//    buttonGroup->addButton(btnLeftPage,1);
//    buttonGroup->addButton(btnRightPage,2);
//    QObject::connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));

//    timerClickButton=new QTimer(this);
//    timerClickButton->setSingleShot(true);
//    timerClickButton->setInterval(300);
//    QObject::connect(timerClickButton, SIGNAL(timeout()), this, SLOT(timerClickButton_slot()));

    HttpRequest::getInstance()->send_time();
    timerFixedCommunication=new QTimer(this);
    QObject::connect(timerFixedCommunication,SIGNAL(timeout()),this,SLOT(timerFixedCommunication_slot()));

    timerReplenishment=new QTimer(this);
    timerReplenishment->setInterval(3000);
    QObject::connect(timerReplenishment, SIGNAL(timeout()), this, SLOT(timerReplenishment_slot()));
    timerFixedCommunication->start(180000);
    timerFixedCommunication_slot();

    statusBar->show_time();
    serialPutOut->start();
    getSeriesData->start();
    mIcCardClient->start();

    //serialPutOut->open_box();
}

//void MainWindow::UpdateGoodInfo()
//{
//    for(int i=0;i<App::goodsNum;i++){
//        sqliteGoods->get_table_data(QString::number(i+1),
//                                goodInfo[i].name,
//                                goodInfo[i].price,
//                                goodInfo[i].remain,
//                                goodInfo[i].sales);
////        goodsTable->goodsDisplay.at(i)->setPresent(goodInfo[i].name,
////                                               goodInfo[i].price,
////                                               goodInfo[i].remain);

//        qDebug()<<"goodsTable->goodsDisplay.at "<<i;
//    }
//}

MainWindow::~MainWindow(){
}



///*RadioButton样式设置*/
//QRadioButton::indicator {
//    width: 17px;
//    height: 17px;
//}
///*单选框未选中样式*/
//QRadioButton::indicator::unchecked {

//    image: url(:/ico/radiobutton-unchecked-md.png);
//}
///*单选框选中样式*/
//QRadioButton::indicator::checked {
//    image: url(:/ico/radiobutton-checked-md.png);
//}
///*RadioButton和checkbox字体和间距设置*/
//QRadioButton ,QCheckBox{
//    spacing: 5px;
//    font-size: 15px;
//}
///*checkbox样式设置*/
//QCheckBox::indicator {
//    width: 26px;
//    height: 50px;
//}
///*未选中*/
//QCheckBox::indicator::unchecked {
//    image: url(:/ico/checkbox_unchecked.png);
//}
///*选中*/
//QCheckBox::indicator::checked {
//    image: url(:/ico/checkbox_checked.png);
//}
//error: undefined reference to `FirmWareDownload::FirmWareDownload(QObject*)'
