#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QWidget>
#include <QLabel>
#include <QBasicTimer>
#include <QDateTime>
#include <QTimerEvent>

class StatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit StatusBar(QWidget *parent = 0);
    void show_time(){
        QDateTime time = QDateTime::currentDateTime();            //获取系统现在的时间
        labelTime->setText(time.toString("yyyy-MM-dd hh:mm"));//设置系统时间显示格式  :ss
    }

private:
    QBasicTimer timer;
    QLabel *labelTime;

//    bool try_network(){
//        QTcpSocket tcpClient;
//        tcpClient.abort();
//        tcpClient.connectToHost("115.239.211.112",80);//百度IP
//        if(tcpClient.waitForConnected(3000)){
//            return true;
//        }
//        return false;
//    }
protected:
    void timerEvent(QTimerEvent *event){
        if (event->timerId()==timer.timerId()) {
            show_time();
        } else {
            QWidget::timerEvent(event);
        }
    }
};

#endif // STATUSBAR_H
