#ifndef SYSTEMDIALOG_H
#define SYSTEMDIALOG_H

#include <QDialog>

namespace Ui {
class SystemDialog;
}

class SystemDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SystemDialog(QWidget *parent = 0);
    ~SystemDialog();
    
private slots:
    void on_SaveButton_clicked();

private:
    Ui::SystemDialog *ui;
};

#endif // SYSTEMDIALOG_H
