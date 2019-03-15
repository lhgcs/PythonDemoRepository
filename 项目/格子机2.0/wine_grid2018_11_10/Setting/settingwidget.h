#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QDebug>
#include "QPainter"
#include "Setting/tableview.h"
#include "addgoods.h"

#include "Sqlite/sqlitegoods.h"

#include <QApplication>

class SettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingWidget(SqliteGoods *_sqliteGoods, HttpRequest *_httpRequest, QWidget *parent = 0);
    ~SettingWidget(){
        delete listWidget;
        delete stackedWidget;
        delete hl;
    }
    AddGoods *addGoods;

private:
    QListWidget *listWidget;
    QStackedWidget *stackedWidget;
    QHBoxLayout *hl;
private slots:
    void click_slot(int currentRow){
        if((currentRow+1)==listWidget->count()){
            this->close();
        }
        else if((currentRow+1)==listWidget->count()-1){
            //QApplication::exit(773);
            system("reboot");
        }
    }
protected:
    void paintEvent(QPaintEvent *){
        QStyleOption  opt;
        opt.init(this);
        QPainter  p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // SETTINGWIDGET_H
