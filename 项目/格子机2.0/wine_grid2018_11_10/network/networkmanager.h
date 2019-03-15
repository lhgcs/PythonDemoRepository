#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "netcardinterface.h"


class NetWorkManager
{
public:
    NetWorkManager();
    ~NetWorkManager();
    void setSystartHandel(SysStart *SysStartHandle){ mSystart = SysStartHandle;};
    int checkNetWork();
    int Detection();


private:
    NetCardInterface *Interface;
    NetCardType Type;
    SysStart *mSystart;
};

#endif // NETWORKMANAGER_H
