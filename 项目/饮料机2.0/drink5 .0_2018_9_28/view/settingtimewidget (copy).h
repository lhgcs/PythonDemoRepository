#ifndef SETTINGTIMEWIDGET_H
#define SETTINGTIMEWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QPushButton>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "configure/app.h"
#include "model/databasehelper.h"
#include "model/mysqltablemodel.h"

class SettingTimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingTimeWidget(QWidget *parent = 0);
    int get_record_num(){
        return model->rowCount();
    }
private:
    void get_database_data();
    void model_remove();
    void model_insert();

    QTableView *tableView;
    MySqlTableModel *model;
    QPushButton *btnInsert,*btnDelete,*btnAdd,*btnClear;
    QHBoxLayout *hl;
    QVBoxLayout *vl;

    //QSqlDatabase db;
//    bool link_database(){//连接数据库
//      this->db=QSqlDatabase::addDatabase("QSQLITE");    //加载数据库驱动
//      this->db.setHostName("127.0.0.1");                //本地连接
//      this->db.setUserName("root");                     //root用户
//      //this->db.setPassword(""+passResult+"");         //mysql密码
//      this->db.setDatabaseName(App::dataBaseName);//需要连接的数据库
//      if(!db.open())  {//打开
//          return false;
//      }
//      return true;
//    }

    void model_submit()   {            //提交修改
      model->database().transaction(); //开始事务操作
      if(model->submitAll()) {
          model->database().commit();  //提交
      }else {
          model->database().rollback(); //回滚
          //QMessageBox::warning(this, tr("tableModel"),tr("数据库错误: %1").arg(model->lastError().text()));
      }
    }
    void model_revert()  {          //撤销修改
      model->revertAll();
    }
    void model_all()     {               //返回全表
      model->setTable("student");        //重新关联表
      model->select();                   //这样才能再次显示整个表的内容
    }
    void model_sort_up() {               //升序
      model->setSort(0,Qt::AscendingOrder);//id属性，即第0列，升序排列
      model->select();
    }
    void model_sort_down(){              //降序
      model->setSort(0,Qt::DescendingOrder);
      model->select();
    }
    void model_selete() {                //查询
      QString name = "11";
      model->setFilter(QObject::tr("name = '%1'").arg(name)); //根据姓名进行筛选 使用setFilter()函数进行关键字筛选
      model->select(); //显示结果
    }

private slots:
    void btnInsert_slot(){
        model_insert();
        model_sort_up();
    }
    void btnDelete_slot(){
        model_remove();
        model_sort_up();
    }
    void btnAdd_slot(){
        int rowNum = model->rowCount();                         //获得表的行数
        model->insertRow(rowNum);
        QModelIndex index = model->index(rowNum-1,0);         //选中行第1列的内容
        int value = index.data().toInt()+1;
        model->setData(model->index(rowNum,0),value);
        model->setData(model->index(rowNum,1),0);
    }
    void btnClear_slot(){
        int rowNum = model->rowCount();
        if(rowNum >= 0){
            for(int i=rowNum-1;i>=0;i--){
                model->removeRow(i);   //删除该行
            }
            model->submitAll();             //提交，在数据库中删除该行
        }
    }
};

#endif // SETTINGTIMEWIDGET_H
