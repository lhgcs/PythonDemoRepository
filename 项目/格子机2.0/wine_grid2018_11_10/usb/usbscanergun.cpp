#include "usbscanergun.h"

UsbScanerGun::UsbScanerGun(QObject *parent)
{
    fd=-1;
    runFlag=false;
}

