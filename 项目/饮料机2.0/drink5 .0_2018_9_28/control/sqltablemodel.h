#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>
#include <QTableView>

class SqlTableModel : public QSqlTableModel
{
public:
    SqlTableModel(QTableView *parent=0);
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const{
        QVariant value  = QSqlTableModel::data(idx,role);
            if(Qt::TextAlignmentRole == role){  //控制对齐方式
                value   = int(Qt::AlignCenter | Qt::AlignHCenter);
                return value;
            }
//            if(Qt::ForegroundRole == role){     //控制前景色
//                if(3 == idx.column())
//                    return QVariant(QColor(255,144,144));
//            }
//            if(Qt::FontRole == role){            //控制字体
//                if(3 == idx.column()){
//                    QFont font;
//                    font.setBold(true);
//                    return QVariant(font);
//                }
//            }
            return value;
    }
};

#endif // SQLTABLEMODEL_H
