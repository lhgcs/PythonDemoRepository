#include "mainwindow.h"
#include "managerdialog.h"
#include "ui_mainwindow.h"
#include "engineerdialog.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include "configure.h"
#include "filewr.h"
#include "json.h"
#include "cJSON.h"
#include <time.h>

#include "uart.h"
#include "UartQrcode.h"
#include "UartIC.h"

#include <QtWebKit/QtWebKit>
#include <QtWebKit/QWebView>
#include <QUrl>

void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type)
    {
        //调试信息提示
        case QtDebugMsg:
           txt = QString("Debug: %1").arg(msg);
           break;
        //一般的warning提示
        case QtWarningMsg:
           txt = QString("Warning: %1").arg(msg);
           break;
       //严重错误提示
       case QtCriticalMsg:
           txt = QString("Critical: %1").arg(msg);
           break;
       //致命错误提示
       case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
            abort();
            break;
       default:
            break;
   }
   QString message = QString("[%1] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")).arg(txt);
   QFile outFile("/mnt/debuglog.txt");
   if(outFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))//读写、追加，不存在则创建
   {
        QTextStream ts(&outFile);
        ts << message << endl;     //日志信息追加
        outFile.flush();
        outFile.close();

        if(outFile.size()>10485760)//日志文件大于10M
        {
            QFile outFile2("/mnt/debuglog_copy.txt");
            if(outFile2.exists())
            {
                outFile2.close();
                outFile2.remove();
            }
            QFile::copy("/mnt/debuglog.txt","/mnt/debuglog_copy.txt");//备份
            outFile.remove();
        }
   }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCursor(Qt::BlankCursor);                //隐藏鼠标
    this->setStyleSheet("#MainWindow{background-image:url(/mnt/res/welcome.png);}");
    ui->LineEditPwd->setStyleSheet("#LineEditPwd{border:none;border-radius:12px;padding-left:5px;padding-top:5px;background-color:rgba(255,255,255,255);}");
    ui->Button1->setStyleSheet("#Button1{border:none;border-radius:12px;background-color:rgba(255,255,255,255); }" "#Button1:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->Button2->setStyleSheet("#Button2{border:none;border-radius:12px;background-color:rgba(255,255,255,255); }" "#Button2:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->Button3->setStyleSheet("#Button3{border:none;border-radius:12px;background-color:rgba(255,255,255,255); }" "#Button3:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->Button4->setStyleSheet("#Button4{border:none;border-radius:12px;background-color:rgba(255,255,255,255); }" "#Button4:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->Button5->setStyleSheet("#Button5{border:none;border-radius:12px;background-color:rgba(255,255,255,255); }" "#Button5:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->Button6->setStyleSheet("#Button6{border:none;border-radius:12px;background-color:rgba(255,255,255,255); }" "#Button6:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->Button7->setStyleSheet("#Button7{border:none;border-radius:12px;background-color:rgba(255,255,255,255); }" "#Button7:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->Button8->setStyleSheet("#Button8{border:none;border-radius:12px;background-color:rgba(255,255,255,255); }" "#Button8:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->Button9->setStyleSheet("#Button9{border:none;border-radius:12px;background-color:rgba(255,255,255,255); }" "#Button9:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->Button0->setStyleSheet("#Button0{border:none;border-radius:12px;background-color:rgba(255,255,255,255); }" "#Button0:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->ButtonCancel->setStyleSheet("#ButtonCancel{border:none;border-radius:12px;background-color:rgba(255,255,255,255);}" "#ButtonCancel:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->ButtonEnter->setStyleSheet("#ButtonEnter{border:none;border-radius:12px;background-color:rgba(255,255,255,255);}""#ButtonEnter:pressed{background-color:rgb(0,255,127);color:white;}");
    ui->ButtonEnter->setShortcut(QKeySequence::InsertParagraphSeparator);//设置快捷键为键盘的“回车”键

    this->msgtimer = new QTimer(this);              //提示对话框
    this->msgtimer->setSingleShot(true);
    this->msgBox=new myDialog();
    connect(this->msgtimer,SIGNAL(timeout()),this,SLOT(msgTimeout()));

    this->manager = new QNetworkAccessManager(this);//请求
    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    this->view=new QWebView(this);                  //图片浏览
    this->view->resize(768,1360);
    //this->view->showFullScreen();
    this->viewTimer=new QTimer(this);
    this->viewTimer->setSingleShot(true);
    this->loadflag=false;
    connect(this->viewTimer, SIGNAL(timeout()),this, SLOT(viewTimerout()));
    connect(this->view, SIGNAL(selectionChanged()),this, SLOT(viewNotShow()));
    connect(this->view,SIGNAL(loadFinished(bool)),this,SLOT(viewShow(bool)));
    this->view->close();

/*    FileWR runfile;
    QString run = runfile.Read("/mnt/run.txt");
    if (run=="shengxian")                          //运行生鲜
    {
        UartQrcode *uart_qrcode=new UartQrcode();  //二维码扫描
        uart_qrcode->start();

        UartIC *uart_ic=new UartIC();              //IC卡扫描
        uart_ic->start();
    }
*/
    ui->LineEditPwd->setFocus();                   //输入框获得焦点
    qInstallMsgHandler(customMessageHandler);      //日志处理

    this->viewTimer->start(30000);                 //30秒
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::DispPwd(QString c)//输入框显示
{
    if(c == NULL)
    {
        this->ui->LineEditPwd->setText("");
    }
    else
    {
        this->ui->LineEditPwd->setText(this->ui->LineEditPwd->text().append(c));
    }
    this->ui->LineEditPwd->setFocus();
}

void MainWindow::on_Button1_clicked()
{
    DispPwd("1");
}
void MainWindow::on_Button2_clicked()
{
    DispPwd("2");
}
void MainWindow::on_Button3_clicked()
{
    DispPwd("3");
}
void MainWindow::on_Button4_clicked()
{
    DispPwd("4");
}
void MainWindow::on_Button5_clicked()
{
    DispPwd("5");
}
void MainWindow::on_Button6_clicked()
{
    DispPwd("6");
}
void MainWindow::on_Button7_clicked()
{
    DispPwd("7");
}
void MainWindow::on_Button8_clicked()
{
    DispPwd("8");
}
void MainWindow::on_Button9_clicked()
{
    DispPwd("9");
}
void MainWindow::on_Button0_clicked()
{
    DispPwd("0");
}
void MainWindow::on_ButtonCancel_clicked()
{
    DispPwd(NULL);
}

void MainWindow::replyFinished(QNetworkReply* reply1)//数据下载完成后进行处理，解析，开箱
{
    QByteArray data = reply1->readAll();
    reply1->close();                                 //关闭请求   abort()
    reply1->deleteLater();                           //释放reply对象

    QString str=QByteArray(data);
    str.replace("\n","");                            //\n  替换为空
    QByteArray ba = str.toUtf8();
    char *text = ba.data();

    char logger[100];
    memset(logger, 0x0, sizeof(logger));
    sprintf(logger,"JSON:%s",text);
    printf("recevice:%s\n", text);
    qDebug(logger);

    cJSON *pArray = cJSON_CreateArray();             //创建JSON数组
    cJSON *json=cJSON_Parse(text);                   //字符串解析成json结构体,NULL错误

    if (!json) 
    {
        printf("json error\n");
       // sprintf(logger, "Error before:[%s]\n", (this->pwd.toLatin1()).data(), cJSON_GetErrorPtr());
       // printf("%s\n", logger);
        sprintf(logger, "Error before:[%s]\n",cJSON_GetErrorPtr());
        qDebug(logger);
        if(this->msgtimer->isActive())
        {
            this->msgtimer->stop();
        }
        this->msgBox->setText("服务器正在维护，请稍后再取件");
    }
    else
    {
        printf("json ok\n");
                                                               //cJSON_GetObjectItem() 函数负责进行某个对象的成员的名字比对和指针的返回
        int result = cJSON_GetObjectItem(json, "result")->valueint;//获取json指向的JSON节点(result)的值
        if(result==1)
        {
            pArray = cJSON_GetObjectItem(json, "lockRoom");    //获取lockRoom数组变量的指针
            if (NULL == pArray)
            {
                printf("Array=NULL");
                return;
            }

            int iCount = cJSON_GetArraySize(pArray);           //获取pArray数组的大小
            for (int i = 0; i < iCount; ++i)
            {
                cJSON* pItem = cJSON_GetArrayItem(pArray, i);  //i=0
                if (NULL == pItem)
                {
                    continue;
                }
                char* lockRoomId = cJSON_GetObjectItem(pItem, "lockRoomId")->valuestring;
                char* boxId = cJSON_GetObjectItem(pItem, "boxIds")->valuestring;
                uart_ctrl(atoi(lockRoomId), atoi(boxId));      //开箱操作
                this->msgBox->setText("");
             }
         }
         else
         {
            char* errMsg = cJSON_GetObjectItem(json, "errMsg")->valuestring;
            if(this->msgtimer->isActive())
            {
                this->msgtimer->stop();
            }
            this->msgBox->setText(QString::fromUtf8(errMsg));
         }
     }
     if((msgBox->text())!=NULL)
     {
         this->msgBox->show();
         this->msgtimer->start(10000);
     }
     cJSON_Delete(json);                            // 释放内存空间
}

void MainWindow::on_ButtonEnter_clicked()           //密码开箱，发送数据
{
    this->view->stop();
    if(this->view->isVisible())                     //浏览器可见
    {
        this->view->close();
        this->viewTimer->start(30000);
        ui->LineEditPwd->setFocus();
    }
    else
    {
        this->ui->LineEditPwd->setReadOnly(true);   //关闭只读模式
        this->pwd=this->ui->LineEditPwd->text();

        FileWR fr_admin;
        QString admin = fr_admin.Read("/opt/admin_pwd.txt");

        if(this->ui->LineEditPwd->text()==admin)    //系统设置
        {
            EngineerDialog* engineerDlg =new EngineerDialog(this);
            engineerDlg->setWindowFlags(Qt::SubWindow);
            engineerDlg->setFixedSize(768, 320);
            engineerDlg->move(0, 250);
            engineerDlg->show();
            this->DispPwd(NULL);
        }
        else if(this->ui->LineEditPwd->text()==NULL)//密码为空
        {
            if(this->msgtimer->isActive())
            {
                this->msgtimer->stop();
            }
            this->msgBox->setText("请输入密码");
            this->msgBox->show();
            this->msgtimer->start(10000);
        }
        else                                        //发送密码
        {
            FileWR fr,fr_uid;
            QString sys_info = fr.Read("/opt/sys_info.txt");

            if(sys_info!="error")
            {
                QString uid = fr_uid.Read("/opt/uid.txt");
                QStringList sections = sys_info.split(",");
                QString strurl = "http://"+sections[0]+":"+sections[4]+"/app_get_put_goods.jsp?uid="+uid+"&psw="+this->ui->LineEditPwd->text();
                QNetworkRequest request;
                request.setUrl(QUrl(strurl));
                QNetworkReply *reply = this->manager->get(request);//发送GET请求
            }
            this->DispPwd(NULL);
        }
        this->ui->LineEditPwd->setReadOnly(false);                 //关闭只读模式
    }
}

void MainWindow::msgTimeout()                     //关闭提示对话框
{
    if(this->msgBox->isVisible())
    {
        this->msgBox->close();
    }
    if(this->msgtimer->isActive())
    {
        this->msgtimer->stop();
    }
}

void MainWindow::viewTimerout()                   //广告下载
{
    //if(!(this->view->isVisible()))
   // {
        this->view->stop();
        //if(this->loadflag)
        //{
        //    this->view->reload();
       // }
        //else
        //{
            //FileWR file;
            //QString webUrl = file.Read("/opt/web.txt");
            //this->view->load(QUrl(webUrl));
            //this->view->load(QUrl("http://gztj.com.cn/ad/tu.html"));
        this->view->load(QUrl("http://gztj.com.cn/ad/tu.html"));
        //}
   // }
}

void MainWindow::viewShow(bool ok)                //显示广告
{
    //if(ok)
   // {

        this->view->show();                       //下载完再显示
        this->loadflag=true;
    //}
    //else
   // {
        this->viewTimer->start(30000);
   // }
}

void MainWindow::viewNotShow()                    //关闭广告
{
    this->view->stop();
    if(this->view->isVisible())                   //浏览器可见
    {
        this->view->close();
        this->viewTimer->start(30000);
        ui->LineEditPwd->setFocus();
    }
}

void MainWindow::on_LineEditPwd_textChanged(const QString &arg1)
{
    this->viewTimer->start(30000);
    this->viewNotShow();
}
