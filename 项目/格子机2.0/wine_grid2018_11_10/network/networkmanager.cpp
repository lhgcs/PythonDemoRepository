#include "networkmanager.h"

#include "Serial/serialclient.h"
#include "loadingpprogess.h"
#include "ProtocolInterface.h"
#include "MobileNetWorkProtocol.h"

#include <stdio.h>


NetWorkManager::NetWorkManager()
{
    Interface = NULL;
    Type = NETCARD_NO;
    mSystart = NULL;

}
NetWorkManager::~NetWorkManager()
{
    if(Interface)
    delete Interface;

    if(mSystart)
    delete mSystart;
}

int NetWorkManager::checkNetWork()
{

    NetCardInterface *CurInterface;

    CurInterface = new EthnetCard();

    if(!CurInterface->checkNetWorkIsAvailable("eth0", mSystart))
    {
        Interface = CurInterface;
        Type = NETCARD_ETH;
        return 0;
    }

    delete CurInterface;
    CurInterface = new MobileCard();
    if(!CurInterface->checkNetWorkIsAvailable("ppp0", mSystart))
    {
        Interface = CurInterface;
        Type = NETCARD_4G;
        return 0;
    }

    return -1;
}

int NetWorkManager::Detection()
{
    if(Interface)
        return 0;

    return Interface->netWorkDetection();

}
