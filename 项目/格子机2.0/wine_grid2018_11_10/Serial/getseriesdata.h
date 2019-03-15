#ifndef GETSERIESDATA_H
#define GETSERIESDATA_H

#include <QObject>
#include <QThread>
#include "serial.h"

class GetSeriesData : public QThread
{
    Q_OBJECT
public:
    GetSeriesData(char *tty);
    void run();
    int time;
signals:
    void data(QString str);
private:
    int fd;
};

#endif // GETSERIESDATA_H
