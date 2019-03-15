#include "managerdialog.h"
#include "ui_managerdialog.h"
#include "engineerdialog.h"
#include "configure.h"

ManagerDialog::ManagerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManagerDialog)
{
    ui->setupUi(this);
    this->setStyleSheet("#ManagerDialog{background-image:url(/mnt/res/bg.png);}");

    ui->LineEditPwd->setStyleSheet("#LineEditPwd{border:2px groove gray;border-radius:10px;padding-left:5px;padding-top:5px;background-color:white;}");
    ui->Button1->setStyleSheet("#Button1{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->Button2->setStyleSheet("#Button2{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->Button3->setStyleSheet("#Button3{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->Button4->setStyleSheet("#Button4{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->Button5->setStyleSheet("#Button5{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->Button6->setStyleSheet("#Button6{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->Button7->setStyleSheet("#Button7{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->Button8->setStyleSheet("#Button8{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->Button9->setStyleSheet("#Button9{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->Button0->setStyleSheet("#Button0{border:2px groove gray;border-radius:10px;background-color:white;}");


    ui->ButtonPageUp->setStyleSheet("#ButtonPageUp{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->ButtonReturn->setStyleSheet("#ButtonReturn{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->ButtonPageDown->setStyleSheet("#ButtonPageDown{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->ButtonPoint->setStyleSheet("#ButtonPoint{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->ButtonEnter->setStyleSheet("#ButtonEnter{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->ButtonCancel->setStyleSheet("#ButtonCancel{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->ButtonExit->setStyleSheet("#ButtonExit{border:2px groove gray;border-radius:10px;background-color:white;}");
}

ManagerDialog::~ManagerDialog()
{
    delete ui;
}


void ManagerDialog::DispPwd(QString c)
{
    this->pwd.append(c);
    this->ui->LineEditPwd->setText(this->pwd);
}


void ManagerDialog::on_ButtonEnter_clicked()
{
    EngineerDialog* engineerDlg =new EngineerDialog(this);
#ifdef PC_DEBUG
    engineerDlg->setFixedSize(1027, 768);
    engineerDlg->show();
#else
    engineerDlg->showFullScreen();
#endif
}

void ManagerDialog::on_Button1_clicked()
{
     DispPwd(this->ui->Button1->text());
}

void ManagerDialog::on_Button2_clicked()
{
    DispPwd(this->ui->Button2->text());
}

void ManagerDialog::on_Button3_clicked()
{
    DispPwd(this->ui->Button3->text());
}

void ManagerDialog::on_Button4_clicked()
{

    DispPwd(this->ui->Button4->text());
}

void ManagerDialog::on_Button5_clicked()
{
    DispPwd(this->ui->Button5->text());
}

void ManagerDialog::on_Button6_clicked()
{
    DispPwd(this->ui->Button6->text());
}

void ManagerDialog::on_Button7_clicked()
{
    DispPwd(this->ui->Button7->text());
}

void ManagerDialog::on_Button8_clicked()
{
    DispPwd(this->ui->Button8->text());
}

void ManagerDialog::on_Button9_clicked()
{
    DispPwd(this->ui->Button9->text());
}

void ManagerDialog::on_Button0_clicked()
{
    DispPwd(this->ui->Button0->text());
}

void ManagerDialog::on_ButtonPoint_clicked()
{
    DispPwd(this->ui->ButtonPoint->text());
}

