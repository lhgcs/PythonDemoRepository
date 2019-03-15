#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpserver=new TCPThread;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //tcpserver->write_data(0,ui->textEdit_send->toPlainText());
}

void MainWindow::on_pushButton_2_clicked()
{   //对话框显示时默认打开的目录，"." 代表程序运行目录 ,对话框的后缀名过滤器
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("all(*.*);;Files(*.jpg *.png)"));
    if(path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    } else {
        tcpserver->send_file(path);
    }
}
