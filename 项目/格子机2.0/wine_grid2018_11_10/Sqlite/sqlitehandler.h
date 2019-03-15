#ifndef SQLITEHANDLER_H
#define SQLITEHANDLER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariantList>
#include <QDebug>
#include <QSqlError>
#include <QVector>
#include "Loading/jsonhandler.h"

class SqliteHandler
{
public:
    SqliteHandler();
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

    //饿汉式单例是指在方法调用前，实例就已经创建好了 线程安全
    //volatile static SqliteHandler *pSqliteHandler=new SqliteHandler();

    //懒汉式单例是指在方法调用获取实例时才创建实例
    //使用volatile关键字保其可见性
    static SqliteHandler *pSqliteHandler;
    static pthread_mutex_t mutex;
    static SqliteHandler *getInstance(){
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
private:
    QString dbName;
    QSqlDatabase db;
    QSqlQuery query;
    bool isOpenDb;
    //执行和操作的SQL语句
    bool query_exec(const QString &str){
        qDebug()<<str;
        query.prepare(str);
        if(!query.exec()){
            QSqlError error=query.lastError();// if(error.isValid())//发生错误时isValid()返回true
                switch (error.type()) {
                    case QSqlError::NoError:
                        qDebug()<<"无错误";
                    break;
                    case QSqlError::ConnectionError: //连接错语
                        qDebug()<<"ConnectionError";
                        qDebug()<<error.text();
                    break;
                    case QSqlError::StatementError:  //语句错语
                        qDebug()<<"StatementError";
                        qDebug()<<error.text();
                    break;
                    case QSqlError::TransactionError://事务错误
                        qDebug()<<"TransactionError";
                        qDebug()<<error.text();
                    break;
                    default://未知错误
                        qDebug()<<error.text();
                    break;
                }
            return false;
        }
        return true;
    }
    //创建数据库
    bool create_database(const QString &dbName){
        if(isOpenDb){
            return true;
        }
        db = QSqlDatabase::addDatabase("QSQLITE");//添加数据库驱动
        db.setDatabaseName(dbName);               //设置数据库名称
        db.setUserName("root");                   //数据库登录用户名
        //db.setPassword("123456");               //密码
        if(!db.open()) {
            qDebug() << "Database Error: "<<db.lastError();
            return false;
        }
        isOpenDb=true;
        query = QSqlQuery(dbName);
        return true;
    }
public:
    //创建表
    bool create_table(const QString &table){
        QString str=QString("CREATE TABLE IF NOT EXISTS ").append(table);
        return query_exec(str);
    }
    //插入数据
    bool table_insert(const QString &tableData){
        QString str=QString("INSERT INTO ").append(tableData);
        return query_exec(str);
    }
    //更新数据
    bool table_update(const QString &tableData){
        QString str=QString("update ").append(tableData);
        query.prepare(str);
        return query_exec(str);
    }

    bool is_record_exists(const QString &tableData){
        QString str=QString("select * from ").append(tableData);
        if(query_exec(str)&&query.next()){
            return true;
        }
        return false;
    }

public:
    //关闭数据库
    void database_close(){
        db.close();
    }
    //删除表
    bool delete_table(const QString &table){
        QString str=QString("DROP TABLE %1").append(table);
        return query_exec(str);
    }


    //清空表
    bool clear_table(const QString &tableName){
        QString str=QString("delete from ").append(tableName);
        return query_exec(str);
    }
    //删除数据
    bool table_delete(const QString &tableName,const QString &id){
        QString str=QString("delete from %1 where id=%2").arg(tableName,id);;
        return query_exec(str);
    }
    bool table_delete(const QString &tableData){
        QString str=QString("delete from ").append(&tableData);
        return query_exec(str);
    }
    //QString("delete from %1 where id>%2").arg(tableName,id);;

    bool is_ready_database(){
        QStringList dri=QSqlDatabase::drivers();
        qDebug()<<dri;
        if(!dri.contains("QSQLITE",Qt::CaseInsensitive)){
            qDebug()<<"SQLITE not have QSQLITE drivers";
            return false;
        }
        if(create_database(dbName)){
            qDebug() << "Database Create Sucessfully!";
            return true;
        }
        else{
            qDebug() << "Database Create Failed!";
        }
        return false;
    }

    QString get_table_data(const QString &tableName,QString id,const QString dataName){
       QString str=QString("select %1 from %2 where id=%3").arg(dataName,tableName,id);
       if(query_exec(str)){
           if(query.next()){
               return query.value(dataName).toString();
           }
       }
       return "";
    }
    bool get_table_data(const QString &tableName,QString id,
            QString &name,QString &price,QString &remain,QString &sales){
        QString str=QString("select name,price,remain,sales from %1 where id=%2").arg(tableName,id);
        if(query_exec(str)){
            if(query.next()){
                name =query.value("name").toString();
                price=query.value("price").toString();
                remain=query.value("remain").toString();
                sales=query.value("sales").toString();
                return true;
            }
        }
        return false;
    }
    bool get_table_data(const QString &tableName,QString id,QString &price,QString &remain){
        QString str=QString("select price,remain from %1 where id=%2").arg(tableName,id);
        if(query_exec(str)){
            if(query.next()){
                price=query.value("price").toString();
                remain=query.value("remain").toString();
                return true;
            }
        }
        return false;
    }
    QString get_table_data(const QString &tableName){
        QString json="";
        QString str=QString("select id,price,remain from %1").arg(tableName);
        if(query_exec(str)){
            QJsonArray array;
            while(query.next()){
                QString id ="box"+query.value("id").toString();
                QJsonObject obj;
                JsonHandler::json_object_add(obj,"id",id);
                JsonHandler::json_object_add(obj,"price",query.value("price").toString());
                JsonHandler::json_object_add(obj,"remain",query.value("remain").toString());
                array.append(obj);
            }
            json=JsonHandler::package_json_array(array);
        }
        return json;
    }
    bool get_table_data(const QString &tableName,QVector<int> &record,const int &len){
        bool res=true;
        for(int i=0;i<len;i++){
            QString str=QString("select remain from %1 where id=%2").arg(tableName,QString::number(i+1));
            if(query_exec(str)){
                if(query.next()){//first
                    QString remain =query.value("remain").toString();
                    record[i]=remain.toInt();
                }
            }
            else{
                res &=false;
            }
        }
        return res;
    }

    //查询数据
     bool table_select(const QString &tableName,QString id,QString &name,
                      QString &price,QString &state){
        QString str=QString("select name,price,state from %1 where id=%2").arg(tableName,id);
        if(!query_exec(str)){
            return false;
        }
        else{
            if(query.next()){
                name =query.value("name").toString();
                price=query.value("price").toString();
                state=query.value("state").toString();
                qDebug()<<name<<price<<state;
                return true;
            }
            else{
                return false;
            }
        }
    }

    bool foreign_keys(bool value){
        QString str;
        if(value){
            str="PRAGMA foreign_keys = ON";
        }
        else{
            str="PRAGMA foreign_keys = OFF;";
        }
        return query_exec(str);
    }
};

#endif // SQLITEHANDLER_H
