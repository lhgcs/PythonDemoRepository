#ifndef NUMINPUTDIALOG_H
#define NUMINPUTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QSignalMapper>
#include <QGridLayout>

class NumInputDialog : public QDialog
{
    Q_OBJECT
public:
    NumInputDialog(QWidget *parent = 0);
    ~NumInputDialog(){
        delete []button;
        delete buttonEnter;
        delete buttonBackspace;
        delete signalMapper;
        delete gl;
    }
signals:
     void num(int value);
private slots:
     void keyPress(int value){
         emit num(value);
     }
private:
    QPushButton *button;
    QPushButton *buttonEnter;
    QPushButton *buttonBackspace;
    QSignalMapper *signalMapper;
    QGridLayout *gl;
};

#endif // NUMINPUTDIALOG_H
