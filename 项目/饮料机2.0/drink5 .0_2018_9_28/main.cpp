#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include "configure/app.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    App::set_font(a);
    App::set_qss(":/configure/ui.qss");
    QMatrix matrix;
    //QSplashScreen screen=App::boot_animation(matrix,"/home/ico/1.png");

//    QFont font;
//    font.setPixelSize(24);
//    font.setFamily("simhei");
//    a.setFont(font);

    MainWindow *main=new MainWindow;               //项目主窗口
    QGraphicsScene *scene = new QGraphicsScene;    //容器
    QGraphicsProxyWidget *w = scene->addWidget(main);
    w->setRotation(90);
    QGraphicsView *view = new QGraphicsView(scene);//视图
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setWindowFlags(Qt::FramelessWindowHint);
    view->setFixedSize(App::screenHeight,App::screenWidget);
    view->show();

    main->init_network();
    //screen.finish(main);//在主窗体对象初始化完成后,结束启动画面

    return a.exec();
}
