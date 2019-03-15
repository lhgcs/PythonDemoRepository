#include "mydialog.h"
#include "ui_mydialog.h"
#include <QPainter>
#include <QPen>

myDialog::myDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::myDialog)
{
    ui->setupUi(this);
    //outline: none  padding: -1
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    //this->setAttribute(Qt::WA_DeleteOnClose);        //否则关闭的时候内存释放不了
    this->setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口
    //this->ui->pushButton->setFocusPolicy(Qt::NoFocus);//可以去掉虚线，同样失去焦点

    this->ui->pushButton->setStyleSheet("#pushButton{border:none;border-radius:8px;background-color:rgba(222,222,222,255);outline: none;}" "#pushButton:pressed{background-color:rgb(0,255,127);color:white;border-style:inset;}");

    //border:0px groove gray;
    this->move(210,600);
}

myDialog::~myDialog()
{
    delete ui;
}

void myDialog::on_pushButton_clicked()
{
    this->close();
}

void myDialog::setText(QString text)
{
    ui->label->setWordWrap(true);//设置label中的文字可以换行
    ui->label->setText(text);
}

QString myDialog::text()
{
   return ui->label->text();
}

void myDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//反锯齿;
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    //rect.setWidth(rect.width() - 1);
    //rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 15, 15);
 }
