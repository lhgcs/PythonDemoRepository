#include "update.h"

#include <QHeaderView>
#include <QScrollBar>

#define ROW_H    50
#define COLUMN_H 100

Update::Update(int *_status_p, QWidget *parent)//:QTabWidget(parent)
{
//    setStyleSheet("selection-background-color:lightblue;");  //设置选中背景色
//    this->setStyleSheet("QTableWidget::item{border:1px solid rgb(45, 45, 45);}");
   this->resize(720,360);
//    this->move(0,405);
    status_p=_status_p;
    this->setFrameShape(QFrame::NoFrame); //设置无边框
    this->setShowGrid(false);             //设置不显示格子线
    this->setWindowTitle("修改数量");
    this->setColumnCount(6);
    this->setRowCount(21);

//    //设置水平、垂直滚动条样式
//     this->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
//      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
//      "QScrollBar::handle:hover{background:gray;}"
//      "QScrollBar::sub-line{background:transparent;}"
//      "QScrollBar::add-line{background:transparent;}");
//      this->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
//      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
//      "QScrollBar::handle:hover{background:gray;}"
//      "QScrollBar::sub-line{background:transparent;}"
//      "QScrollBar::add-line{background:transparent;}");

    //设置表头字体，颜色，模式
//    this->horizontalHeader()->setStyleSheet(
//    "QHeaderView::section {background-color:lightblue;color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
    this->verticalHeader()->setVisible(false);   //隐藏列表头
//this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //行自适应宽
//    this->horizontalHeader()->setFixedHeight(50);                        //设置表头的高度
//    this->horizontalHeader()->setStretchLastSection(true);               //设置充满表宽度

//    horizontalHeader()->setDefaultSectionSize(120);
//     horizontalHeader()->setMaximumWidth(200);
//    verticalHeader()->setDefaultSectionSize(60);
    for(int j=0;j<6;j++)
    {
       if(j%2==0)
       {
           setColumnWidth(j,100);//指定某列的大小
       }
       else
       {
           setColumnWidth(j,140);//指定某列的大小
       }
    }
//    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//先自适应宽度,使列完全填充并平分
//    for(int i=0;i<6;i++)
//    {
//        this->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents); //然后设置要根据内容使用宽度的列
//    }
//    int colCount = this->horizontalHeader()->count();
//    for (int i = 0; i < colCount-1; i++)
//    {
//        resizeColumnToContents(i);
//    }



    //设置表头字体加粗
    QFont font = this->horizontalHeader()->font();
    font.setBold(true);
    this->horizontalHeader()->setFont(font);
    QStringList header;
    header<<"货道"<<"数量"<<"货道"<<"数量"<<"货道"<<"数量";
    this->setHorizontalHeaderLabels(header);
    this->horizontalHeader()->setSectionsClickable(false);               //设置表头不可点击（默认点击后进行排序）
//    this->horizontalHeader()->resizeSection(0,10000);
//    QTableWidgetItem *columnHeaderItem0 = this->horizontalHeaderItem(0); //获得水平方向表头的Item对象
//    columnHeaderItem0->setFont(QFont("Helvetica"));                      //设置字体
//    columnHeaderItem0->setBackgroundColor(QColor(0,60,10));              //设置单元格背景颜色
//    columnHeaderItem0->setTextColor(QColor(200,111,30));                 //设置文字颜色

//    tableWidget->setItem(0,1,new QTableWidgetItem(QIcon("images/IED.png"), "Jan's month"));
//    tableWidget->setItem(1,1,new QTableWidgetItem(QIcon("images/IED.png"), "Feb's month"));
//    tableWidget->setItem(2,1,new QTableWidgetItem(QIcon("images/IED.png"), "Mar's month"));
 //this->setItem(i,j,new QTableWidgetItem(QString::number(i*6+j+1)));
   // checkBox=new QCheckBox[60];
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<6;j++)
        {
            if(j%2==0)
            {
                checkBox.append(new QCheckBox(QString::number(i*3+j/2+1),this));
                this->setCellWidget(i,j,checkBox.back());//在单元格里加入控件
            }
            else
            {
                button.append(new ShoppingCart());
                this->setCellWidget(i,j,button.back());//在单元格里加入控件
            }
        }
    }

    button_all=new QPushButton("全选",this);
    button_up=new QPushButton("同步数据",this);
    button_down=new QPushButton("下载数据",this);
    button_close=new QPushButton("退出",this);
    this->setCellWidget(20,0,button_all);
    this->setCellWidget(20,1,button_up);
    this->setCellWidget(20,2,button_down);
    this->setCellWidget(20,3,button_close);
    QObject::connect(button_all,SIGNAL(clicked(bool)),this,SLOT(button_all_slot()));
    QObject::connect(button_up,SIGNAL(clicked(bool)),this,SLOT(button_up_slot()));
    QObject::connect(button_down,SIGNAL(clicked(bool)),this,SLOT(button_down_slot()));
    QObject::connect(button_close,SIGNAL(clicked(bool)),this,SLOT(close()));

    this->setStyleSheet(
      "QTableWidget{"
              "background:rgb(57,58,60);"
              "gridline-color:rgb(50,50,50);"
              "}"
      "QTableWidget::item {"
          "padding-left: 5px;"
          "padding-right: 5px;"
          "border: none;"
          "background: rgb(72,72,74);"
          "border-right: 1px solid rgb(45, 45, 45);"
          "border-bottom: 1px solid rgb(45, 45, 45);"
      "}"
      "QTableView::item:selected {background: rgba(255, 255, 255, 40);}"
      "QTableView::item:selected:!active {color: white;}"

      "QCheckBox{color: white;}"//spacing: 20px;
      "QCheckBox::indicator {width:48px; height:48px;}"//改变大小
      "QCheckBox::indicator:checked {image:url(/home/ico/check.png)}"//选中时的样子
      "QCheckBox::indicator:unchecked {image:url(/home/ico/uncheck.png)}"
      "QPushButton{border-radius:5px;font-size:30px;"
      "background-color:rgba(0,0,0,0);border:1px solid rgb(111,156,207);color:rgb(111,156,207)}"
      );
    this->show();

    for(int i=0;i<GOODS_TOTAL;i++)
    {
        if(status_p[i]>0)
        {
            checkBox[i]->setCheckState(Qt::Checked);
            button[i]->setText(QString::number(status_p[i]));
        }
    }
}

