#include "checkboxsdialog.h"
#include "ui_checkboxsdialog.h"
#include "configure.h"
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <filewr.h>
#include <json.h>

CheckBoxsDialog::CheckBoxsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckBoxsDialog)
{
    ui->setupUi(this);
    this->setStyleSheet("#CheckBoxsDialog{background-image:url(/mnt/res/bg.png);}");
    ui->OpenAllButton->setStyleSheet("#OpenAllButton{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->ChekedButton->setStyleSheet("#ChekedButton{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->AuthenButton->setStyleSheet("#AuthenButton{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->ExitButton->setStyleSheet("#ExitButton{border:2px groove gray;border-radius:10px;background-color:white;}");
}

CheckBoxsDialog::~CheckBoxsDialog()
{
    delete ui;
}


void CheckBoxsDialog::replyFinished(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();
    //这里会输出百度首页的HTML网页代码

    QString str=QByteArray(data);
    str.replace("\n","");

    cJSON *json;

    int result=2;

    char *text = str.toUtf8().data();

    json=cJSON_Parse(text);
    if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
    else
    {
         result = cJSON_GetObjectItem(json, "result")->valueint;

         if(result==1)
         {
             char* uid = cJSON_GetObjectItem(json, "uid")->valuestring;
             FileWR *fr = new FileWR();
             fr->Write("/opt/uid.txt", uid);
              QMessageBox::information(this, "提示信息", "初始化成功，生成机柜标识:"+QString( QString::fromUtf8(uid)));
         }
         else
         {
             char* errMsg = cJSON_GetObjectItem(json, "errMsg")->valuestring;
             QMessageBox::information(this, "提示信息", QString( QString::fromUtf8(errMsg)));
         }
    }

    qDebug()<< str;
}

void CheckBoxsDialog::replyRegister(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();
    //这里会输出百度首页的HTML网页代码

    QString str=QByteArray(data);
    str.replace("\n","");

    cJSON *json;

    int result=2;

    char *text = str.toUtf8().data();

    json=cJSON_Parse(text);
    if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
    else
    {
         result = cJSON_GetObjectItem(json, "result")->valueint;

         if(result==1)
         {
             char* errMsg = cJSON_GetObjectItem(json, "errMsg")->valuestring;
             QMessageBox::information(this, "提示信息", QString( QString::fromUtf8(errMsg)));
         }
         else
         {
             char* errMsg = cJSON_GetObjectItem(json, "errMsg")->valuestring;
             QMessageBox::information(this, "提示信息", QString( QString::fromUtf8(errMsg)));
         }
    }

    qDebug()<< str;
}

void CheckBoxsDialog::on_AuthenButton_clicked()
{

}

void CheckBoxsDialog::on_OpenAllButton_clicked()
{

}

void CheckBoxsDialog::on_ChekedButton_clicked()
{
    //创建一个请求
    QNetworkRequest request;
    int i=0;
    bool ok;

    FileWR *fr = new FileWR();
    QString sys_info = fr->Read("/opt/sys_info.txt");

    FileWR *fr_uid = new FileWR();
    QString uid = fr_uid->Read("/opt/uid.txt");

    if(sys_info!="error")
    {
        QStringList sections = sys_info.split(",");

        QString strurl = "http://"+sections[0]+":"+sections[4]+"/app_reg_boxcontainer.jsp?uid=";
        strurl = strurl+uid+"&lockRoomNum="+sections[5];

        for(i=1;i<=sections[5].toInt(&ok,10);i++)
        {
            strurl = strurl + "&lockRoomId"+QString::number(i)+"="+QString::number(i);
        }
        for(i=1;i<=sections[6].toInt(&ok,10);i++)
        {
            strurl = strurl + "&boxNum"+QString::number(i)+"="+QString::number(i);
        }

        QMessageBox::information(this, "提示信息", strurl);

        request.setUrl(QUrl(strurl));
        //创建一个管理器
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        //发送GET请求
        QNetworkReply *reply = manager->get(request);
        //连接请求结束信号
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyRegister(QNetworkReply*)));
    }

}
