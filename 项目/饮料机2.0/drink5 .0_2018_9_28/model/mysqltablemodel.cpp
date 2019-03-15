#include "mysqltablemodel.h"

MySqlTableModel::MySqlTableModel(QObject *parent):QSqlTableModel(parent)
{
//    QString sql="select "
//                    "userNumId ,"
//                    "TaskNum "
//                    "from taskinfo where executor =''";
//        this->setQuery(sql);
//        this->setHeaderData(0,Qt::Horizontal,"发布ID");//设置表头显示的数据
//        this->setHeaderData(1,Qt::Horizontal,"订单编号");

}

//QVariant MySqlTableModel::data(const QModelIndex &item, int role) const{
//    if (role == Qt::TextAlignmentRole) {//默认代理的对齐方式
//        QVariant value = (Qt::AlignCenter);
//        return value;
//    }
//    else {
//        return QSqlQueryModel::data(item,role);
//    }
//}

//bool MySqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role){
//if (role == Qt::EditRole )
//{
//if (index.column() == 6)
//{
//m_devList->at(index.row()).personalUserName = value.toString().toStdString();
//}
//if (index.column() == 7)
//{
//m_devList->at(index.row()).personalPassword = value.toString().toStdString();
//}
//}
//emit this->dataChanged(index, index);
//}
