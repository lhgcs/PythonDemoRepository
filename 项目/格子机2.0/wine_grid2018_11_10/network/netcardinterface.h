#ifndef NETCARDINTERFACE_H
#define NETCARDINTERFACE_H

#include "netutil.h"
#include <QString>
#include "loadingpprogess.h"
#include "sysstart.h"
#include "Serial/serialclient.h"
#include "ProtocolInterface.h"
#include "MobileNetWorkProtocol.h"





typedef enum{
    ERROR_CODE_OK                               = 0,
    ERROR_CODE_MOBILENETWORK_SIMCARD_NOT_INSERT  = -1,
    ERROR_CODE_MOBILENETWORK_REGISTRATION_FAILED = -2,
    ERROR_CODE_MOBILENETWORK_GET_STRENGTH_FAILED = -3,
}ERROR_CODE;


class NetCardInterface{
public:
    NetCardInterface();
    ~NetCardInterface();
    QString getIp();
//    virtual int getNetWorkStatus() = 0;
     virtual int checkNetWorkIsAvailable(const QString &NetWorkInterface, SysStart *mSystart) = 0;
     virtual int netWorkDetection() = 0;

    NetDevice *mNetHanle;

};


class EthnetCard :public NetCardInterface{

public:
    EthnetCard(){};
    ~EthnetCard(){};
    int checkNetWorkIsAvailable(const QString &NetWorkInterface, SysStart *mSystart);
    int netWorkDetection();
};


class MobileCard :public NetCardInterface{

public:
    MobileCard();
    ~MobileCard();
     int checkNetWorkIsAvailable(const QString &NetWorkInterface, SysStart *mSystart);
     int netWorkDetection();

private:
    int MobileNetDilogDetecttion();
    int MobileNetDilogOn();
    int MobileNetDilogOff();
    int checkSimCard(LoadingpProgess *mProgess);

    SerialClient *mNetClient;
    static QString DilogOnScript;
    static QString DilogOffScript;
};



#endif // NETCARDINTERFACE_H
