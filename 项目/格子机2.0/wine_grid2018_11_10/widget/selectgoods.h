#ifndef SELECTGOODS_H
#define SELECTGOODS_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QButtonGroup>
#include "app.h"
#include <QList>
#include <QDebug>
#include "subwidget/goodsgrid.h"
#include <QThread>

class SelectGoods : public QWidget
{
    Q_OBJECT
public:
    explicit SelectGoods(QWidget *parent = 0);
    ~SelectGoods(){
        delete []goodsGrid;
        delete buttonGroup;
        delete gl;
    }
signals:
    void num(int layout,int value);
private:
    GoodsGrid *goodsGrid;
    QButtonGroup *buttonGroup;
    QGridLayout *gl;
    QList<QIcon> listIcon;
private slots:
    void keyPress(int value){
        if((value>=0)){
            emit num(App::selectGoodsLayout,value);
        }
    }
public:
//    void set_content(int sark,int page){
//        if((sark>=0)&&(sark<App::selectDeviceNum)){
//            for (int i=0;i<App::selectGoodsNum_page;i++){
//                int index=page*App::selectGoodsNum_page+i+1;
//                button[i].setIcon(QIcon(QString("/home/image/%1").arg(QString::number(index+sark*App::selectDevice_GoodsNum))));
//                button[i].setText("编号:"+QString::number(index));
//            }
//        }
//    }
//    void clear_content(){
//        for (int i=0;i<App::onePageGoodsNum;i++){
//            //button[i].hide();
//        }
//    }
    void init_content(const int &sark,const int &num){
        mSark=sark;
        mNum=num;
        QThread *thread = new QThread();
        goodsGrid->moveToThread(thread);
        QObject::connect(thread, &QThread::started, this, &SelectGoods::thread_init);//在run()函数执行前会发射信号started()
        connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()),Qt::QueuedConnection);
        thread->start();
    }

    int mSark;
    int mNum;
    void thread_init(){
        goodsGrid->set_content(mSark,mNum);
    }

//    void set_content(const int &sark,const int &page){
//        for (int i=0;i<App::onePageGoodsNum;i++){
//            int index=page*App::onePageGoodsNum+i;
//            button[i].setIcon(listIcon.at(index+sark*App::oneDeviceGoodsNum));
//            button[i].setText("编号:"+QString::number(index+1));
//        }
//    }

//    void set_content(const int &sark){
//        for (int i=0;i<App::onePageGoodsNum;i++){
//            button[i].setIcon(listIcon.at(sark*App::oneDeviceGoodsNum+i));
//            button[i].setText("编号:"+QString::number(i+1));
//        }
//        clear_content();
//        if((sark>=0)&&(sark<App::deviceNum)){
//            for (int i=0;i<App::onePageGoodsNum;i++){
//                button[i].setIcon(QIcon(QString("/home/image/%1").arg(QString::number(i+sark*App::oneDeviceGoodsNum))));
//                button[i].setText("编号:"+QString::number(i+1));
//            }
//        }
//    }

//    void set_content_init(){
//        for (int i=1;i<App::selectGoodsNum_page;i++){
//            listIcon_A.append(QIcon("/home/image/"+QString::number(i)));
//            listIcon_B.append(QIcon("/home/image/"+QString::number(i+App::se)));
//            listIcon_c.append(QIcon("/home/image/"+QString::number(i)));
//            listIcon_D.append(QIcon("/home/image/"+QString::number(i)));
//            listIcon_E.append(QIcon("/home/image/"+QString::number(i)));
//        }
//    }
};

#endif // SELECTGOODS_H
