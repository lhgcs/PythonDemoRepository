#ifndef TABLEGOODINFO_H
#define TABLEGOODINFO_H

#include "tableinterface.h"
#include "databasehelper.h"
#include "goodinfo.h"

class TableGoodInfo : public TableInterface
{
private:
    TableGoodInfo();
    static TableGoodInfo *pTableGoodInfo;
    static pthread_mutex_t mutex;

    bool query_exec(const QString &str){
        return DataBaseHelper::getInstence()->query_exec(str);
    }
    bool query_exec_record_size(const QString &str){
        bool res=DataBaseHelper::getInstence()->query_exec(str);
        DataBaseHelper::getInstence()->query.size();
        return res;
    }

public:

    static TableGoodInfo *getInstence(){
        // Double Check Locking 双检查锁机制
        if(pTableGoodInfo == NULL){
            pthread_mutex_lock(&mutex);
            if(pTableGoodInfo == NULL){
                pTableGoodInfo=new TableGoodInfo();
            }
            pthread_mutex_unlock(&mutex);
        }
        return pTableGoodInfo;
    }

    void start_transaction(){// 开始启动事务
        DataBaseHelper::getInstence()->start_transaction();
    }
    void start_commit(){     // 提交事务，这个时候才是真正打开文件执行SQL语句的时候
        DataBaseHelper::getInstence()->start_commit();
    }

    //创建表
    bool table_create(const QString &tableName){
        QString str=QString("CREATE TABLE IF NOT EXISTS %1 ("
                            "id INTEGER PRIMARY KEY, "  //AUTOINCREMENT
                            "name VARCHAR(10), "        //TEXT
                            "price VARCHAR(8), "        //FLOAT
                            "points INTEGER, "          //BIGINT
                            "stock INTEGER, "           //CHAR
                            "time VARCHAR(32),"
                            "url VARCHAR(100),"
                            "motor INTEGER)").arg(tableName);//TIMESTAMP  DATETIME
        return query_exec(str);
    }
    bool table_create(const QString &tableName,QSqlQuery &query){
        QString str=QString("CREATE TABLE IF NOT EXISTS %1 ("
                            "id INTEGER PRIMARY KEY, "  //AUTOINCREMENT
                            "name VARCHAR(10), "        //TEXT
                            "price VARCHAR(8), "        //FLOAT
                            "points INTEGER, "          //BIGINT
                            "stock INTEGER, "           //CHAR
                            "time VARCHAR(32),"
                            "url VARCHAR(100),"
                            "motor INTEGER)").arg(tableName);//TIMESTAMP  DATETIME
        query.prepare(str);
        return query.exec();
    }

    //清空表
    bool table_delete(const QString &tableName){
        QString str=QString("drop table %1").arg(tableName);//"delete t from %1"
        return query_exec(str);
    }
    //删除数据
    bool table_delete_record(const QString &tableName,const int &id){
        QString str=QString("delete from %1 where id=%2").arg(tableName,QString::number(id));;
        return query_exec(str);
    }
    bool table_select_id(const QString &tableName,const QString &id){
        QString str=QString("select id from %1 where id=%2").arg(tableName,id);
        if(query_exec(str)&&DataBaseHelper::getInstence()->query.next()){//若这个id已经在数据库存在
            return true;//查询已存在子集
        }
        return false;
    }

    int table_select_motor(const QString &tableName,const int &id){
        QString str=QString("select motor from %1 where id=%2").arg(tableName,QString::number(id));
        qDebug()<<str;
        if(query_exec(str)){
            QSqlQuery query = DataBaseHelper::getInstence()->query;
            if(query.next()){
                return query.value("motor").toInt();
            }
        }
        return -1;
    }

    //插入数据  插入相同ID 失败
    bool table_insert(const QString &tableName,const GoodInfo &info){
        QString str=QString("INSERT INTO %1 (id,name,price,points,stock,time,url,motor) "
                            "VALUES (\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\',\'%8\',\'%9\')")
                            .arg(tableName,
                                 QString::number(info.getId()),
                                 info.getName(),
                                 info.getPrice(),
                                 QString::number(info.getPoints()),
                                 QString::number(info.getStock()),
                                 info.getTimeStamp(),
                                 info.getImageUrl(),
                                 QString::number(info.getMotor())
                                 );
        return query_exec(str);
    }
    //更新数据
    bool table_update(const QString &tableName,const GoodInfo &info){
        QString str=QString("update %1 set name=\'%2\',price=\'%3\',points=\'%4\',stock=\'%5\',time=\'%6\',url=\'%7\',motor=\'%8\'where id=%9")
                .arg(tableName,
                     info.getName(),
                     info.getPrice(),
                     QString::number(info.getPoints()),
                     QString::number(info.getStock()),
                     info.getTimeStamp(),
                     info.getImageUrl(),
                     QString::number(info.getMotor()),
                     QString::number(info.getId()));
        //query.prepare(str);
        return query_exec(str);
    }

    //这中语法会自动查询主键或索引冲突，如有冲突，他会先删除原有的数据记录，然后执行插入新的数据。
    bool table_replace(const QString &tableName,const GoodInfo &info){
        QString str=QString("REPLACE INTO %1 VALUES ('%2','%3','%4','%5','%6','%7','%8','%9');")
                .arg(tableName,
                     QString::number(info.getId()),
                     info.getName(),
                     info.getPrice(),
                     QString::number(info.getPoints()),
                     QString::number(info.getStock()),
                     info.getTimeStamp(),
                     info.getImageUrl(),
                     QString::number(info.getMotor())
                     );
        return query_exec(str);
    }

    bool table_update_stock(const QString &tableName,const int &id,const QString stock){
        QString str=QString("update %1 set stock=\'%2\' where id=%3")
                .arg(tableName,
                     stock,
                     QString::number(id));
        //query.prepare(str);
        return query_exec(str);
    }
    bool table_update_time(const QString &tableName,const int &id,const QString time){
        QString str=QString("update %1 set time=\'%2\' where id=%3")
                .arg(tableName,
                     time,
                     QString::number(id));
        //query.prepare(str);
        return query_exec(str);
    }
    //查询数据
    bool table_select_all(const QString &tableName,int id,GoodInfo &info){
        QString str=QString("select * from %1 where id=%2").arg(tableName,QString::number(id));
        if(query_exec(str)){
            QSqlQuery query = DataBaseHelper::getInstence()->query;
            if(query.next()){
                info.setId(query.value("id").toInt());
                info.setName (query.value("name").toString());
                info.setPrice(query.value("price").toString());
                info.setPoints(query.value("points").toInt());
                info.setStock(query.value("stock").toInt());
                info.setTimeStamp(query.value("time").toString());
                info.setImageUrl(query.value("url").toString());
                return true;
            }
        }
        return false;
    }
    bool table_select_price_stock(const QString &tableName,int id,GoodInfo &info){
        QString str=QString("select name,price,stock from %1 where id=%2").arg(tableName,QString::number(id));
        if(query_exec(str)){
            QSqlQuery query = DataBaseHelper::getInstence()->query;
            if(query.next()){
                info.setPrice(query.value("price").toString());
                info.setStock(query.value("stock").toInt());
                info.setName(query.value("name").toString());
                return true;
            }
        }
        return false;
    }
    int table_select_stock(const QString &tableName,int id){
        QString str=QString("select stock from %1 where id=%2").arg(tableName,QString::number(id));
        if(query_exec(str)){
            QSqlQuery query = DataBaseHelper::getInstence()->query;
            if(query.next()){
                return query.value("stock").toInt();
            }
        }
        return 0;
    }
    //数量减1
    bool table_number_reduce(const QString &tableName,const int &id){
        int value=table_select_stock(tableName,id);
        qDebug()<<"id:"<<id<<"int:"<<value;
        value=value>=1?(value-1):0;
        return table_update_stock(tableName,id,QString::number(value));
    }
    //清空字段
    bool table_field_clear(const QString &tableName,const int &id,const QString &field){
        QString idStr=QString::number(id);
        QString str=QString("update %1 set %2=\'0\' where id=%3").arg(tableName,field,idStr);
        return query_exec(str);
    }

    void setTableName(const QString &value);
    QString getTableName() const;

private:
    QString tableName;

    bool table_insert_or_update(const QString &tableName,const GoodInfo &info){
        QString idStr=QString::number(info.getId());
        if(table_select_id(tableName,idStr)){
            table_update(tableName,info);
        }else{
            table_insert(tableName,info);//如果插入相同的id出错
        }
        return true;
    }

    //在insert时判断是否已有主键或索引重复，如果有，一句update后面的表达式执行更新，否则，执行插入。
    //更新操作虽然也会更新数据，但其对主键的索引却不会有改变，也就是说，insert　on duplicate　更新对主键索引没有影响.
    //INSERT INTO table (a,b,c) VALUES (1,2,3) ON DUPLICATE KEY UPDATE c=c+1;
    //INSERT INTO t (id,name) VALUES (100,2222) ON DUPLICATE KEY UPDATE id=1,name=333;
};

#endif // TABLEGOODINFO_H
