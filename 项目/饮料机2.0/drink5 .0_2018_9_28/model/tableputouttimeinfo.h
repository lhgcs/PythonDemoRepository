#ifndef TABLEPUTOUTTIMEINFO_H
#define TABLEPUTOUTTIMEINFO_H

#include "databasehelper.h"

class TablePutOutTimeInfo
{
public:
    TablePutOutTimeInfo();
    static TablePutOutTimeInfo *pTablePutOutTimeInfo;
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

    static TablePutOutTimeInfo *getInstence(){
        // Double Check Locking 双检查锁机制
        if(pTablePutOutTimeInfo == NULL){
            pthread_mutex_lock(&mutex);
            if(pTablePutOutTimeInfo == NULL){
                pTablePutOutTimeInfo=new TablePutOutTimeInfo();
            }
            pthread_mutex_unlock(&mutex);
        }
        return pTablePutOutTimeInfo;
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
                            "myid INTEGER, "          //BIGINT
                            "time INTEGER DEFAULT(0))"           //CHAR
                           ).arg(tableName);//TIMESTAMP  DATETIME
        return query_exec(str);
    }

    bool table_create(const QString &tableName,QSqlQuery &query){
        QString str=QString("CREATE TABLE IF NOT EXISTS %1 ("
                             //AUTOINCREMENT
                            "myid INTEGER, "          //BIGINT
                            "time INTEGER DEFAULT(0))"           //CHAR
                           ).arg(tableName);//TIMESTAMP  DATETIME
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
    bool table_select_id(const QString &tableName,const int &id){
        QString str=QString("select myid from %1 where myid=%2").arg(tableName,QString::number(id));
        if(query_exec(str)&&DataBaseHelper::getInstence()->query.next()){//若这个id已经在数据库存在
            return true;//查询已存在子集
        }
        return false;
    }

    //插入数据
    bool table_insert(const QString &tableName,const int &myid,const int &time){
        QString str=QString("INSERT INTO %1 (myid,time) "
                            "VALUES (\'%2\',\'%3\')")
                            .arg(tableName,
                                 QString::number(myid),
                                 QString::number(time)
                                 );
        return query_exec(str);
    }
    //更新数据
    bool table_update(const QString &tableName,const int &myid,const int &time){
        QString str=QString("update %1 set time=\'%3\' where myid=%3")
                .arg(tableName,
                     QString::number(time),
                     QString::number(myid)
                     );
        //query.prepare(str);
        return query_exec(str);
    }

    //这中语法会自动查询主键或索引冲突，如有冲突，他会先删除原有的数据记录，然后执行插入新的数据。
    bool table_replace(const QString &tableName,const int &myid,const int &time){
        QString str=QString("REPLACE INTO %1 VALUES ('%2','%3');")
                .arg(tableName,
                     QString::number(myid),
                     QString::number(time));
        return query_exec(str);
    }

    int table_select_time(const QString &tableName,int id){
        QString str=QString("select time from %1 where myid=%2").arg(tableName,QString::number(id));
        if(query_exec(str)){
            QSqlQuery query = DataBaseHelper::getInstence()->query;
            if(query.next()){
                return query.value("time").toInt();
            }
        }
        return 0;
    }

    void setTableName(const QString &value);

    QString getTableName() const;

private:
    QString tableName;

    bool table_insert_or_update(const QString &tableName,const int &myid,const int &time){
        if(table_select_id(tableName,myid)){
            table_update(tableName,myid,time);
        }else{
            table_insert(tableName,myid,time);//如果插入相同的id出错
        }
        return true;
    }

    //在insert时判断是否已有主键或索引重复，如果有，一句update后面的表达式执行更新，否则，执行插入。
    //更新操作虽然也会更新数据，但其对主键的索引却不会有改变，也就是说，insert　on duplicate　更新对主键索引没有影响.
    //INSERT INTO table (a,b,c) VALUES (1,2,3) ON DUPLICATE KEY UPDATE c=c+1;
    //INSERT INTO t (id,name) VALUES (100,2222) ON DUPLICATE KEY UPDATE id=1,name=333;
};

#endif // TABLEPUTOUTTIMEINFO_H
