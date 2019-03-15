#include "sqlitehandler.h"

#include "configure/app.h"

SqliteHandler *SqliteHandler::pSqliteHandler = NULL;
pthread_mutex_t SqliteHandler::mutex;

SqliteHandler::SqliteHandler()
{
    db_open = false;
    dbName=App::dataBaseName;
    database_and_table_init(dbName,App::goodsTableName);
}
