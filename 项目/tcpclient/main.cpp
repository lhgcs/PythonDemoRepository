#include <QCoreApplication>

#include "tcpthread.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TCPThread tcp;

    return a.exec();
}

