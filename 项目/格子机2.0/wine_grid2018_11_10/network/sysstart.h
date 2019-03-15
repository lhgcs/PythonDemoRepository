#ifndef SYSSTART_H
#define SYSSTART_H

#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QStyleOption>
#include "loadingpprogess.h"

#define OFFSET_ADDRESS_X 540
#define OFFSET_ADDRESS_Y 260
#define CIRCLE_R1 150
#define CIRCLE_R2 130
#define CIRCLE_R3 110

class SysStart :public QWidget
{
    Q_OBJECT

public:
    SysStart(QWidget *parent=0);
    ~SysStart() {}
     LoadingpProgess *Progess;

public slots:
    void LodingExectionSlot(QString info);

private:
    int id;
    QLabel *ExcetionInfo;
    bool isLoading;
    QVBoxLayout *layout;

    void paintEvent(QPaintEvent *);

//    NetWorkManager *NetWork;
};

#endif // SYSSTART_H
