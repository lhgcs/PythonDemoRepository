#include "netcardinterface.h"

#include "networkmanager.h"

#include "Serial/serialclient.h"
#include "loadingpprogess.h"
#include "ProtocolInterface.h"
#include "MobileNetWorkProtocol.h"
#include "dhcputil.h"
#include "pingutil.h"

#include <stdio.h>
#include <QDebug>
#include <QThread>
#include <QProcess>

#include "Serial/serial.h"

/*#######################################NetCardInterface#####################################################*/

NetCardInterface::NetCardInterface(){
    mNetHanle = NULL;
}


NetCardInterface::~NetCardInterface()
{
    if(mNetHanle)
    {
        Net_DelInitNetCard(mNetHanle);
        delete mNetHanle;
        mNetHanle = NULL;

    }
}

QString NetCardInterface::getIp()
{
    char IpString[20]={0};

    if(mNetHanle != NULL)
    {
       Net_getIpAddrString(mNetHanle, IpString , 20);

       return QString(IpString);
    }
    return QString();
}

/*#######################################EthnetCard#####################################################*/

int EthnetCard::checkNetWorkIsAvailable(const QString &NetWorkInterface, SysStart *mSystart)
{
    int ret = 0;
    int i = 0;
    NetDevice *network =NULL;
    QByteArray ba = NetWorkInterface.toLatin1();

    //检测移动网络
    LoadingpProgess *Progess = mSystart->Progess;
    Progess->setLoadingItemInfo("正在检测有线网络");
    Progess->setProgessRange(0,2);

    ret = Net_isExistNetCable(ba.data());
    if(ret < 0)
    {
        printf("not ethnet cable\n");
        return ret;
    }

    Progess->setProgessValue(1);

    network = new NetDevice;

    Net_InitNetCard(network ,ba.data());


    for(i = 0; i< 3; i++)
    {
        if(!DhcpClient(network, DhcpHandler, NULL))
        {
            break;
        }
    }

    if(i == 3)
    {
        Net_DelInitNetCard(network);
        delete network;
        ret =  -1;
        return ret;
    }

    Progess->setProgessValue(2);

    mNetHanle = network;

    return ret;
}

int EthnetCard::netWorkDetection()
{
    return 0;
}

/*#######################################MobileCard#####################################################*/

QString MobileCard::DilogOnScript  = "/home/pppd/dilog-on.sh";
QString MobileCard::DilogOffScript = "/home/pppd/dilog-off.sh";

MobileCard::MobileCard()
{
    mNetClient = new SerialClient("/dev/ttyUSB1", NULL);
    ProtocolInterface *Interface = new MobileNetWorkProtocol(mNetClient->getFd());
    mNetClient->setInterface(Interface);
    mNetClient->start();
}

MobileCard::~MobileCard()
{
    mNetClient->quit();
    delete mNetClient;
}

int MobileCard::MobileNetDilogOn()
{
   // QProcess *DilogOn = new QProcess();
    QProcess DilogOn;

    DilogOn.start (DilogOnScript);
    DilogOn.waitForFinished(5000);

    //delete DilogOn;

    return 0;
}

int MobileCard::MobileNetDilogOff()
{
    //QProcess *DilogOff = new QProcess();
    QProcess DilogOff;

    DilogOff.start (DilogOffScript);
    DilogOff.waitForFinished(5000);

    //delete DilogOff;

     return 0;
}

int MobileCard::MobileNetDilogDetecttion()
{
    int fd = -1;
    char buffer[64];

    fd  = open("/sys/class/net/ppp0/operstate", O_RDONLY);//读取/sys/class/net/ppp0/operstate来监控数据网络数据的状态
    if (fd >= 0)
    {
        buffer[0] = 0;
        read(fd, buffer, sizeof(buffer));
        close(fd);

        printf("buffer = %s", buffer);

        if(!strncmp(buffer, "up", strlen("up")) || !strncmp(buffer, "unknown", strlen("unknown"))) {
            printf("Detect PPP state in on\n");

        } else
        {
            return -1;
        }

    } else {
        printf("Can not detect PPP state in off. Will retry dilog again fd :%d\n", fd);
        return -1;
    }

    return 0;
}

int  MobileCard::checkSimCard(LoadingpProgess *mProgess)
{
    int ret = 0;

    if( mProgess == NULL)
    {
        qDebug()<<"load goods info failed";
        return false;
    }

    qDebug()<<"checkSimCard";


    qDebug()<<"MOBILE_NET_WORK_GET_SIM_STATUS";
    QThread::msleep(500);

    //检测是否存在sim卡
    int sim_status = 0;
    ret = mNetClient->sendData(MOBILE_NET_WORK_GET_SIM_STATUS, (void *)&sim_status);
    qDebug()<<"sim card is exits"<<sim_status;
    if(ret < 0 || sim_status == 0)
    {
        return ERROR_CODE_MOBILENETWORK_SIMCARD_NOT_INSERT;
    }
    mProgess->setProgessValue(1);

    QThread::msleep(800);
    RegistrationState state;


    //检测当前网络信号
    SignalStrength Strength;
    Strength.rssi = 0;
    Strength.ber = 0;
    ret = mNetClient->sendData(MOBILE_NET_WORK_GET_SIGNAL_STRENGTH, (void *)&Strength);
    if(ret < 0 || (Strength.rssi == 0 && Strength.rssi == 99))
    {
        printf("get signal strength failed\n");
        return ERROR_CODE_MOBILENETWORK_GET_STRENGTH_FAILED;
    }
    mProgess->setProgessValue(2);

    printf("signa strength  rssi:%d  ber:%d\n", Strength.rssi, Strength.ber);
    QThread::msleep(800);


    for(int i = 0; i< 5; i++)
    {
        //检测sim卡是否注册成功

        state.ci = 0;
        state.stat = 0;
        ret = mNetClient->sendData(MOBILE_NET_WORK_GET_REGISTRATION_STATE, (void *)&state);
        printf("RegistrationState ci:%d  stat:%d\n", state.ci, state.stat);
        if(state.stat == 1)
        {
            printf("network registration successful\n");
            break;
        }
        QThread::msleep(800);

    }
    if(ret < 0 ||  state.stat != 1)
    {
        printf("network registration failed\n");
        return ERROR_CODE_MOBILENETWORK_REGISTRATION_FAILED;
    }
    mProgess->setProgessValue(3);


    return ERROR_CODE_OK;
}


int MobileCard::checkNetWorkIsAvailable(const QString &NetWorkInterface, SysStart *mSystart)
{
    int ret = 0;
    int index = 0;
    PingReslut Reslut;
    int check_index;

    //检测移动网络
    LoadingpProgess *Progess = mSystart->Progess;
    Progess->setLoadingItemInfo("正在检测移动网络");
    Progess->setProgessRange(0,5);

    for(index= 0; index<2; index++)
    {
        ret = checkSimCard(Progess);
        if(ret == 0)
        {
            break;
        }

    }
    if(ret < 0)
    {
        if(ret == ERROR_CODE_MOBILENETWORK_SIMCARD_NOT_INSERT)
        {
            mSystart->LodingExectionSlot("请检查SIM卡是否插入并重新启动系统");
            return ret;
        }
        else if(ret == ERROR_CODE_MOBILENETWORK_GET_STRENGTH_FAILED)
        {
            mSystart->LodingExectionSlot("信号强度过弱,请检查网络信号");
            return ret;
        }
        else if(ret == ERROR_CODE_MOBILENETWORK_REGISTRATION_FAILED)
        {
            mSystart->LodingExectionSlot("网络注册失败,检查SIM卡是否欠费");
            return ret;
        }
    }

    for(check_index = 0; check_index<5; check_index++)
    {
        int i=0;
        for(index = 0; index < 4; index++)
        {
            MobileNetDilogOff();                //停止拨号
            QThread::msleep(3000);

            MobileNetDilogOn();                 //开始拨号
            QThread::msleep(2000);              //5s
            i=0;
            while(MobileNetDilogDetecttion()&&i<3)   //检测拨号状态
            {
                i++;
                QThread::msleep(1500);
            }

            if(i<3){
                qDebug()<<"mobile net dialog seccess!";
                break;
            }


//            qDebug()<<"mobile net dialog seccess!";
//            if(!MobileNetDilogDetecttion())     //检测拨号状态
//            {
//                qDebug()<<"mobile net dialog seccess!";
//                break;
//            }

            qDebug()<<"mobile net dialog failed! count :"<<index;

//            MobileNetDilogOff();                //停止拨号
        }

        if(i>=3){
            continue;
        }

         //ping网络
        if(ping((char *)"114.114.114.114", 4, 4, &Reslut) > 0)
        {
            qDebug()<<"ping sucess\n";
            break;
        }

       qDebug()<<"mobile net redialog! count :"<<check_index;
    }
    if(check_index == 5)
    {
         mSystart->LodingExectionSlot("移动网络拨号失败");
         return -1;
    }


    Progess->setProgessValue(4);



    return ret;
}


int MobileCard::netWorkDetection()
{
    return MobileNetDilogDetecttion();
}
