#include "filehandler.h"
#include <QDir>
//读文件
QString read_File(const QString &file_name){
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
int write_File(const QString &file_name,const QString &txt){
    QFile f(file_name);
    if(!f.open(QIODevice::WriteOnly|QIODevice::Text)){
        return -1;
    }
    else{
        QByteArray byte=txt.toLatin1();//默认覆盖
        int len=f.write(byte);
        f.flush();
        f.close();
        return (len==byte.size())?len:-1;
    }
}

int write_File_byte(QString file_name,QByteArray byte){
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


#include <QFileInfo>
bool is_file_exits(QString fileFull){
    QFileInfo file_info(fileFull);
    if(file_info.isFile()){
        return true;
    }
    return false;
}

void create_Dir(QString dirPath){
    if(dirPath.endsWith("/")){
        dirPath.chop(1);
    }
    QDir dir(dirPath);
    if(!dir.exists()) {      //准确判断文件夹是否存在
        dir.mkpath(dirPath);//创建多级目录
    }
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


