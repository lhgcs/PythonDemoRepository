#include "firmwaredownload.h"
#include "httprequest.h"
#include "filehandler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <QJsonObject>
#include <QJsonDocument>

#include "app.h"

const unsigned char MonthStr[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov","Dec"};

QString FirmWareDownload::FirmWareFileFormat = "VD-1000_%1.Upgrade";
QString FirmWareDownload::mCurentVersion = "V2.0.4";

static QString mBuildVersion;


FirmWareDownload::FirmWareDownload(QObject *parent):QObject(parent)
{
    QString BuildDate;
    getBuildDate(BuildDate);
    mBuildVersion = QString("%1_%2").arg(mCurentVersion).arg(BuildDate);
}

int FirmWareDownload::getBuildDate(QString &BuildDate)
{
    unsigned char temp_str[4] = {}, i = 0;
    unsigned int Year = 0;
    unsigned int Month = 0;
    unsigned int Date = 0;
    char BuildDateTime[64];

    sscanf(__DATE__, "%s %d %d", temp_str, &(Date), &(Year));

    for (i = 0; i < 12; i++)
    {
        if (temp_str[0] == MonthStr[i][0] && temp_str[1] == MonthStr[i][1] && temp_str[2] == MonthStr[i][2])
        {
            Month = i + 1;
            break;
        }
    }

    sprintf(BuildDateTime, "%04d%02d%02d", Year, Month, Date);

    BuildDate = QString(BuildDateTime);

    return 0;
}

bool FirmWareDownload::isUpperServerVerison()
{
    int BuildMainVer = 0,BuildMinorVer = 0, BuildReviseVer = 0;
    int RemoteMainVer = 0,RemoteMinorVer = 0, RemoteReviseVer = 0;
    char BuildTime[40], RemoteTime[40];
    char *data;
    QByteArray ba;
    qDebug()<<"current verion :"<<mBuildVersion<<"server verion :"<<mServerVersion;


    ba = mBuildVersion.toLatin1();
    data = ba.data();

    sscanf(data, "V%d.%d.%d_%s",&BuildMainVer, &BuildMinorVer, &BuildReviseVer,BuildTime);

    ba = mServerVersion.toLatin1();
    data = ba.data();
    sscanf(data, "V%d.%d.%d_%s",&RemoteMainVer, &RemoteMinorVer, &RemoteReviseVer,RemoteTime);

//    mBuildVersion.sscanf("V%d.%d.%d_%s",BuildMainVer, BuildMinorVer, BuildReviseVer,BuildTime);
//    mServerVersion.sprintf("V%d.%d.%d_%s",RemoteMainVer, RemoteMinorVer, RemoteReviseVer,RemoteTime);

    qDebug()<<"BuildMainVer :"<<BuildMainVer<<"BuildMinorVer"<<BuildMinorVer<<"BuildReviseVer"<<BuildReviseVer;
    qDebug()<<"RemoteMainVer :"<<RemoteMainVer<<"RemoteMinorVer"<<RemoteMinorVer<<"RemoteReviseVer"<<RemoteReviseVer;

    if(RemoteMainVer > BuildMainVer)                                        //主版本号
    {
        return true;
    }
    else if(RemoteMainVer == BuildMainVer)
    {

        if(RemoteMinorVer > BuildMinorVer)                                 //次版本号
        {
            return true;
        }
        else if(RemoteMinorVer == BuildMinorVer)
        {
            if(RemoteReviseVer > BuildReviseVer)                           //修订版本号
            {
                return true;
            }
            else
            {
                return false;
            }

        }
        else
        {
            return false;
        }

    }
    else
    {
        return false;
    }

    return true;
}

bool FirmWareDownload::startDownloadFirmWare(QString Version)
{

    FirmWareFile.clear();
    mServerVersion = Version;
    FirmWareFile = QString(FirmWareFileFormat.arg(Version));

    return download();
}

QString FirmWareDownload::getSoftWareVersion()
{
    QString BuildDate;

    getBuildDate(BuildDate);

    mBuildVersion = QString("%1_%2").arg(mCurentVersion).arg(BuildDate);

    return mBuildVersion;
}


bool FirmWareDownload::checkVersion(QString url, QString &Version)
{
    QByteArray byte;
    QJsonDocument jsonDocument;
    QJsonParseError error;

    if(HttpRequest::get_net_source(url,byte, 50000))
    {
         qDebug()<<byte;

         jsonDocument = QJsonDocument::fromJson(byte,&error);
         if(error.error == QJsonParseError::NoError)
         {
             QJsonObject obj =jsonDocument.object();

             if (obj.contains("version"))
             {
                 QString fileName = obj.take("version").toString();         //获取文件名
                 qDebug()<<"server firmware :"<<fileName;
                 fileName.remove("VD-1000_");                               //去除文件名头
                 fileName.remove(".Upgrade");                               //去除文件后缀

                 Version = fileName;
                 mServerVersion = fileName;
                 return true;
             }
             else
             {
                 printf("no key\n");
             }
         }
         else
         {
             qDebug()<<"json error: "<<error.error;
         }
    }

    return false;
}

bool FirmWareDownload::download()
{
    QString url;
    QByteArray FileData;


    if(!FirmWareFile.isEmpty())
    {
        mCurentVersion = mServerVersion;
        create_Dir("/home/upgrade");

        QString LocalFileName="/home/upgrade/"+FirmWareFile;

        url = App::ip + "/update_version/grid2/" + FirmWareFile;

        qDebug()<<"download firmware "<<url;
        if(((HttpRequest::get_net_source(url,FileData, 300000)==false) || (FileData.isEmpty())))
        {
            return false;
        }
        else
        {
            int len=write_File_byte(LocalFileName, FileData);
            if(len>0)
            {
                qDebug()<<"firmware save ok";
                return true;
            }
            else
            {
                qDebug()<<"firmware save failed";
                return false;
            }
        }
    }

    return true;
}


