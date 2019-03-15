#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QPainter>

class SettingDialog : public QWidget
{
    Q_OBJECT
public:
    SettingDialog(QWidget *parent=0);
    ~SettingDialog(){
        delete label;
        delete btnOk;
        delete btnCancel;
        delete comboBox_price;
        delete comboBox_num;
        delete fl;
    }
    void set_context(const QString goods,const QString price,const QString num){
        label->setText(goods);
        comboBox_price->setCurrentText(price);
        comboBox_num->setCurrentText(num);
        this->show();
    }
private:
    QLabel *label;
    QPushButton *btnOk,*btnCancel;
    QComboBox *comboBox_price,*comboBox_num;
    QFormLayout *fl;
signals:
    void up_data(QString prive,QString num);
private slots:
    void on_click_ok(){
        emit up_data(comboBox_price->currentText(),comboBox_num->currentText());
        this->close();
    }
    void on_click_cancel(){
        this->close();
    }
protected:
    void paintEvent(QPaintEvent *){
        QStyleOption  opt;
        opt.init(this);
        QPainter  p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // SETTINGDIALOG_H
