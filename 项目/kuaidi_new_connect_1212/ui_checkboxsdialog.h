/********************************************************************************
** Form generated from reading ui file 'checkboxsdialog.ui'
**
** Created: Thu Aug 18 07:10:41 2016
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CHECKBOXSDIALOG_H
#define UI_CHECKBOXSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CheckBoxsDialog
{
public:
    QPushButton *OpenAllButton;
    QPushButton *ChekedButton;
    QPushButton *AuthenButton;
    QPushButton *ExitButton;

    void setupUi(QDialog *CheckBoxsDialog)
    {
        if (CheckBoxsDialog->objectName().isEmpty())
            CheckBoxsDialog->setObjectName(QString::fromUtf8("CheckBoxsDialog"));
        CheckBoxsDialog->resize(801, 496);
        CheckBoxsDialog->setStyleSheet(QString::fromUtf8("background-color: rgb(43, 125, 173);"));
        OpenAllButton = new QPushButton(CheckBoxsDialog);
        OpenAllButton->setObjectName(QString::fromUtf8("OpenAllButton"));
        OpenAllButton->setGeometry(QRect(270, 40, 277, 51));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        OpenAllButton->setFont(font);
        ChekedButton = new QPushButton(CheckBoxsDialog);
        ChekedButton->setObjectName(QString::fromUtf8("ChekedButton"));
        ChekedButton->setGeometry(QRect(270, 100, 277, 51));
        ChekedButton->setFont(font);
        AuthenButton = new QPushButton(CheckBoxsDialog);
        AuthenButton->setObjectName(QString::fromUtf8("AuthenButton"));
        AuthenButton->setGeometry(QRect(270, 160, 277, 51));
        AuthenButton->setFont(font);
        ExitButton = new QPushButton(CheckBoxsDialog);
        ExitButton->setObjectName(QString::fromUtf8("ExitButton"));
        ExitButton->setGeometry(QRect(270, 220, 277, 51));
        ExitButton->setFont(font);

        retranslateUi(CheckBoxsDialog);
        QObject::connect(ExitButton, SIGNAL(clicked()), CheckBoxsDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(CheckBoxsDialog);
    } // setupUi

    void retranslateUi(QDialog *CheckBoxsDialog)
    {
        CheckBoxsDialog->setWindowTitle(QApplication::translate("CheckBoxsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        OpenAllButton->setText(QApplication::translate("CheckBoxsDialog", "\346\211\223\345\274\200\345\205\250\351\203\250\346\237\234\345\255\220", 0, QApplication::UnicodeUTF8));
        ChekedButton->setText(QApplication::translate("CheckBoxsDialog", "\345\267\262\345\205\250\351\203\250\346\211\223\345\274\200\357\274\214\350\277\233\350\241\214\346\243\200\346\265\213", 0, QApplication::UnicodeUTF8));
        AuthenButton->setText(QApplication::translate("CheckBoxsDialog", "\347\241\256\350\256\244\350\256\244\350\257\201", 0, QApplication::UnicodeUTF8));
        ExitButton->setText(QApplication::translate("CheckBoxsDialog", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CheckBoxsDialog);
    } // retranslateUi

};

namespace Ui {
    class CheckBoxsDialog: public Ui_CheckBoxsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHECKBOXSDIALOG_H
