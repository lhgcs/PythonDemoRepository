#ifndef MANAGERDIALOG_H
#define MANAGERDIALOG_H

#include <QDialog>

namespace Ui {
class ManagerDialog;
}

class ManagerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ManagerDialog(QWidget *parent = 0);
    ~ManagerDialog();
    void DispPwd(QString c);
    QString pwd;
    
private slots:
    void on_ButtonEnter_clicked();

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

    void on_ButtonPoint_clicked();

private:
    Ui::ManagerDialog *ui;
};

#endif // MANAGERDIALOG_H
