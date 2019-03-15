#include "openboxdialog.h"
#include "ui_openboxdialog.h"
#include "configure.h"
#include "uart.h"

OpenBoxDialog::OpenBoxDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenBoxDialog)
{
    ui->setupUi(this);
    this->setStyleSheet("#OpenBoxDialog{background-image:url(/mnt/res/bg.png);}");


    ui->spinBardNo->setStyleSheet("#spinBardNo{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->spinBoxNo->setStyleSheet("#spinBoxNo{border:2px groove gray;border-radius:10px;background-color:white;}");

    ui->OpenButton->setStyleSheet("#OpenButton{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->ExitButton->setStyleSheet("#ExitButton{border:2px groove gray;border-radius:10px;background-color:white;}");
}

OpenBoxDialog::~OpenBoxDialog()
{
    delete ui;
}

void OpenBoxDialog::on_OpenButton_clicked()
{
    int bard_no = ui->spinBardNo->value();
    int box_no = ui->spinBoxNo->value();
    uart_ctrl((unsigned char)bard_no, (unsigned char)box_no);
}
