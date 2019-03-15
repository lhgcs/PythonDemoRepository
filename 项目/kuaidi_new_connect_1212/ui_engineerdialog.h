/********************************************************************************
** Form generated from reading ui file 'engineerdialog.ui'
**
** Created: Thu Aug 18 07:10:41 2016
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ENGINEERDIALOG_H
#define UI_ENGINEERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_EngineerDialog
{
public:
    QPushButton *RegisterButton;
    QPushButton *OpenButton;
    QPushButton *SystemButton;
    QPushButton *ExitButton;

    void setupUi(QDialog *EngineerDialog)
    {
        if (EngineerDialog->objectName().isEmpty())
            EngineerDialog->setObjectName(QString::fromUtf8("EngineerDialog"));
        EngineerDialog->resize(864, 450);
        EngineerDialog->setStyleSheet(QString::fromUtf8("background-color: rgb(43, 125, 173);"));
        RegisterButton = new QPushButton(EngineerDialog);
        RegisterButton->setObjectName(QString::fromUtf8("RegisterButton"));
        RegisterButton->setGeometry(QRect(70, 110, 277, 59));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        RegisterButton->setFont(font);
        OpenButton = new QPushButton(EngineerDialog);
        OpenButton->setObjectName(QString::fromUtf8("OpenButton"));
        OpenButton->setGeometry(QRect(380, 110, 277, 59));
        OpenButton->setFont(font);
        SystemButton = new QPushButton(EngineerDialog);
        SystemButton->setObjectName(QString::fromUtf8("SystemButton"));
        SystemButton->setGeometry(QRect(70, 190, 277, 59));
        SystemButton->setFont(font);
        ExitButton = new QPushButton(EngineerDialog);
        ExitButton->setObjectName(QString::fromUtf8("ExitButton"));
        ExitButton->setGeometry(QRect(380, 190, 277, 59));
        ExitButton->setFont(font);

        retranslateUi(EngineerDialog);
        QObject::connect(ExitButton, SIGNAL(clicked()), EngineerDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(EngineerDialog);
    } // setupUi

    void retranslateUi(QDialog *EngineerDialog)
    {
        EngineerDialog->setWindowTitle(QApplication::translate("EngineerDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        RegisterButton->setText(QApplication::translate("EngineerDialog", "\346\234\272\346\237\234\346\263\250\345\206\214", 0, QApplication::UnicodeUTF8));
        OpenButton->setText(QApplication::translate("EngineerDialog", "\346\211\213\345\212\250\345\274\200\346\237\234", 0, QApplication::UnicodeUTF8));
        SystemButton->setText(QApplication::translate("EngineerDialog", "\347\263\273\347\273\237\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        ExitButton->setText(QApplication::translate("EngineerDialog", "\351\200\200    \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(EngineerDialog);
    } // retranslateUi

};

namespace Ui {
    class EngineerDialog: public Ui_EngineerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENGINEERDIALOG_H
