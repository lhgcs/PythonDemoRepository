#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QFileDialog>
#include <QApplication>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setIconSize(QSize(10,10));//图标不够大，只会显示各自图标的最大尺寸
    this->setWindowIcon(QIcon(":/icon/images/my.png"));
    this->setWindowTitle("多媒体播放器");
    //打开文件按钮
    ui->pushButton_open->setIconSize(QSize(ui->pushButton_open->width(),ui->pushButton_open->height()));//设置图标大小
    ui->pushButton_open->setIcon(QPixmap(":/icon/images/open.png"));      //设置图标
    ui->pushButton_open->setAttribute(Qt::WA_TranslucentBackground, true);//控件背景色透明
    ui->pushButton_open->setStyleSheet("border: 0px");
    //上一个按钮
    ui->pushButton_previous->setIconSize(QSize(ui->pushButton_previous->width(),ui->pushButton_previous->height()));
    ui->pushButton_previous->setIcon(QPixmap(":/icon/images/previous.png"));
    ui->pushButton_previous->setAttribute(Qt::WA_TranslucentBackground, true);//控件背景色透明
    ui->pushButton_previous->setStyleSheet("border: 0px");
    //播放按钮
    ui->pushButton_next->setIconSize(QSize(ui->pushButton_previous->width(),ui->pushButton_previous->height()));
    ui->pushButton_next->setIcon(QPixmap(":/icon/images/next.png"));
    ui->pushButton_next->setAttribute(Qt::WA_TranslucentBackground, true);//控件背景色透明
    ui->pushButton_next->setStyleSheet("border: 0px");
    //下一个按钮
    ui->pushButton_pause->setIconSize(QSize(ui->pushButton_pause->width(),ui->pushButton_pause->height()));
    ui->pushButton_pause->setIcon(QPixmap(":/icon/images/stop.png"));
    ui->pushButton_pause->setAttribute(Qt::WA_TranslucentBackground, true);//控件背景色透明
    ui->pushButton_pause->setStyleSheet("border: 0px");
    //播放器
    m_player=new QMediaPlayer(this);
    videow = new QVideoWidget(this);
    m_player->setVideoOutput(videow);
    timer=new QTimer();
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(on_time_out()));
    //进度条
    ui->horizontalSlider->setRange(0,maxValue);
    ui->horizontalSlider->setValue(0);
    //时间显示
    ui->label->setText("00:00/00:00");
    //音量
    ui->volum->setRange(0,100);
    ui->volum->setSingleStep(20);
    ui->volum->setTickPosition(QSlider::TicksAbove);  //刻度在上方
    ui->volum->setValue(20);
    m_player->setVolume(ui->volum->value());
    //调速
    ui->rate->setRange(1,9);//0.2 0.4 0.6 0.8 1 1.2 1.4 1.6 1.8
    ui->rate->setSingleStep(1);
    ui->rate->setTickPosition(QSlider::TicksAbove);  //刻度在上方
    ui->rate->setValue(5);
    //播放窗口
    videow->setFixedSize(550,400);
    videow->move(50,25);
    videow->show();
    fileCount=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_open_clicked()//选择文件
{
    if(fileCount)//列表中已有文件
    {
        QStringList file=QFileDialog::getOpenFileNames(this,"choose movies","/","Movie (*.m4v *.mp4 *.avi *.wmv *.mpg *.mp3)");
        int add_file_count=file.count();
        if(add_file_count)
        {
            fileCount+=add_file_count;
            for(int i=0;i<add_file_count;i++)
            {
                fileNames.append(file.at(i));
                ui->list->insertItem(ui->list->currentRow()+1,file.at(i));
            }
        }
    }
    else//列表中还没有
    {
        fileNames=QFileDialog::getOpenFileNames(this,"choose movies","/","Movie (*.m4v *.mp4 *.avi *.wmv *.mpg *.mp3)");
        if(fileNames.count()!=0)
        {
            fileCount=fileNames.count();
            ui->list->addItems(fileNames);//加入列表
            ui->list->setCurrentRow(0);
            qDebug()<<fileNames.at(ui->list->currentRow());//返回当前选择的项的序号
            m_player->setMedia(QUrl::fromLocalFile(fileNames.at(ui->list->currentRow())));
        }
    }
}

void MainWindow::on_pushButton_previous_clicked()//上一个按钮
{  
    m_player->stop();
    ui->horizontalSlider->setValue(0);
    if(fileCount)//列表中有选项
    {
        if(ui->list->currentRow()==0)//当前在第一个
        {
           ui->list->setCurrentRow(ui->list->count()-1);//设置当前项为最后一项
        }
         else
        {
           ui->list->setCurrentRow(ui->list->currentRow()-1);
        }
        m_player->setMedia(QUrl::fromLocalFile(fileNames.at(ui->list->currentRow())));//加载媒体
        ui->pushButton_pause->setIcon(QIcon(":/icon/images/pause.png"));
        m_player->play();
        timer->start();
    }
}

void MainWindow::on_pushButton_pause_clicked()//播放按钮
{
    if(fileCount)
    {
        switch(m_player->state())//播放器状态
        {
        case QMediaPlayer::StoppedState:ui->horizontalSlider->setValue(0);
            m_player->play(); timer->start();ui->pushButton_pause->setIcon(QIcon(":/icon/images/pause.png"));break;
        case QMediaPlayer::PausedState:  m_player->play(); timer->start();ui->pushButton_pause->setIcon(QIcon(":/icon/images/pause.png"));break;
        case QMediaPlayer::PlayingState: m_player->pause();timer->stop(); ui->pushButton_pause->setIcon(QIcon(":/icon/images/stop.png"));break;
        default:
            ui->horizontalSlider->setValue(0);
            m_player->play(); timer->start();ui->pushButton_pause->setIcon(QIcon(":/icon/images/pause.png"));
        }
    }
}

void MainWindow::on_pushButton_next_clicked()//下一个按钮
{
    m_player->stop();
    ui->horizontalSlider->setValue(0);
    if(fileCount)
    {
        if(ui->list->currentRow()==ui->list->count()-1)//当前在第一个
        {
           ui->list->setCurrentRow(0);//设置当前项为最后一项
        }
         else
        {
           ui->list->setCurrentRow(ui->list->currentRow()+1);
        }
        m_player->setMedia(QUrl::fromLocalFile(fileNames.at(ui->list->currentRow())));
        ui->pushButton_pause->setIcon(QIcon(":/icon/images/pause.png"));
        m_player->play();
        timer->start();
    }
}

void MainWindow::on_time_out()//时间显示
{
    qint64 totalTime=m_player->duration();   //总时长 ms
    qint64 currentTime=m_player->position(); //当前时间 ms
    ui->horizontalSlider->setValue(currentTime*maxValue*1.0/totalTime);
    ui->label->setText(QString::number(currentTime/1000/60%60).rightJustified(2,'0')+":"+QString::number(currentTime/1000%60).rightJustified(2,'0')+"/"+
                       QString::number(totalTime/1000/60%60).rightJustified(2,'0')+":"+QString::number(totalTime/1000%60).rightJustified(2,'0'));

    if(totalTime==currentTime)//播放完
    {
        timer->stop();
        ui->pushButton_pause->setIcon(QIcon(":/icon/images/stop.png"));
        ui->horizontalSlider->setValue(0);
    }
}

void MainWindow::on_horizontalSlider_sliderReleased()//调进度
{
    //决定当前视频的播放位置（ms）
    m_player->setPosition(ui->horizontalSlider->value()*m_player->duration()/maxValue);
}

void MainWindow::on_volum_valueChanged(int value)//调音量
{
    m_player->setVolume(value);
    ui->label_volum->setText(QString::number(value)+"%");
}

void MainWindow::on_list_doubleClicked(const QModelIndex &index)//列表双击
{
    m_player->stop();
    m_player->setMedia(QUrl::fromLocalFile(fileNames.at(ui->list->currentRow())));
    ui->pushButton_pause->setIcon(QIcon(":/icon/images/pause.png"));
    ui->horizontalSlider->setValue(0);
    m_player->play();
    timer->start();
}

void MainWindow::on_rate_valueChanged(int value)//调速
{
    m_player->setPlaybackRate(value*0.2);//播放速率；1是正常速率，大于1加快播放
    ui->label_play_rate->setText(QString::number(value*0.2,'f',1)+"倍");
}
