#ifndef SYSSTART_H
#define SYSSTART_H

#include <QWidget>
#include <QLabel>
#include <QMovie>

class SysStart :public QWidget
{
public:
    SysStart(QWidget *parent = 0);
    ~SysStart()
    {
        movie->stop();
        delete movie;
        delete label;
    }
private:
    QLabel *label;
    QMovie *movie;
};

#endif // SYSSTART_H
