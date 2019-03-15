#include "sqlitehandler.h"
#include "filehandler.h"

SqliteHandler *SqliteHandler::pSqliteHandler = NULL;
pthread_mutex_t SqliteHandler::mutex;

SqliteHandler::SqliteHandler()
{   
    this->dbName="/home/sql/database.db";
    create_Dir("/home/sql");
    isOpenDb=false;
    create_database(dbName);
}
