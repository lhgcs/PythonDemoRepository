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
#include "model/mysqlquerymodel.h"

#include "configure/toast.h"

class SettingTimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingTimeWidget(QWidget *parent = 0);
    int get_record_num(){
        return model->rowCount();
    }
    void referce(){
        model->refresh();
    }

private:
    void get_database_data();
    void model_remove();
    void model_insert();

    QTableView *tableView;
    MySqlQueryModel *model;
    QPushButton *btnInsert,*btnDelete,*btnAdd,*btnClear;
    QHBoxLayout *hl;
    QVBoxLayout *vl;

private slots:
    void btnInsert_slot(){
        model_insert();
    }
    void btnDelete_slot(){
        model_remove();
    }
    void btnAdd_slot(){
        int rowNum = model->rowCount();                       //获得表的行数
        QModelIndex index = model->index(rowNum-1,0);         //选中行第1列的内容
        int value = index.data().toInt()+1;
        model->insert_row(value,0);
        model->refresh();
        tableView->scrollToBottom();
    }
    void btnClear_slot(){
        model->remove_row_all();
        model->refresh();
    }
};

#endif // SETTINGTIMEWIDGET_H
