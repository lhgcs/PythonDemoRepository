#ifndef SQLITEHANDLER_H
#define SQLITEHANDLER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariantList>
#include <QDebug>
#include <QSqlError>
#include <QVector>

#include "goodinfo.h"
#include "util/filehandler.h"

class SqliteHandler
{
private://下面这些函数要设为private
    SqliteHandler();//防止出现 new 这样生成实例的方式

    // 禁止拷贝
    SqliteHandler(const SqliteHandler&);
    SqliteHandler& operator=(const SqliteHandler&);

    // 析构函数也要设置为private
    ~SqliteHandler(){
        if (SqliteHandler::pSqliteHandler != NULL){
            SqliteHandler::pSqliteHandler->database_close();
            delete SqliteHandler::pSqliteHandler;
            SqliteHandler::pSqliteHandler = NULL;
        }
    }

public:
    //饿汉式单例是指在方法调用前，实例就已经创建好了 线程安全
    //volatile static SqliteHandler *pSqliteHandler=new SqliteHandler();

    //懒汉式单例是指在方法调用获取实例时才创建实例
    //使用volatile关键字保其可见性
    static SqliteHandler *pSqliteHandler;
    static pthread_mutex_t mutex;
    static SqliteHandler *getInstence(){
        // Double Check Locking 双检查锁机制
        if(pSqliteHandler == NULL){
            pthread_mutex_lock(&mutex);
            if(pSqliteHandler == NULL){
                pSqliteHandler=new SqliteHandler();
            }
            pthread_mutex_unlock(&mutex);
        }
        return pSqliteHandler;
    }

    //单例方法二:内部类
//    static class MySingletonHandler11 {
//        static SqliteHandler *pSqliteHandler=new SqliteHandler();
//    };

    //单例方法三:使用static代码块实现单例
//    static{
//        pSqliteHandler=new SqliteHandler();
//    }

    void start_transaction(){// 开始启动事务
        if(db.isOpen()){
            db.transaction();
        }
    }
    void start_commit(){     // 提交事务，这个时候才是真正打开文件执行SQL语句的时候
        if(db.isOpen()){
            db.commit();
        }
    }
    void add_query_exec(QString str){
       if (!query.exec(str)){
            qDebug()<<"Error occur"<<endl;
       }
    }


    //失败,不识别ON DUPLICATE
    //insert into t(name) values(q) ON DUPLICATE KEY UPDATE name=w;//当INSERT(因为主键或唯一键冲突)失败时，执行UPDATE

    //成功,可以插入或忽略
    //INSERT OR IGNORE INTO t VALUES (2,2,2,2,2,2,2);
    //UPDATE visits SET hits = hits + 1 WHERE ip LIKE $ip;

    //成功,没有就插入,有就更新
    //REPLACE INTO t VALUES (42,3,3,3,3,3,3);

    /* 失败,不存在
    IF EXISTS(SELECT * FROM t WHERE id = 200)
    BEGIN
    UPDATE t SET name = 4 WHERE id = 200
    END
    ELSE
    BEGIN
    INSERT INTO t (id, name) VALUES(200, 55)
    END
    */

    //成功
    //insert into student(name,age) select 'zz7zz7zz',25 where not exists(select * from student where name='zz7zz7zz' and age=25)

    //普通的批量插入

    //查询记录数目   select count(*) from t;

    //关闭数据库
    void database_close(){
        if (db_open == true){
            db.close();
            db_open = false;
        }
    }
    //创建数据库
    bool database_open(const QString &dbName){
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);
        db.setUserName("root");//        db.setPassword("123456");
        if (!db.open()) {
            qDebug() << "Database Error: "<<db.lastError();
            return false;
        }
        return true;
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
                            "url VARCHAR(100))").arg(tableName);//TIMESTAMP  DATETIME
        return query_exec(str);
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
    //插入数据
    bool table_insert(const QString &tableName,const GoodInfo &info){
        QString str=QString("INSERT INTO %1 (id,name,price,points,stock,time,url) "
                            "VALUES (\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\',\'%8\')")
                            .arg(tableName,
                                 QString::number(info.getId()),
                                 info.getName(),
                                 info.getPrice(),
                                 QString::number(info.getPoints()),
                                 QString::number(info.getStock()),
                                 info.getTimeStamp(),
                                 info.getImageUrl());
        return query_exec(str);
    }
    //更新数据
    bool table_update(const QString &tableName,const GoodInfo &info){
        QString str=QString("update %1 set name=\'%2\',price=\'%3\',points=\'%4\',stock=\'%5\',time=\'%6\',url=\'%7\' where id=%8")
                .arg(tableName,
                     info.getName(),
                     info.getPrice(),
                     QString::number(info.getPoints()),
                     QString::number(info.getStock()),
                     info.getTimeStamp(),
                     info.getImageUrl(),
                     QString::number(info.getId()));
        query.prepare(str);
        return query_exec(str);
    }
    bool table_update_stock(const QString &tableName,const int &id,const QString stock){
        QString str=QString("update %1 set stock=\'%2\' where id=%3")
                .arg(tableName,
                     stock,
                     QString::number(id));
        query.prepare(str);
        return query_exec(str);
    }
    //查询数据
    bool table_select_all(const QString &tableName,int id,GoodInfo &info){
        QString str=QString("select * from %1 where id=%2").arg(tableName,QString::number(id));
        if(query_exec(str)){
            query.size();
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
        QString str=QString("select price,stock from %1 where id=%2").arg(tableName,QString::number(id));
        if(query_exec(str)){
            query.size();
            if(query.next()){
                info.setPrice(query.value("price").toString());
                info.setStock(query.value("stock").toInt());
                return true;
            }
        }
        return false;
    }
    int table_select_stock(const QString &tableName,int id){
        QString str=QString("select stock from %1 where id=%2").arg(tableName,QString::number(id));
        if(query_exec(str)){
            query.size();
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

private:
    QString dbName;
    QSqlQuery query;
    QSqlDatabase db;
    bool db_open;

    //执行和操作的SQL语句
    bool query_exec(const QString &str){
        //qDebug()<<str;
        query.prepare(str);
        if(!query.exec()){
            qDebug()<<query.lastError();
            return false;
        }
        return true;
    }

    bool table_select_id(const QString &tableName,const QString &id){
        QString str=QString("select id from %1 where id=%2").arg(tableName,id);
        if(query_exec(str)&&query.next()){//若这个id已经在数据库存在
            return true;//查询已存在子集
        }
        return false;
    }

public:
    bool database_and_table_init(const QString &dbName,const QString &tableName){
        bool res=false;
        QStringList dri=QSqlDatabase::drivers();
        qDebug()<<dri;
        if(dri.contains("QSQLITE",Qt::CaseInsensitive)){
            db_open = false;
            if(database_open(dbName)){
                db_open = true;
                query = QSqlQuery(dbName);
                qDebug() << "Database Create Sucessfully!";
                if(table_create(tableName)) {
                    qDebug() << "Table Create Sucessfully!";
                    res=true;
                }else{
                    qDebug() << "Table Create Failed!";
                }
            } else{
                query = QSqlQuery(dbName);
                qDebug() << "Database Create Failed!";
            }

            if(res == false){
                database_close();
                clear_dir_files("/home/sql");
                db_open = false;
                if(database_open(dbName)){
                    db_open = true;
                    query = QSqlQuery(dbName);
                    qDebug() << "Database Create Sucessfully!";
                    if(table_create(tableName)) {
                        qDebug() << "Table Create Sucessfully!";
                        res=true;
                    }else{
                        qDebug() << "Table Create Failed!";
                    }
                }else{
                    query = QSqlQuery(dbName);
                    qDebug() << "Database Create Failed!";
                }
            }


        }else{
            qDebug()<<"SQLITE not have QSQLITE drivers";
            return false;
        }
        return res;
    }

    bool table_insert_or_update(const QString &tableName,const GoodInfo &info){
        QString idStr=QString::number(info.getId());
        if(table_select_id(tableName,idStr)){
            table_update(tableName,info);
        }else{
            table_insert(tableName,info);//如果插入相同的id出错
        }
        return true;
    }
    //这中语法会自动查询主键或索引冲突，如有冲突，他会先删除原有的数据记录，然后执行插入新的数据。
    bool table_replace(const QString &tableName,const GoodInfo &info){
        QString str=QString("REPLACE INTO %1 VALUES ('%2','%3','%4','%5','%6','%7','%8');")
                .arg(tableName,
                     QString::number(info.getId()),
                     info.getName(),
                     info.getPrice(),
                     QString::number(info.getPoints()),
                     QString::number(info.getStock()),
                     info.getTimeStamp(),
                     info.getImageUrl());
        query.prepare(str);
        return query_exec(str);
    }
    //在insert时判断是否已有主键或索引重复，如果有，一句update后面的表达式执行更新，否则，执行插入。
    //更新操作虽然也会更新数据，但其对主键的索引却不会有改变，也就是说，insert　on duplicate　更新对主键索引没有影响.
    //INSERT INTO table (a,b,c) VALUES (1,2,3) ON DUPLICATE KEY UPDATE c=c+1;
//INSERT INTO t (id,name) VALUES (100,2222) ON DUPLICATE KEY UPDATE id=1,name=333;
};

#endif // SQLITEHANDLER_H
