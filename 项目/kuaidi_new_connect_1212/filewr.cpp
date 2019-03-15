#include "filewr.h"


FileWR::FileWR()
{
}

FileWR::~FileWR()
{
}

QString FileWR::Read(QString file_name)
{
    QFile file(file_name);

    QString line = "";

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "error";
    }

    QTextStream in(&file);

    while (!in.atEnd()) {

        line = in.readLine();

        //process string line here
     }

    file.close();

    return line;
}

void FileWR::Write(QString file_name, QString str)
{
    QFile file(file_name);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

     QTextStream out(&file);

     out << str << "\n";

     file.close();
}
