#ifndef LABELANDEDIT_H
#define LABELANDEDIT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

class LabelAndEdit : public QWidget
{
    Q_OBJECT
public:
    explicit LabelAndEdit(QWidget *parent = 0);
    ~LabelAndEdit(){
        delete label;
        delete lineEdit;
        delete hl;
    }

    void set_text(QString name,QString value){
        label->setText(name);
        lineEdit->setText(value);
    }

    void set_text(QString value){
        lineEdit->setText(value);
    }

    QString get_value(){
        return lineEdit->text();
    }

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QHBoxLayout *hl;
};

#endif // LABELANDEDIT_H
