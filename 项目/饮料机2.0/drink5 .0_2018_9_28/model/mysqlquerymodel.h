#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlQuery>

class MySqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    MySqlQueryModel(QObject *parent);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setData(const QModelIndex &index, const QVariant &value);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    void refresh();

    bool insert_row(int myid,int time){
        QSqlQuery query;
        query.prepare("insert into putout_time_info values (?,?)");
        query.addBindValue(myid);
        query.addBindValue(time);
        return query.exec();
    }
    bool remove_row(int myid){
        QSqlQuery query;
        query.prepare("delete from putout_time_info where myid=?");
        query.addBindValue(myid);
        return query.exec();
    }
    bool remove_row_all(){
        QSqlQuery query;
        //query.prepare("TRUNCATE TABLE putout_time_info");
        query.prepare("delete from putout_time_info");
        return query.exec();
    }

private:
    bool setName(int studentId, const QString &name);


};

#endif // MYSQLQUERYMODEL_H
