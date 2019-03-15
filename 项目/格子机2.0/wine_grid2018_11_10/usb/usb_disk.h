#ifndef USB_DISK_H
#define USB_DISK_H

#include <QObject>
#include <QFileSystemWatcher>//监视文件和目录修改

class USB_Disk :public QObject
{
    Q_OBJECT
public:
    USB_Disk();
    ~USB_Disk(){delete fileWatcher;}
private slots:
    void slotFileChanged(const QString& path);/*槽函数，处理下面fileWatcher的信号*/
private:
     QFileSystemWatcher *fileWatcher;
     bool bMouse;
signals:
    void show_txt(int num);
};

#endif // USB_DISK_H
