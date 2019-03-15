#include "sqlitevideo.h"

SqliteVideo::SqliteVideo()
{
    sqliteHandler=new SqliteHandler;
    tableName="video_info";
   // sqliteHandler->is_ready_database();
    if(create_table()) {
        qDebug() << "Table Create Sucessfully!";
    }
    else{
        qDebug() << "Table Create Failed!";
    }
}

