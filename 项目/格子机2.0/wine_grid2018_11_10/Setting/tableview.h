#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QList>

#include <QHeaderView>

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "settingdialog.h"
#include "sqltablemodel.h"

class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget *parent=0);
    ~TableView();

    QSqlDatabase db;
    SqlTableModel *model;
    SettingDialog *dialog;

    bool link_database()//连接数据库
    {
//        this->db=QSqlDatabase::addDatabase("QSQLITE");    //加载数据库驱动
//        this->db.setHostName("127.0.0.1");                //本地连接
//        this->db.setUserName("root");                     //root用户
//        //this->db.setPassword(""+passResult+"");         //mysql密码
//        this->db.setDatabaseName("/home/sql/database.db");//需要连接的数据库
//        if(!db.open())  {//打开
//            QMessageBox::warning(this,"错误","未连接，请检查.");
//            return false;
//        }
        return true;
    }
    void get_database_data(){                                      //查询数据显示到tableview
        model = new SqlTableModel(this);
        model->setTable("goods_info");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);    //设置保存策略为手动提交
        model->select();                                           //选取整个表的所有行
        this->setModel(model);                                     //绑定模型与视图,数据放置进去,//在筛选和排序的条件下,将数据库中符合要求的在mode表格中显示出来
        //this->hideColumn(1);                                     //隐藏某一列数据
        //this->setColumnHidden(1,true);                           //不显示name属性列,如果这时添加记录，则该属性的值添加不上
        //this->setColumnHidden(i,true);
        //隐藏不需要显示的列
        while(model->columnCount()>4) {
            qDebug()<<model->columnCount();
            model->removeColumn(model->columnCount()-1);
        }
        this->setEditTriggers(QAbstractItemView::NoEditTriggers);  //使其不可编辑
        this->setSelectionBehavior (QAbstractItemView::SelectRows);//设置选择行为时每次选择一行
//        QObject::connect(this->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
//        this, SLOT(showOperationDetails(const QModelIndex &, const QModelIndex &)));
        QObject::connect(this,SIGNAL(clicked(QModelIndex)),this, SLOT(showOperationDetails(const QModelIndex)));
    }
    void model_submit()   {              //提交修改
        model->database().transaction(); //开始事务操作
        if(model->submitAll()) {
            model->database().commit();  //提交
        }
        else {
            model->database().rollback(); //回滚
            QMessageBox::warning(this, tr("tableModel"),tr("数据库错误: %1").arg(model->lastError().text()));
        }
    }
    void model_revert()  {          //撤销修改
        model->revertAll();
    }
    void model_all()     {               //返回全表
        model->setTable("student");      //重新关联表
        model->select();                 //这样才能再次显示整个表的内容
    }
    void model_sort_up() {               //升序
        model->setSort(0,Qt::AscendingOrder);//id属性，即第0列，升序排列
        model->select();
    }
    void model_sort_down(){              //降序
        model->setSort(0,Qt::DescendingOrder);
        model->select();
    }
    void model_remove() {                 //删除当前行  这里删除行的操作会先保存在model中，当我们执行了submitAll()函数后才会真正的在数据库中删除该行
        int curRow = this->currentIndex().row();//获取选中的行
        model->removeRow(curRow);               //删除该行
        int ok = QMessageBox::warning(this,tr("删除当前行!"),tr("你确定""删除当前行吗？"),QMessageBox::Yes,QMessageBox::No);
        if(ok == QMessageBox::No){
            model->revertAll();          //如果不删除，则撤销
        }
        else model->submitAll();         //否则提交，在数据库中删除该行
    }
    void model_insert()  {               //插入记录
        int rowNum = model->rowCount();  //获得表的行数
        int id = 10;
        model->insertRow(rowNum);        //添加一行
        model->setData(model->index(rowNum,0),id);
        model->submitAll();              //可以直接提交
    }
    void model_selete() {                //查询
        QString name = "11";
        model->setFilter(QObject::tr("name = '%1'").arg(name)); //根据姓名进行筛选 使用setFilter()函数进行关键字筛选
        model->select(); //显示结果
    }
private slots:
    void showOperationDetails(const QModelIndex &current_index) {
        qDebug()<<"index"<<current_index.row()<<current_index.column();
        QStringList list;
        QAbstractItemModel* m = (QAbstractItemModel *)current_index.model();
        for(int i=0;i< m->columnCount();i++){
            QModelIndex x = m->index(current_index.row(),i);//选中行第一列的内容
            list<<x.data().toString();
        }
        dialog->set_context(list.at(1),list.at(2),list.at(3));
    }

    void up_data_slot(QString price,QString num){
        int curRow = this->currentIndex().row();         //获取选中的行
        QModelIndex index_price = model->index(curRow,2);//选中行第3列的内容
        QModelIndex index_num = model->index(curRow,3);  //选中行第4列的内容
        model->setData(index_price, QVariant::fromValue(price));
        model->setData(index_num, QVariant::fromValue(num));
        model_submit();
    }
};

#endif // TABLEVIEW_H


//"select box_id,case box_state when 1 then '待取' "
//"else '已取' end as box_state,case box_size when 1 "
//"then '大' when 2 then '中' else '小' end as box_size "
//"from box;"

//        beginResetModel();
//        endResetModel();
