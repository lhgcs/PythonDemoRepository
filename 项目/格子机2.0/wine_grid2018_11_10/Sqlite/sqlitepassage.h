#ifndef SQLITEPASSAGE_H
#define SQLITEPASSAGE_H

#include "sqlitehandler.h"

class SqlitePassage
{
public:
    SqlitePassage();
    ~SqlitePassage(){
        sqliteHandler->close_database();
        delete sqliteHandler;
    }

private:
    QString tableName;
    SqliteHandler *sqliteHandler;
    //创建表
    bool create_table(){
        QString str=QString(" %1 (id INTEGER PRIMARY KEY, "
                            "num VARCHAR(10), "
                            "goods_id INTEGER, "
                            "FOREIGN KEY (goods_id) REFERENCES goods_info(id))").arg(tableName);
        return sqliteHandler->create_table(str);
    }
    //插入数据
    bool table_insert(const QString &id,const QString &num,const QString &goods_id){
        QString str=QString(" %1 (id,num,goods_id) "
                            "VALUES (\'%2\',\'%3\',\'%4\')")
                            .arg(tableName,id,num,goods_id);
        return sqliteHandler->table_insert(str);
    }
    //更新数据
    bool table_update(const QString &id,const QString &num,const QString &goods_id){
        QString str=QString(" %1 set num=\'%2\',goods_id=\'%3\' where id=%4")
                            .arg(tableName,num,goods_id,id);
        return sqliteHandler->table_update(str);
    }

    bool is_record_exists(const QString &id){
        QString str=QString(" %1 where id=%2").arg(tableName,id);
        return sqliteHandler->is_record_exists(str);
    }

public:
    //清空表
    bool clear_table(){
        return sqliteHandler->create_table(tableName);
    }
    //删除数据
    bool table_delete(const QString &id){
        return sqliteHandler->table_delete(tableName,id);
    }

    bool table_insert_or_update(const QString &id,const QString &num,const QString &goods_id){
        if(is_record_exists(id)){
           return table_update(id,num,goods_id);
        }
        else{
            return table_insert(id,num,goods_id);
        }
        return true;
    }

    QString get_table_num_data(const QString &id){
        return sqliteHandler->get_table_data(this->tableName,id,"num");
    }
    QString get_table_goods_id_data(const QString &id){
        return sqliteHandler->get_table_data(this->tableName,id,"goods_id");
    }

//    bool table_select(QString id,QString &name,QString &price,QString &state){
//       QString str=QString("select name,price,state from %1 where id=%2").arg(tableName,id);
//       return sqliteHandler->table_select(this->tableName,id,name,price,state);
//   }
};

#endif // SQLITEPASSAGE_H
