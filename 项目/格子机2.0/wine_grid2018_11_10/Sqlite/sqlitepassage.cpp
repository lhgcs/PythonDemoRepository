#include "sqlitepassage.h"

SqlitePassage::SqlitePassage()
{
    sqliteHandler=new SqliteHandler;
    tableName="passage_info";
    if(create_table()) {
        qDebug() << "Table Create Sucessfully!";
    }
    else{
        qDebug() << "Table Create Failed!";
    }
    QString id="1";
    table_insert_or_update(id,id,id);
}

