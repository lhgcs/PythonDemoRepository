/********************************************************************************
** Form generated from reading ui file 'openboxdialog.ui'
**
** Created: Thu Aug 18 07:10:41 2016
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_OPENBOXDIALOG_H
#define UI_OPENBOXDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_OpenBoxDialog
{
public:
    QSpinBox *spinBoxNo;
    QSpinBox *spinBardNo;
    QLabel *label;
    QLabel *label_2;
    QPushButton *OpenButton;
    QPushButton *ExitButton;

    void setupUi(QDialog *OpenBoxDialog)
    {
        if (OpenBoxDialog->objectName().isEmpty())
            OpenBoxDialog->setObjectName(QString::fromUtf8("OpenBoxDialog"));
        OpenBoxDialog->resize(713, 561);
        OpenBoxDialog->setStyleSheet(QString::fromUtf8("background-color: rgb(43, 125, 173);"));
        spinBoxNo = new QSpinBox(OpenBoxDialog);
        spinBoxNo->setObjectName(QString::fromUtf8("spinBoxNo"));
        spinBoxNo->setGeometry(QRect(370, 120, 161, 51));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        spinBoxNo->setFont(font);
        spinBoxNo->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        spinBardNo = new QSpinBox(OpenBoxDialog);
        spinBardNo->setObjectName(QString::fromUtf8("spinBardNo"));
        spinBardNo->setGeometry(QRect(370, 50, 161, 51));
        spinBardNo->setFont(font);
        spinBardNo->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label = new QLabel(OpenBoxDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(180, 60, 121, 41));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_2 = new QLabel(OpenBoxDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(190, 140, 111, 31));
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        OpenButton = new QPushButton(OpenBoxDialog);
        OpenButton->setObjectName(QString::fromUtf8("OpenButton"));
        OpenButton->setGeometry(QRect(160, 190, 162, 49));
        OpenButton->setFont(font1);
        ExitButton = new QPushButton(OpenBoxDialog);
        ExitButton->setObjectName(QString::fromUtf8("ExitButton"));
        ExitButton->setGeometry(QRect(390, 190, 162, 49));
        ExitButton->setFont(font1);

        retranslateUi(OpenBoxDialog);
        QObject::connect(ExitButton, SIGNAL(clicked()), OpenBoxDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(OpenBoxDialog);
    } // setupUi

    void retranslateUi(QDialog *OpenBoxDialog)
    {
        OpenBoxDialog->setWindowTitle(QApplication::translate("OpenBoxDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("OpenBoxDialog", "\351\224\201\346\216\247\346\235\277\347\274\226\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("OpenBoxDialog", "\347\256\261\345\255\220\347\274\226\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        OpenButton->setText(QApplication::translate("OpenBoxDialog", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
        ExitButton->setText(QApplication::translate("OpenBoxDialog", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(OpenBoxDialog);
    } // retranslateUi

};

namespace Ui {
    class OpenBoxDialog: public Ui_OpenBoxDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENBOXDIALOG_H
