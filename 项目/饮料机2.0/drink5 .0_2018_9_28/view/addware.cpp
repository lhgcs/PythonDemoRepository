#include "addware.h"

AddWare::AddWare(int _w, int _h, QWidget *parent) : BaseSubWidget(parent){
    this->setObjectName("AddWare");
    this->setFixedSize(_w,_h);

    btn=new QPushButton[10];
    signalMapper = new QSignalMapper(this);
    QStringList list;
    list<<"全选"<<"一键补货"<<"上一页"<<"下一页"<<"上传"<<"下载"<<"校正电机"<<"设置"<<"参数"<<"退出";
    for (int i=0; i<10; i++){
        btn[i].setText(list.at(i));
        btn[i].setMaximumSize(200,80);
        btn[i].setMinimumSize(70,45);
        btn[i].setFocusPolicy(Qt::NoFocus);
        signalMapper->setMapping(&btn[i], i);
        QObject::connect(&btn[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
    }

//    btn[8].setText(list.at(8));
//    btn[8].setMaximumSize(200,200);
//    btn[8].setMinimumSize(70,45);
//    btn[8].setFocusPolicy(Qt::NoFocus);
//    signalMapper->setMapping(&btn[8], 8);
//    QObject::connect(&btn[8], SIGNAL(clicked()), signalMapper, SLOT(map()));

    QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(btn_press(int)));

    QGridLayout *gl=new QGridLayout;
    gl->setMargin(0);
    int l=0;
    for(int i=0;i<2;i++){
        for(int j=0;j<5;j++){
//            if(l<8){
                gl->addWidget(&btn[l],i,j,1,1);
                l++;
//            }
        }
    }
//    gl->addWidget(&btn[8],0,4,2,1);

    int rowCount=10;  //行数
    int columnCount=2;//列数
    int pageCount=0;  //页数
    int itemCount=App::goodsTotal;

    pageCount=itemCount/(rowCount*columnCount);
    if(itemCount%(rowCount*columnCount) != 0){
        pageCount++;
    }

    qDebug()<<"pageCount"<<pageCount;
    int w=App::screenWidget-20;
    int h=(App::screenHeight*2.0/3);

    addOrReduce=new AddOrReduce[itemCount];
    stackedLayout=new QStackedWidget();
    stackedLayout->setContentsMargins(5,0,5,0);
    stackedLayout->setFixedSize(w,h-150);
    stackedLayout->setAttribute(Qt::WA_TranslucentBackground, true);
    int k=0;
    for(int p=0;p<pageCount;p++){
        QWidget *widget=new QWidget(this);
        widget->setWindowOpacity(0.0);
        widget->setAttribute(Qt::WA_TranslucentBackground, true);
        widget->setFixedSize(w-10,h-150);
        QGridLayout *gl=new QGridLayout();
        gl->setMargin(0);
        gl->setVerticalSpacing(0);
        gl->setHorizontalSpacing(0);

        for(int i=0;i<rowCount;i++){
            for(int j=0;j<columnCount;j++){
                if(++k>itemCount){
                    break;
                }else{
                    int index=i*columnCount+j+p*rowCount*columnCount;
                    gl->addWidget(&addOrReduce[index],i,j);
                    addOrReduce[index].set_id(k);
                    addOrReduce[index].set_limit(0,10);
                }
                //qDebug()<<k<<i<<j<<i*columnCount+j;
            }
            if(k>itemCount){
                break;
            }
        }
        widget->setLayout(gl);
        stackedLayout->addWidget(widget);
    }

    QVBoxLayout *vl=new QVBoxLayout(this);
    vl->setContentsMargins(10,10,10,10);
    //vl->addStretch();
    vl->addWidget(stackedLayout);
    vl->addLayout(gl);
    vl->addStretch();
    setLayout(vl);

    index=0;
    stackedLayout->setCurrentIndex(0);
    this->show();
}

