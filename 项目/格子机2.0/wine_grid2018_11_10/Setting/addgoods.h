#ifndef ADDGOODS_H
#define ADDGOODS_H

#include <QWidget>
#include "tableview.h"
#include <QPushButton>
#include <QVBoxLayout>

#include "Sqlite/sqlitegoods.h"
#include "httprequest.h"
#include "toast.h"

//extern HttpRequest *httpRequest;

class AddGoods : public QWidget
{
    Q_OBJECT
public:
    explicit AddGoods(QWidget *parent = 0);
    TableView *tableView;
    SqliteGoods *sqliteGoods;
    HttpRequest *httpRequest;/////////////////////////

private:
    QPushButton *btnUpLoad;
    QVBoxLayout *vl;
private slots:
    void btnUpLoad_slots(){
        if(sqliteGoods){
            QString json=sqliteGoods->get_price_remain();
            httpRequest->send_stauts(json);
            Toast::getInstance()->set_text("正在上传，请耐心等待");
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

#endif // ADDGOODS_H
