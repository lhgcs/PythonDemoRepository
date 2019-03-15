#include "engineerdialog.h"
#include "ui_engineerdialog.h"
#include "configure.h"
#include "openboxdialog.h"
#include "checkboxsdialog.h"
#include "systemdialog.h"

EngineerDialog::EngineerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EngineerDialog)
{
    ui->setupUi(this);
    this->setStyleSheet("#EngineerDialog{background-image:url(/mnt/res/bg.png);}");


    ui->RegisterButton->setStyleSheet("#RegisterButton{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->OpenButton->setStyleSheet("#OpenButton{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->SystemButton->setStyleSheet("#SystemButton{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->ExitButton->setStyleSheet("#ExitButton{border:2px groove gray;border-radius:10px;background-color:white;}");
}

EngineerDialog::~EngineerDialog()
{
    delete ui;
}

void EngineerDialog::on_OpenButton_clicked()
{
    OpenBoxDialog* openBoxDlg =new OpenBoxDialog(this);
    openBoxDlg->setWindowFlags(Qt::SubWindow);
    openBoxDlg->setFixedSize(768, 680);
    openBoxDlg->move(0, 0);
    openBoxDlg->show();

}

void EngineerDialog::on_RegisterButton_clicked()
{
    CheckBoxsDialog* checkBoxsDlg =new CheckBoxsDialog(this);
    checkBoxsDlg->setWindowFlags(Qt::SubWindow);
    checkBoxsDlg->setFixedSize(768, 680);
    checkBoxsDlg->move(0, 0);
    checkBoxsDlg->show();
}

void EngineerDialog::on_SystemButton_clicked()
{
    SystemDialog* systemDlg =new SystemDialog(this);
    systemDlg->setWindowFlags(Qt::SubWindow);
    systemDlg->setFixedSize(768, 680);
    systemDlg->move(0, 0);
    systemDlg->show();
}
