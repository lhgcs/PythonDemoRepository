#include "sqlitegoods.h"

SqliteGoods *SqliteGoods::pSqliteGoods = NULL;
pthread_mutex_t SqliteGoods::mutex;

SqliteGoods::SqliteGoods(){
    //SqliteHandler::getInstance()->is_ready_database();
    //SqliteHandler::getInstance()->foreign_keys(true);

    tableName="goodsInfo";
    if(create_table()) {
        qDebug()<<"goods_info"<<"Table Create Sucessfully!";
    }
    else{
        qDebug()<<"goods_info"<<"Table Create Failed!";
        SqliteHandler::getInstance()->delete_table(tableName);
        create_table();
    }
}
