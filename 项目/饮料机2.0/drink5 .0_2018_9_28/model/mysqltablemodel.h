#ifndef MYSQLTABLEMODEL_H
#define MYSQLTABLEMODEL_H

#include <QSqlTableModel>

#include <QSqlQueryModel>
#include <QtSql>
#include "model/tableputouttimeinfo.h"

class MySqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit MySqlTableModel(QObject *parent=0);

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole)const{
        if (role == Qt::TextAlignmentRole) {//默认代理的对齐方式
            QVariant value = (Qt::AlignCenter);
            return value;
        }
        else {
            return QSqlQueryModel::data(item,role);
        }
     }

     bool setData(const QModelIndex &index,const QVariant &value,int role=Qt::EditRole){
         int row = index.row();
         int v = index.data().toInt();//index.column() == 1 &&
         if(v >= 0){
             TablePutOutTimeInfo::getInstence()->table_replace(
                         TablePutOutTimeInfo::getInstence()->getTableName(),
                         row+1,v
                         );

             return QSqlTableModel::setData(index,value,role);
         }
         return QSqlTableModel::setData(index,value,role);
     }
};

#endif // MYSQLTABLEMODEL_H
