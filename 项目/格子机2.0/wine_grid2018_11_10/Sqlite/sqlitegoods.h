#ifndef SQLITEGOODS_H
#define SQLITEGOODS_H

#include "sqlitehandler.h"

class SqliteGoods
{
public:
    SqliteGoods();

    // 析构函数也要设置为private
    ~SqliteGoods(){
        if (SqliteGoods::pSqliteGoods != NULL){
            delete SqliteGoods::pSqliteGoods;
            SqliteGoods::pSqliteGoods = NULL;
        }
    }

    static SqliteGoods *pSqliteGoods;
    static pthread_mutex_t mutex;
    static SqliteGoods *getInstance(){
        // Double Check Locking 双检查锁机制
        if(pSqliteGoods == NULL){
            pthread_mutex_lock(&mutex);
            if(pSqliteGoods == NULL){
                pSqliteGoods=new SqliteGoods();
            }
            pthread_mutex_unlock(&mutex);
        }
        return pSqliteGoods;
    }

private:
    QString tableName;

    //创建表
    bool create_table(){
        QString str=QString(" %1 (id INTEGER PRIMARY KEY, "  //AUTOINCREMENT
                            "name VARCHAR(10), "             //TEXT
                            "price VARCHAR(8), "             //FLOAT
                            "remain VARCHAR(8), "
                            "sales VARCHAR(8), "
                            "time VARCHAR(32),"
                            "url VARCHAR(100))").arg(tableName);//TIMESTAMP  DATETIME
        return SqliteHandler::getInstance()->create_table(str);
    }
    //插入数据
    bool table_insert(const QString &id,const QString &name,const QString &price,const QString &remain,const QString &time,const QString &url){
        QString str=QString(" %1 (id,name,price,remain,time,url) "
                            "VALUES (\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\')")
                            .arg(tableName,id,name,price,remain,time,url);
        return SqliteHandler::getInstance()->table_insert(str);
    }
    //更新数据
    bool table_update(const QString &id,const QString &name,const QString &price,const QString &remain,//const QString &sales,  sales=\'%5\',
                      const QString &time,const QString &url){
        QString str=QString(" %1 set name=\'%2\',price=\'%3\',remain=\'%4\',time=\'%5\',url=\'%6\' where id=%7")
                            .arg(tableName,name,price,remain,time,url,id);
        return SqliteHandler::getInstance()->table_update(str);
    }

    bool is_record_exists(const QString &id){
        QString str=QString(" %1 where id=%2").arg(tableName,id);
        return SqliteHandler::getInstance()->is_record_exists(str);
    }

public:
    //清空表
    bool clear_table(){
        return SqliteHandler::getInstance()->create_table(tableName);
    }
    //删除数据
    bool table_delete(const QString &id){
        return SqliteHandler::getInstance()->table_delete(tableName,id);
    }

    bool table_insert_or_update(const QString &id,const QString &name,const QString &price,const QString &remain,//const QString &sales,
                                const QString &time,const QString &url){
        if(is_record_exists(id)){
           return table_update(id,name,price,remain,time,url);
        }
        else{
            table_insert(id,name,price,remain,time,url);
        }
        return true;
    }

    bool get_table_data(QString id,QString &name,QString &price,QString &remain,QString &sales){
        SqliteHandler::getInstance()->get_table_data(this->tableName,id,name,price,remain,sales);
        return true;
    }

    QString get_table_time_data(const QString &id){
        return SqliteHandler::getInstance()->get_table_data(this->tableName,id,"time");
    }
    QString get_table_url_data(const QString &id){
        return SqliteHandler::getInstance()->get_table_data(this->tableName,id,"url");
    }
    float get_table_price_data(const QString &id){
        QString value=SqliteHandler::getInstance()->get_table_data(this->tableName,id,"price");
        if(!value.isEmpty()){
            return value.toFloat();
        }
        return 0;
    }
    int get_table_remain_data(const QString &id){
        QString value=SqliteHandler::getInstance()->get_table_data(this->tableName,id,"remain");
        if(!value.isEmpty()){
            return value.toInt();
        }
        return 0;
    }
    bool get_table_remain_data(QVector<int> &record,const int &len){
        return SqliteHandler::getInstance()->get_table_data(this->tableName,record,len);
    }

    int get_table_total_value_data(int id){
        QString price;
        QString remain;
        if(SqliteHandler::getInstance()->get_table_data(this->tableName,QString::number(id),price,remain)){
            int total=price.toFloat()*remain.toInt()/500;
            return total;
        }
        return 0;
    }

//    bool table_select(QString id,QString &name,QString &price,QString &state){
//       QString str=QString("select name,price,state from %1 where id=%2").arg(tableName,id);
//       return SqliteHandler::getInstance()->table_select(this->tableName,id,name,price,state);
//   }


    QString get_price_remain(){
        return SqliteHandler::getInstance()->get_table_data(this->tableName);
    }

    bool table_update(const QString &id,const QString &remain){
        QString str=QString(" %1 set remain=\'%2\' where id=%3").arg(tableName,remain,id);
        return SqliteHandler::getInstance()->table_update(str);
    }
};

#endif // SQLITEGOODS_H
