#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QtNetwork>
#include "mydialog.h"

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QtWebKit/QtWebKit>
#include <QtWebKit/QWebView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void DispPwd(QString c);
    QString pwd;

    QWebView *view;
    QTimer *viewTimer;

private slots:
    void on_Button1_clicked();
    void on_Button2_clicked();
    void on_Button3_clicked();
    void on_Button4_clicked();
    void on_Button5_clicked();
    void on_Button6_clicked();
    void on_Button7_clicked();
    void on_Button8_clicked();
    void on_Button9_clicked();
    void on_Button0_clicked();
    void on_ButtonCancel_clicked();
    void on_ButtonEnter_clicked();

    void replyFinished(QNetworkReply* reply1);
    void msgTimeout();

    void viewShow(bool ok);
    void viewNotShow();
    void viewTimerout();
    //void dateTimerOut();
    //void sysRestart();

    void on_LineEditPwd_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QTimer *msgtimer;
    myDialog *msgBox;

    //QTimer *dateTimer;
    bool loadflag;
    //char viewLoad;

    QNetworkAccessManager *manager;
};
#endif // MAINWINDOW_H
