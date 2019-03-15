#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <QFile>

QString read_File(const QString &file_name);
int write_File(const QString &file_name, const QString &txt);
bool is_file_exits(QString fileFull);

int write_File_byte(QString file_name,QByteArray byte);
void create_Dir(QString dirPath);

void clear_dir_files(const QString &folderFullPath);

#endif // FILEHANDLER_H
