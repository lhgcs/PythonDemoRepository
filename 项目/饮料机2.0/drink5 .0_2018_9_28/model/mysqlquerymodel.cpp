#include "mysqlquerymodel.h"
#include <QColor>
#include <QSqlQuery>

MySqlQueryModel::MySqlQueryModel(QObject *parent)
{

}
Qt::ItemFlags MySqlQueryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == 1) //第二个属性可更改
        flags |= Qt::ItemIsEditable;
    return flags;
}

bool MySqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() < 1 || index.column() > 2)
        return false;
    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
    int id = data(primaryKeyIndex).toInt(); //获取id号
    clear();
    bool ok;
    if (index.column() == 1) //第二个属性可更改
        ok = setName(id, value.toString());
    refresh();
    return ok;
}

bool MySqlQueryModel::setData(const QModelIndex &index, const QVariant &value)
{
    if (index.column() < 1 || index.column() > 3)
        return false;
    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
    int id = data(primaryKeyIndex).toInt(); //获取id号
    clear();
    bool ok;
    if (index.column() == 1) //第二个属性可更改
        ok = setName(id, value.toString());
    refresh();
    return ok;
}

QVariant MySqlQueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    //第一个属性的字体颜色为红色
//    if (role == Qt::TextColorRole && index.column() == 0){

//        return qVariantFromValue(QColor(Qt::red));
//    }
//    return value;

    if (role == Qt::TextAlignmentRole) {//默认代理的对齐方式
        QVariant value = (Qt::AlignCenter);
        return value;
    }else {
        return QSqlQueryModel::data(index,role);
    }
}

bool MySqlQueryModel::setName(int id, const QString &name){
    QSqlQuery query;
    query.prepare("update putout_time_info set time = ? where myid = ?");
    query.addBindValue(name);
    query.addBindValue(id);
    return query.exec();
}

void MySqlQueryModel::refresh(){
    setQuery("select * from putout_time_info ORDER BY myid");
    setHeaderData(0,Qt::Horizontal,QObject::tr("货道"));
    setHeaderData(1,Qt::Horizontal,QObject::tr("时间(毫秒)"));
}
