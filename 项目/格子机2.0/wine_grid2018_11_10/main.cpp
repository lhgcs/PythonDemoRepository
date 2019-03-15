#include "mainwindow.h"
#include <QApplication>

#include "subwidget/dialoginstance.h"
#include <QPropertyAnimation>
#include "pickupcode.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    App::set_font(a);
    App::set_qss(":/ui.qss");

    int ret=-1;
    do{

//        PickUpCode p;
//        p.show();

        MainWindow w;
        //QPropertyAnimation
//        QPropertyAnimation *animation = new QPropertyAnimation(&w,"windowOpacity");
//        animation->setDuration(10000);
//        animation->setStartValue(0);
//        animation->setEndValue(1);
//        animation->start();
        w.show();
        ret =a.exec();
    }while (ret!=0);

    return ret;
}
