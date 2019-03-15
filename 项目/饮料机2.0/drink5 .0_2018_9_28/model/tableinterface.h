#ifndef TABLEINTERFACE_H
#define TABLEINTERFACE_H

#include <QString>

class TableInterface
{
public:
    virtual bool table_create(const QString &tableName) = 0;
    //清空表
    virtual bool table_delete(const QString &tableName) = 0;
    //删除数据
    virtual bool table_delete_record(const QString &tableName,const int &id) = 0;
    virtual bool table_select_id(const QString &tableName,const QString &id) = 0;
};

#endif // TABLEINTERFACE_H
