#include "filehandler.h"

#include <QFile>
#include <QFileInfo>

//读文件
QString read_File(QString file_name){
    QFile f(file_name);
    if(!f.open(QIODevice::ReadOnly|QIODevice::Text)){
        return NULL;
    }
    else{
        QString str=f.readAll();
        str.replace(QString("\n"), QString(""));
        f.close();
        return str;
    }
}
//写文件
int write_File(QString file_name,QString txt)
{
    QFile f(file_name);
    if(!f.open(QIODevice::WriteOnly|QIODevice::Text)){
        return -1;
    }
    else{
        QByteArray byte=txt.toLatin1();//默认覆盖
        int len=f.write(byte);
        f.close();
        return (len==byte.size())?len:-1;
    }
}
int write_File_byte(QString file_name,QByteArray byte)
{
    QFile f(file_name);
    if(!f.open(QIODevice::WriteOnly)){
        return -1;
    }
    else{
        int len=f.write(byte);
        f.close();
        return (len==byte.size())?len:-1;
    }
}


void create_Dir(QString dirPath)
{
    if(dirPath.endsWith("/")){
        dirPath.chop(1);
    }
    QDir dir(dirPath);
    if(!dir.exists()) {      //准确判断文件夹是否存在
        dir.mkpath(dirPath);//创建多级目录
    }
}
#include <QDebug>
bool is_file_exits(QString fileFull){
    QFileInfo file_info(fileFull);
    if(file_info.isFile()){
        return true;
    }
    return false;
}

void create_File(QString fileFull)
{
    int index=fileFull.lastIndexOf ("/");                       //字符串的最后一次出现的索引
    QString path=fileFull.left(index);
    //QString name=file_full.right(file_full.length()-index-1); //从右边截取
    create_Dir(path);
    if(!is_file_exits(fileFull)){                               //准确判断文件是否存在
        QFile f(fileFull);
        f.open(QIODevice::WriteOnly);
        f.close();
    }
    //调用open的时候只要用QIODevice::WriteOnly或者QIODevice::ReadWrite，如果文件不存在，就会创建
}

void remove_File(QString fileFull){
    QFile::remove(fileFull);
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
//#include <string.h>

#define LOG_SIZE 5120000
void write_log(char writebuf[],int len)
{
    static int pos=0;
    static int fd=-1;
    if(fd<0){
        fd=open("/home/log/log.txt", O_WRONLY|O_CREAT,0666);//注意之前自己定义个a.txt  S_IWUSR|S_IRGRP
    }
    if(fd>0){
        if(pos>LOG_SIZE) {       //超出文件
            lseek(fd,0,SEEK_SET);//文件开头
        }
        else {
            lseek(fd,pos,SEEK_SET);
        }
        write(fd, writebuf,len); // strlen(writebuf)
        pos=lseek(fd,0,SEEK_CUR);//当前的偏移量
    }
//    close(fd);
}


#include <QCoreApplication>
#include <QDateTime>

void delay_sec(int sec){
    QDateTime n=QDateTime::currentDateTime();
    QDateTime now;
    do{
       now=QDateTime::currentDateTime();
       QCoreApplication::processEvents();
       //a.processEvents();        //使程序在显示启动画面的同时仍能响应鼠标其他事件
    } while (n.secsTo(now)<=sec);//5为需要延时的秒数
}

void clear_dir_files(const QString &folderFullPath)
{
    QDir dir(folderFullPath);
    dir.setFilter(QDir::Files);
    int fileCount = dir.count();
    for (int i = 0; i < fileCount; i++){
        dir.remove(dir[i]);
    }
}

