#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariantList>
#include <QDebug>
#include <QSqlError>
#include <QVector>
#include <QCursor>
#include "util/filehandler.h"

class DataBaseHelper
{
private:
    QString dbName;
    QSqlDatabase db;
    //下面这些函数要设为private,防止出现 new 这样生成实例的方式
    DataBaseHelper();

    // 禁止拷贝
    DataBaseHelper(const DataBaseHelper&);
    DataBaseHelper& operator=(const DataBaseHelper&);

    // 析构函数也要设置为private
    ~DataBaseHelper(){
        if (DataBaseHelper::pDataBaseHelper != NULL){
            DataBaseHelper::pDataBaseHelper->database_close();
            delete DataBaseHelper::pDataBaseHelper;
            DataBaseHelper::pDataBaseHelper = NULL;
        }
    }

public:
    //饿汉式单例是指在方法调用前，实例就已经创建好了 线程安全
    //使用volatile关键字保其可见性
    //volatile static DataBaseHelper *pDataBaseHelper=new DataBaseHelper();

    //懒汉式单例是指在方法调用获取实例时才创建实例
    static DataBaseHelper *pDataBaseHelper;
    static pthread_mutex_t mutex;
    static DataBaseHelper *getInstence(){
        // Double Check Locking 双检查锁机制
        if(pDataBaseHelper == NULL){
            pthread_mutex_lock(&mutex);
            if(pDataBaseHelper == NULL){
                pDataBaseHelper=new DataBaseHelper();
            }
            pthread_mutex_unlock(&mutex);
        }
        return pDataBaseHelper;
    }

    //单例方法二:内部类
//    static class MySingletonHandler11 {
//        static DataBaseHelper *pDataBaseHelper=new DataBaseHelper();
//    };
    //单例方法三:使用static代码块实现单例
//    static{
//        pDataBaseHelper=new DataBaseHelper();
//    }

    //关闭数据库
    void database_close(){
//        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());


//        QString name;
//            {
//                name = QSqlDatabase::database().connectionName();
//            }//超出作用域，隐含对象QSqlDatabase::database()被删除。
//            QSqlDatabase::removeDatabase(name);




            db.close();
            db_open = false;


            QString name;
                {
                    name = QSqlDatabase::database().connectionName();
                }//超出作用域，隐含对象QSqlDatabase::database()被删除。
                QSqlDatabase::removeDatabase(name);
   db = QSqlDatabase::database();
   QSqlDatabase::removeDatabase("qt_sql_default_connection");//不推荐。

//        QSqlDatabase db = QSqlDatabase::database();//获得实例。
//        QString name = db.connectionName();//获得默认连接名。
//        QSqlDatabase::removeDatabase(name);//删除默认连接
    }
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

    QSqlQuery query;


    /**
      判断表中一个字段是否存在
     * @brief is_exist_field
     * @param tableName
     * @param field
     * @return
     */
    bool is_exist_field(const QString &tableName,QString field){
        QString strSql = QString::fromLocal8Bit("select * from sqlite_master where name='%1' and sql like '%%2%'").arg(tableName,field);
        QSqlQuery query(db);
        query.exec(strSql);
        qDebug()<<strSql;
        if (!query.next()){   // 无SuccessRate字段
          qDebug()<<"table not field";
          return false;
        }else{
          qDebug()<<"table field";
        }
        return true;
    }

    /**
      添加字段
     * @brief table_add_field_int
     * @param tableName
     * @param field
     * @return
     */
    bool table_add_field_int(const QString &tableName,QString field){
      QString str=QString("alter table %1 add %2 int(4) default \'0\'").arg(tableName,field);
      return query_exec(str);
    }

private:
    bool db_open;

    //创建数据库
    bool database_open(const QString &dbName){
        bool res = false;
        db_open = false;
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);
        db.setUserName("root");//        db.setPassword("123456");
        if (!db.open()) {
            qDebug() << "Database Error: "<<db.lastError();
            db_open = false;
        }else{
            qDebug() << "Database Create Sucessfully!";
            db_open = true;
            res = true;
        }
        query = QSqlQuery(dbName);
        return res;
    }

};

#endif // DATABASEHELPER_H


//      QString str=QString("select count(*) from information_schema.columns where table_name=\'%1\' and column_name=\'%2\'").arg(tableName,field);
//      qDebug()<<str;


//      QString str=QString("select sql from sqlite_master where type = 'table' and name = '%1'").arg(tableName);
//      qDebug()<<str;

//      if(query_exec(str)){
//          QSqlQuery query = DataBaseHelper::getInstence()->query;


  //  QCursor c ;//= db.rawQuery(queryStr, null);

//              String tableCreateSql = null;
//              try {
//                  if (c != null && c.moveToFirst()) {
//                      tableCreateSql = c.getString(c.getColumnIndex("sql"));
//                  }
//              } finally {
//                  if (c != null)
//                      c.close();
//              }



//          if(query.next()){
//              int level = query.value(0).toString();
//              printf("level=%d \n",level);
//              if(level != 0){
//                res = true;
//              }
//          }
//      }
//      return res;


//      QSqlQuery query(db);
//       QString selectsql;
//      selectsql ="select * from sys.tables where name='t'" ;//是否已经存在表car_bayonet_info
//      qDebug()<<selectsql;
//         query.exec(selectsql);
//          if(!query.next())
//          {
//              //SQLError = GBK("表car_bayonet_info不存在或不完整");
//              qDebug()<<"table not exist";
//              return false;
//          }
//          selectsql ="select * from syscolumns where id=object_id('t') and name='name'";//表car_bayonet_info是否已经存在Cross_ID字段
//           qDebug()<<selectsql;
//          query.exec(selectsql);
//          if(!query.next())
//          {
//              //SQLError = GBK("表car_bayonet_info不存在或不完整");
//              qDebug()<<"table field exist";
//              return false;
//          }
//          selectsql ="select * from syscolumns where id=object_id('t') and name='motor'";
//           qDebug()<<selectsql;
//          query.exec(selectsql);
//          if(!query.next())
//          {
//              //SQLError = GBK("表car_bayonet_info不存在或不完整");
//              qDebug()<<"table field exist";
//              return false;
//          }


// 判断表 CaseHistory 中是否有字段 SuccessRate
//QString strSql = QString::fromLocal8Bit("select * from sqlite_master where name='t' and sql like '%SuccessRate%'");
//QString strSql = QString::fromLocal8Bit("select * from sqlite_master where name='t' and sql like '%name%'");




