#include "loopimage.h"
#include <QDir>

LoopImage::LoopImage(int status_arr[],QWidget *parent) : QWidget(parent)
{
    paint_flag=true;
    status_p=status_arr;
    x0=IMG_W*(-1);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(700,IMG_H);
    timer=new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(time_out_slot()));

    QStringList stringFilter;
//    stringFilter<<"*.jpg"<<"*.png"<<"*.jpeg";//设置过滤类型
    stringFilter<<"*.png";//设置过滤类型
    QDir dir("/home/image");
    if(dir.exists())
    {
        QStringList FileList = dir.entryList(stringFilter);//创建完整的路径
        int vector_size=FileList.count()<GOODS_TOTAL?FileList.count():GOODS_TOTAL;
        for(int i=0; i<vector_size; i++)
        {
            QString str=FileList.at(i);
            if(str.endsWith(".png"))
            {
                str.replace(".png","");
                int num=str.toInt()-1;
                if(num>=0&&(num<=GOODS_TOTAL-1))
                {
                    vector.append(str);
                }
            }
        }
    }
    load_pix();
    this->setMouseTracking(true);
    this->show();
}

void LoopImage::load_pix()
{
    pix.clear();
    if(vector.size()<=0)
    {
        vector.clear();
        for(int i=0;i<4;i++)
        {
            vector.append("sellout");
            pix.append(QPixmap("/home/ico/"+vector.at(0)+".png"));
        }
    }
    else
    {
        for(int i=0;i<vector.size();i++)
        {
            if(!status_p[vector.at(i).toInt()-1])
            {
                vector.erase(vector.begin()+i);
            }
        }

        switch(vector.size())
        {
            case 0:
            {
                vector.clear();
                for(int i=0;i<4;i++)
                {
                    vector.append("sellout");
                    pix.append(QPixmap("/home/ico/"+vector.at(0)+".png"));
                }
                break;
            }
            case 1:
            {
                QString str=vector.at(0);
                for(int i=0;i<3;i++)
                {
                    vector.append(str);
                }
                for(int i=0;i<4;i++)
                {
                    pix.append(QPixmap("/home/ico/"+vector.at(0)+".png"));
                }
                break;
            }
            case 2:
            {
                QString str1=vector.at(0);
                QString str2=vector.at(1);
                vector.append(str1);
                vector.append(str2);
                for(int i=0;i<2;i++)
                {
                    pix.append(QPixmap("/home/image/"+vector.at(0)+".png"));
                    pix.append(QPixmap("/home/image/"+vector.at(1)+".png"));
                }
                break;
            }
            case 3:
            {
                QString str=vector.at(0);
                vector.append(str);
                for(int i=0;i<3;i++)
                {
                    pix.append(QPixmap("/home/image/"+vector.at(i)+".png"));
                }
                pix.append(QPixmap("/home/image/"+vector.at(0)+".png"));
                break;
            }
            default:
            {
                for(int i=0;i<4;i++)
                {
                    pix.append(QPixmap("/home/image/"+vector.at(i)+".png"));
                }
                break;
            }
        }

    }
}
