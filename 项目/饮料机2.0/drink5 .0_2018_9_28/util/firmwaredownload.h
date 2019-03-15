#ifndef FIRMWAREDOWNLOAD_H
#define FIRMWAREDOWNLOAD_H

//#include <QThread>
#include <QObject>

//class FirmWareDownload:public QThread
class FirmWareDownload:public QObject
{
    Q_OBJECT
public:
    static QString FirmWareFileFormat;
    static QString mCurentVersion;
    FirmWareDownload(QObject *parent=0);

    static QString getSoftWareVersion();
    static int getBuildDate(QString &BuildDate);
    bool startDownloadFirmWare(QString Version);

    bool checkVersion(QString url, QString &Version);
    bool isUpperServerVerison();

private:

    bool download();
    QString FirmWareFile;
    QString mServerVersion;

    QString downUrl;
};

#endif // FIRMWAREDOWNLOAD_H
