#include "settingtimewidget.h"
#include <QHeaderView>
#include "subwidget/dialoginstance.h"
#include "QScrollBar"


SettingTimeWidget::SettingTimeWidget(QWidget *parent) : QWidget(parent)
{
    tableView = new QTableView();
    tableView->setAlternatingRowColors(true);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->horizontalHeader()->setHighlightSections(false);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //均分列
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);  //均分行
    tableView->verticalHeader()->setVisible(false);
    tableView->setShowGrid(true);
    tableView->setFocusPolicy(Qt::NoFocus);                                       //设置不显示点击虚线
    tableView->setEditTriggers(QAbstractItemView::CurrentChanged);
    //tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);              //使其不可编辑
    tableView->setSelectionBehavior (QAbstractItemView::SelectRows);              //设置选择行为时每次选择一行
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);              //选中单个目标
    tableView->setAlternatingRowColors(true);

    tableView->verticalScrollBar()->setStyleSheet(
     "QScrollBar:vertical{"                //垂直滑块整体
          "min-width:36px;"
          "background:rgb(255,255,255);"   //背景色
          "padding-top:0px;"               //上预留位置（放置向上箭头）
          "padding-bottom:0px;"            //下预留位置（放置向下箭头）
          "padding-left:0px;"              //左预留位置（美观）
          "padding-right:0px;"
          "margin:0px;"                    //右预留位置（美观）
          "border-left:0px solid #d7d7d7;" //左分割线
      "}"
      "QScrollBar::handle:vertical{"       //滑块样式
          "background:rgb(239,61,87);"     //滑块颜色
          "border-radius:0px;"             //边角圆润
          "min-height:80px;"               //滑块最小高度
          "padding:3px;"
      "}"
//     "QScrollBar::handle:vertical:hover{" //鼠标触及滑块样式
//       "background:#d0d0d0;"              //滑块颜色
//     "}"
        "QScrollBar::add-line:vertical{"   //向下箭头样式
            "background:url(:/images/resource/images/checkout/down.png) center no-repeat;"
        "}"
        "QScrollBar::sub-line:vertical{"   //向上箭头样式
            "background:url(:/images/resource/images/checkout/up.png) center no-repeat;"
        "}"
    );

    btnInsert = new QPushButton("插入",this);
    btnDelete = new QPushButton("删除",this);
    btnAdd = new QPushButton("添加",this);
    btnClear = new QPushButton("清空",this);
    btnInsert->setFocusPolicy(Qt::NoFocus);
    btnDelete->setFocusPolicy(Qt::NoFocus);
    btnAdd->setFocusPolicy(Qt::NoFocus);
    btnClear->setFocusPolicy(Qt::NoFocus);
    QObject::connect(btnInsert,SIGNAL(clicked(bool)),this,SLOT(btnInsert_slot()));
    QObject::connect(btnDelete,SIGNAL(clicked(bool)),this,SLOT(btnDelete_slot()));
    QObject::connect(btnAdd,   SIGNAL(clicked(bool)),this,SLOT(btnAdd_slot()));
    QObject::connect(btnClear, SIGNAL(clicked(bool)),this,SLOT(btnClear_slot()));

    hl = new QHBoxLayout;
    hl->addWidget(btnInsert);
    hl->addWidget(btnDelete);
    hl->addWidget(btnAdd);
    hl->addWidget(btnClear);

    vl = new QVBoxLayout(this);
    vl->addWidget(tableView);
    vl->addLayout(hl);
    this->setLayout(vl);

    get_database_data();
    model_sort_up();
}

void SettingTimeWidget::get_database_data(){                //查询数据显示到tableview
    model = new MySqlTableModel(this);
    model->setTable("putout_time_info");
    model->setHeaderData(0,Qt::Horizontal,"货道");
    model->setHeaderData(1,Qt::Horizontal,"时间(秒)");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); //设置保存策略为手动提交
    model->select();                                        //选取整个表的所有行
    tableView->setModel(model);                             //绑定模型与视图,数据放置进去,//在筛选和排序的条件下,将数据库中符合要求的在mode表格中显示出来
    //this->hideColumn(0);                                  //隐藏某一列数据
    //this->setColumnHidden(0,true);                        //不显示name属性列,如果这时添加记录，则该属性的值添加不上
    //model->removeColumn(0);//隐藏不需要显示的列

//        QObject::connect(this->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
//        this, SLOT(showOperationDetails(const QModelIndex &, const QModelIndex &)));
//    QObject::connect(tableView,SIGNAL(clicked(QModelIndex)),this, SLOT(showOperationDetails(const QModelIndex)));
}
void SettingTimeWidget::model_remove() {                    //删除当前行  这里删除行的操作会先保存在model中，当我们执行了submitAll()函数后才会真正的在数据库中删除该行
    int curRow = tableView->currentIndex().row();           //获取选中的行
    if(curRow >= 0){
        model->removeRow(curRow);                           //删除该行
        model->submitAll();                                 //提交，在数据库中删除该行
    }
}
void SettingTimeWidget::model_insert()  {                   //插入记录
    int rowNum = model->rowCount();                         //获得表的行数
    int curRow = tableView->currentIndex().row();
    if(rowNum ==0){                                         //第一行
        int id = 0;
        model->insertRow(id);                               //添加一行
        model->setData(model->index(id,0),1);
    }else if(rowNum == curRow+1){                           //最后一行
        int id = curRow+1;
        model->insertRow(id);
        QModelIndex index = model->index(curRow,0);         //选中行第1列的内容
        int value = index.data().toInt()+1;
        model->setData(model->index(id,0),value);
    }else{
        QModelIndex index = model->index(curRow,0);         //选中行第2列的内容
        int value = index.data().toInt()+1;
        int id = curRow+1;
        for(int i=0;i<rowNum;i++){
            QModelIndex index = model->index(i,0);
            int v = index.data().toInt();
            if(v == value){
                DialogInstance::getInstance()->setText("已存在"+QString::number(v)+"货道");
                return;
            }
        }
        model->insertRow(id);
        model->setData(model->index(id,0),value);
    }
    model->submitAll();                                     //可以直接提交
}

