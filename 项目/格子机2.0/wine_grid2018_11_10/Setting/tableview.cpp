#include "tableview.h"

TableView::TableView(QWidget *parent): QTableView(parent)
{
    this->setAlternatingRowColors(true);
//    this->horizontalHeader()->setStretchLastSection(true);
//    this->horizontalHeader()->setHighlightSections(false);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分列
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);  //均分行

    this->verticalHeader()->setVisible(true);
    this->setShowGrid(false);
    this->setFocusPolicy(Qt::NoFocus);                            //设置不显示点击虚线

    this->setEditTriggers(QAbstractItemView::CurrentChanged);
    this->setSelectionMode(QAbstractItemView::SingleSelection);   //选中单个目标
    this->setSelectionBehavior (QAbstractItemView::SelectRows);   //设置选择行为时每次选择一行

    link_database();
    get_database_data();
    dialog=new SettingDialog(this);
    QObject::connect(dialog,SIGNAL(up_data(QString,QString)),this,SLOT(up_data_slot(QString,QString)));
}

TableView::~TableView(void){
    if(model) {
        delete model;
        model = NULL;
    }
}
