#ifndef GOODSTABLE_H
#define GOODSTABLE_H

#include <QWidget>
#include <QEvent>
#include <QGridLayout>
#include "goodsdisplay.h"
#include <QDebug>
#include "app.h"
#include <QMap>
#include "Sqlite/sqlitegoods.h"

class GoodsTable : public QWidget
{
    Q_OBJECT
public:
    explicit GoodsTable(QWidget *parent = 0);
    ~GoodsTable(){
        for(int i=0;i<goodsDisplay.size();i++){
            delete goodsDisplay.at(i);
        }
        delete gl;
    }
     QVector<GoodsDisplay *>goodsDisplay;
private:
    QGridLayout *gl;
signals:
    void data(int layer,int value);
protected:
    bool eventFilter(QObject *target, QEvent *event)  {
        if(event->type() == QEvent::MouseButtonPress) {
            for(int i=0;i<goodsDisplay.size();i++){
                if(target==goodsDisplay.at(i)){
                    emit data(App::displayWidget,i);
                    return true;
                }
            }
        }
        return QWidget::eventFilter(target, event);
    }
};

#endif // GOODSTABLE_H
