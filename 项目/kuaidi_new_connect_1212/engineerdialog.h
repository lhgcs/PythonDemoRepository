#ifndef ENGINEERDIALOG_H
#define ENGINEERDIALOG_H

#include <QDialog>

namespace Ui {
class EngineerDialog;
}

class EngineerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EngineerDialog(QWidget *parent = 0);
    ~EngineerDialog();
    
private slots:
    void on_OpenButton_clicked();

    void on_RegisterButton_clicked();

    void on_SystemButton_clicked();

private:
    Ui::EngineerDialog *ui;
};

#endif // ENGINEERDIALOG_H
