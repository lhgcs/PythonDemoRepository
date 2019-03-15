#ifndef FILEWR_H
#define FILEWR_H
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

class FileWR
{
public:
    FileWR();
    ~FileWR();
    QString Read(QString file_name);
    void Write(QString file_name, QString str);
};

#endif // FILEWR_H
