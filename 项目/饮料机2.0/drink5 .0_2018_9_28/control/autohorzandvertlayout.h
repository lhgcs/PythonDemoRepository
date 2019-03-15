#ifndef AUTOHORZANDVERTLAYOUT_H
#define AUTOHORZANDVERTLAYOUT_H

#include <QBoxLayout>
#include <QWidget>

class AutoHorzAndVertLayout
{
public:
    AutoHorzAndVertLayout(int _rowCount, int oneColumnCount, int totalCount, QWidget *w, QWidget *parent);
    ~AutoHorzAndVertLayout(){
        if(hl){
            delete[] hl;
        }
        if(vl){
            delete vl;
        }
        if(h){
            delete h;
        }
    }
    QVBoxLayout *get_layout(){
        return vl;
    }
    void set_margin(int left,int up,int right,int down){

    }

private:
    QHBoxLayout *hl;
    QHBoxLayout *h;
    QVBoxLayout *vl;
    int marginLeft,marginUp,marginRight,marginDown;
};

#endif // AUTOHORZANDVERTLAYOUT_H
