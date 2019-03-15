#ifndef PICKUPCODE_H
#define PICKUPCODE_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QGridLayout>

class PickUpCode : public QWidget
{
    Q_OBJECT
public:
    explicit PickUpCode(QWidget *parent = 0);
    ~PickUpCode(){
        delete []button;
        delete group;
        delete edit;
        delete gl;
    }
signals:
     void num(QString str);
private slots:
     void button_slot(int value){
         switch (value) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                edit->setText(edit->text().append(QString::number(value)));
                break;
            case 10:
                edit->setText(edit->text().append("."));
                break;
            case 11:{
                QString str=edit->text();
                if(str.length()>1){
                    edit->setText(str.left(str.length()-1));
                }else{
                    edit->clear();
                }
            }
                break;
            case 12:
                edit->clear();
                break;
            case 13:
                emit num(edit->text());
                edit->clear();
                break;
            default:break;
         }
     }

private:
    QPushButton *button;
    QButtonGroup *group;
    QLineEdit *edit;
    QGridLayout *gl;
};

#endif // PICKUPCODE_H
