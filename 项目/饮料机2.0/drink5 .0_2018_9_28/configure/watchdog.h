#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <QThread>

class WatchDog : public QThread
{
public:
    WatchDog();
    ~WatchDog(){}
    void run();
private:
    int fd;
};

#endif // WATCHDOG_H
