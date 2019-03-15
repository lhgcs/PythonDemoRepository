#include "tablegoodinfo.h"
#include "util/filehandler.h"

TableGoodInfo *TableGoodInfo::pTableGoodInfo = NULL;
pthread_mutex_t TableGoodInfo::mutex;

TableGoodInfo::TableGoodInfo()
{

}

void TableGoodInfo::setTableName(const QString &value)
{
    tableName = value;
}

QString TableGoodInfo::getTableName() const
{
    return tableName;
}

