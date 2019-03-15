#ifndef OPENBOXDIALOG_H
#define OPENBOXDIALOG_H

#include <QDialog>

namespace Ui {
class OpenBoxDialog;
}

class OpenBoxDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OpenBoxDialog(QWidget *parent = 0);
    ~OpenBoxDialog();
    
private slots:
    void on_OpenButton_clicked();

private:
    Ui::OpenBoxDialog *ui;
};

#endif // OPENBOXDIALOG_H
