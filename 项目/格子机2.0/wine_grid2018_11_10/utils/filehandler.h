#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QDir>

QString read_File(QString file_name);
int write_File(QString file_name,QString txt);
int write_File_byte(QString file_name,QByteArray byte);

void create_Dir(QString dirPath);
void create_File(QString fileFull);
bool is_file_exits(QString fileFull);
void remove_File(QString fileFull);
void write_log(char writebuf[],int len);

void delay_sec(int sec);

void clear_dir_files(const QString &folderFullPath);

#endif // FILEHANDLER_H
