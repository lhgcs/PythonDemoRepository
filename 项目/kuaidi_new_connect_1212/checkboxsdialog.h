#ifndef CHECKBOXSDIALOG_H
#define CHECKBOXSDIALOG_H

#include <QDialog>
#include <QtNetwork/QtNetwork>

namespace Ui {
class CheckBoxsDialog;
}

class CheckBoxsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CheckBoxsDialog(QWidget *parent = 0);
    ~CheckBoxsDialog();
    
private slots:
    void on_AuthenButton_clicked();

    void replyFinished(QNetworkReply* reply);

    void replyRegister(QNetworkReply* reply);

    void on_OpenAllButton_clicked();

    void on_ChekedButton_clicked();

private:
    Ui::CheckBoxsDialog *ui;
};

#endif // CHECKBOXSDIALOG_H
