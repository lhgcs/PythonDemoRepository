#include <QThread>
#include "mainwindow.h"
#include <QMainWindow>
#include "mydialog.h"
class UartIC : public QThread
{
    Q_OBJECT

public:
    UartIC();
    ~UartIC();
	int openserialport();
    QNetworkAccessManager *ic_manager;
	void ReplyIC(QNetworkReply* reply);
	void PostIC(QString iccode);
	int ParseRecvData(unsigned char *data, int len);
    int SendStrToUart(int fd, int UartNo,  char *str);
protected:
    void run();
private:
    QTimer *msgtimer;
    myDialog *msgBox;

private slots:
    void msgTimeout();
signals:
};
