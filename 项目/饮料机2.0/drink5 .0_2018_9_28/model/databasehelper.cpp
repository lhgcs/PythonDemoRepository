#include "databasehelper.h"
#include "configure/app.h"
#include "util/filehandler.h"
#include "tablegoodinfo.h"
#include "tableputouttimeinfo.h"

DataBaseHelper *DataBaseHelper::pDataBaseHelper = NULL;
pthread_mutex_t DataBaseHelper::mutex;

DataBaseHelper::DataBaseHelper(){
    dbName=App::dataBaseName;
    bool res=false;
    int i = 0;
    do{
        db_open = false;

        res =database_open(dbName);
        if(res == false){
            database_close();
            clear_dir_files("/home/sql");
        }

        TableGoodInfo::getInstence()->setTableName(App::goodsTableName);
        TablePutOutTimeInfo::getInstence()->setTableName("putout_time_info");

        if(res == true){
            bool res1=TableGoodInfo::getInstence()->table_create(TableGoodInfo::getInstence()->getTableName(),query);
            bool res2=TablePutOutTimeInfo::getInstence()->table_create(TablePutOutTimeInfo::getInstence()->getTableName(),query);

            if(res1 == false || res2 == false){
                database_close();
                clear_dir_files("/home/sql");
                res = false;
            }else{
                if(is_exist_field(TableGoodInfo::getInstence()->getTableName(),"motor")){
                    return;
                }
                else{
                    database_close();
                    clear_dir_files("/home/sql");
                    res = false;
                }
            }
        }
    }while (res == false && i++ <3);

}
