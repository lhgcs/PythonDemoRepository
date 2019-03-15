#include <QThread>
#include "mainwindow.h"
#include "mydialog.h"
class UartQrcode : public QThread
{
    Q_OBJECT

public:
    UartQrcode();
    ~UartQrcode();
	int openserialport();
	QNetworkAccessManager *qrcode_manager;
	void ReplyQrcode(QNetworkReply* reply);
	void PostQrcode(QString qrcode);
	int ParseRecvData(unsigned char *data, int len);
protected:
    void run();
private:
    QTimer *msgtimer;
    myDialog *msgBox;

private slots:
    void msgTimeout();
signals:
};
