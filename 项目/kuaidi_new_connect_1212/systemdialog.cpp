#include <QMessageBox>
#include "systemdialog.h"
#include "ui_systemdialog.h"
#include "filewr.h"

SystemDialog::SystemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemDialog)
{
    ui->setupUi(this);
    this->setStyleSheet("#SystemDialog{background-image:url(/mnt/res/bg.png);}");


    ui->SaveButton->setStyleSheet("#SaveButton{border:2px groove gray;border-radius:5px;background-color:white;}");
    ui->ExitButton->setStyleSheet("#ExitButton{border:2px groove gray;border-radius:5px;background-color:white;}");
    ui->textEditRemoteIP->setStyleSheet("#textEditRemoteIP{border:2px groove gray;border-radius:5px;padding-left:2px;padding-top:2px;background-color:white;}");
    ui->textEditLocalIP->setStyleSheet("#textEditLocalIP{border:2px groove gray;border-radius:5px;padding-left:2px;padding-top:2px;background-color:white;}");
    ui->textEditMAC->setStyleSheet("#textEditMAC{border:2px groove gray;border-radius:5px;padding-left:2px;padding-top:2px;background-color:white;}");
    ui->textEditGateWay->setStyleSheet("#textEditGateWay{border:2px groove gray;border-radius:5px;padding-left:2px;padding-top:2px;background-color:white;}");
    ui->textEditPort->setStyleSheet("#textEditPort{border:2px groove gray;border-radius:5px;padding-left:2px;padding-top:2px;background-color:white;}");
    ui->textEditBoardCount->setStyleSheet("#textEditBoardCount{border:2px groove gray;border-radius:5px;padding-left:2px;padding-top:2px;background-color:white;}");
    ui->textEditLockCount->setStyleSheet("#textEditLockCount{border:2px groove gray;border-radius:5px;padding-left:2px;padding-top:2px;background-color:white;}");

    FileWR *fr = new FileWR();
    QString sys_info = fr->Read("/opt/sys_info.txt");

    if(sys_info!="error")
    {
        QStringList sections = sys_info.split(",");
        this->ui->textEditRemoteIP->setText(sections[0]);
        this->ui->textEditLocalIP->setText(sections[1]);
        this->ui->textEditMAC->setText(sections[2]);
        this->ui->textEditGateWay->setText(sections[3]);
        this->ui->textEditPort->setText(sections[4]);
        this->ui->textEditBoardCount->setText(sections[5]);
        this->ui->textEditLockCount->setText(sections[6]);
    }
    //QMessageBox::information(this, "提示信息", "设置保存成功！");
}

SystemDialog::~SystemDialog()
{
    delete ui;
}

void SystemDialog::on_SaveButton_clicked()
{

    FileWR *fr = new FileWR();

    QString sys_info = this->ui->textEditRemoteIP->toPlainText();
    sys_info = sys_info+","+this->ui->textEditLocalIP->toPlainText();
    sys_info = sys_info+","+this->ui->textEditMAC->toPlainText();
    sys_info = sys_info+","+this->ui->textEditGateWay->toPlainText();
    sys_info = sys_info+","+this->ui->textEditPort->toPlainText();
    sys_info = sys_info+","+this->ui->textEditBoardCount->toPlainText();
    sys_info = sys_info+","+this->ui->textEditLockCount->toPlainText();

    fr->Write("/opt/sys_info.txt", sys_info);

    QMessageBox::information(this, "提示信息", "设置保存成功！");

}
