#ifndef USBWATCHER_H
#define USBWATCHER_H

#include <QObject>
#include <QFileSystemWatcher>//监视文件和目录修改
#include "usbscanergun.h"

class UsbWatcher : public QObject
{
    Q_OBJECT
public:
    explicit UsbWatcher(QObject *parent = 0);
    ~UsbWatcher(){
        gun->runFlag=false;
        delete fileWatcher;
    }
    UsbScanerGun *gun;
private:
     QFileSystemWatcher *fileWatcher;
     bool bMouse;
     int fd;

private slots:
    void slotFileChanged(const QString& path);/*槽函数，处理下面fileWatcher的信号*/
};

#endif // USBWATCHER_H
