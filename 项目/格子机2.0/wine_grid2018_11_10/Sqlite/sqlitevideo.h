#ifndef SQLITEVIDEO_H
#define SQLITEVIDEO_H

#include "sqlitehandler.h"

class SqliteVideo
{
public:
    SqliteVideo();
    ~SqliteVideo(){
        delete sqliteHandler;
    }

private:
    QString tableName;
    SqliteHandler *sqliteHandler;
    //创建表
    bool create_table(){
        QString str=QString(" %1 (id INTEGER PRIMARY KEY, "  //AUTOINCREMENT
                            "name VARCHAR(10), "             //TEXT
                            "time VARCHAR(32),"
                            "url VARCHAR(100))").arg(tableName);//TIMESTAMP  DATETIME
        return sqliteHandler->create_table(str);
    }
    //插入数据
    bool table_insert(const QString &id,const QString &name,const QString &time,const QString &url){
        QString str=QString(" %1 (id,name,time,url) "
                            "VALUES (\'%2\',\'%3\',\'%4\',\'%5\')")
                            .arg(tableName,id,name,time,url);
        return sqliteHandler->table_insert(str);
    }
    //更新数据
    bool table_update(const QString &id,const QString &name,const QString &time,const QString &url){
        QString str=QString(" %1 set name=\'%2\',time=\'%3\',url=\'%4\' where id=%5")
                            .arg(tableName,name,time,url,id);
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
    bool table_insert_or_update(const QString &id,const QString &name,const QString &time,const QString &url){
        if(is_record_exists(id)){
           return table_update(id,name,time,url);
        }
        return table_insert(id,name,time,url);
    }
    QString get_table_time_data(const QString &id){
        return sqliteHandler->get_table_data(this->tableName,id,"time");
    }
    QString get_table_url_data(const QString &id){
        return sqliteHandler->get_table_data(this->tableName,id,"url");
    }
};

#endif // SQLITEVIDEO_H
