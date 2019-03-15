#include "autohorzandvertlayout.h"

AutoHorzAndVertLayout::AutoHorzAndVertLayout(int oneRowCount, int oneColumnCount, int totalCount, QWidget *w, QWidget *parent)
{
    if(oneColumnCount<=0 || totalCount<0){
        return ;
    }
    if(w == NULL){
        return ;
    }

    int current=0;
    int rowCount=totalCount/oneColumnCount;
    int remainCount=totalCount%oneColumnCount;

    hl=new QHBoxLayout[rowCount];
    for(int row=0;row<rowCount;row++){
        for(int column=0;column<oneColumnCount;column++){
            hl[row].addWidget(&w[current++],0, Qt::AlignHCenter);
        }
    }

    vl=new QVBoxLayout(parent);
    for(int row=0;row<rowCount;row++){
        vl->addLayout(&hl[row]);
    }

    if(remainCount!=0){
        h=new QHBoxLayout;
        for(int column=0;column<remainCount;column++){
            h->addWidget(&w[current++],0, Qt::AlignHCenter);
        }
        vl->addLayout(h);
    }
    if(parent){
        parent->setLayout(vl);
    }
}

