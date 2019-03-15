#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "playsound.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <QDebug>
void MainWindow::on_pushButton_clicked()
{
    PlaySound::getInstance()->play("/home/yk/Desktop/f.wav");
}
