#ifndef GOODSINFO_H
#define GOODSINFO_H

#include <QString>
#include "app.h"

class GoodsInfo
{
public:
    GoodsInfo();
    int id;
    QString name;
    QString price;
    QString remain;
    QString sales;

//    void set_sqliteGoods(SqliteGoods *_sqliteGoods){
//        sqliteGoods=_sqliteGoods;
//    }

//    void updata_info(int _id){
//        id=_id;
//        sqliteGoods->get_table_data(QString::number(_id+1),name,price,remain,sales);
//    }
};

#endif // GOODSINFO_H
