#ifndef GLOBALAPPLICATION_H
#define GLOBALAPPLICATION_H

#include <QApplication>
//#include <QDebug>
//#define QDBG qDebug()<<__FILE__<<__FUNCTION__<<__LINE__

class GlobalApplication : public QApplication
{
public:
     GlobalApplication(int &argc,char **argv);
     ~GlobalApplication();
     bool notify(QObject*, QEvent *);
     static int closeSec;
};

#endif // GLOBALAPPLICATION_H
