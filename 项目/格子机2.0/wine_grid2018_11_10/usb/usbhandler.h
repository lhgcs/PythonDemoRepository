#ifndef USBHANDLER_H
#define USBHANDLER_H

#include <QObject>

#include <stdio.h>
#include <fcntl.h>		/* low-level i/o */
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <termios.h>

#include <QThread>

class UsbHandler: public QThread
{
    Q_OBJECT
public:
    UsbHandler();
    void run();
    int fd;
signals:
    void data(QString str);
};

#endif // USBHANDLER_H
