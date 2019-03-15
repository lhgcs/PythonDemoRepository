#include "tableputouttimeinfo.h"

TablePutOutTimeInfo *TablePutOutTimeInfo::pTablePutOutTimeInfo = NULL;
pthread_mutex_t TablePutOutTimeInfo::mutex;

TablePutOutTimeInfo::TablePutOutTimeInfo()
{

}

void TablePutOutTimeInfo::setTableName(const QString &value)
{
    tableName = value;
}

QString TablePutOutTimeInfo::getTableName() const
{
    return tableName;
}

