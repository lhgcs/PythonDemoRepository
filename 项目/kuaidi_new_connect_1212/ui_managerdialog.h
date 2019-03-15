/********************************************************************************
** Form generated from reading ui file 'managerdialog.ui'
**
** Created: Thu Aug 18 07:10:41 2016
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MANAGERDIALOG_H
#define UI_MANAGERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ManagerDialog
{
public:
    QPushButton *Button4;
    QPushButton *Button3;
    QPushButton *ButtonReturn;
    QPushButton *Button8;
    QPushButton *ButtonPageDown;
    QPushButton *ButtonCancel;
    QPushButton *Button6;
    QPushButton *ButtonPoint;
    QPushButton *Button5;
    QPushButton *Button7;
    QPushButton *ButtonPageUp;
    QPushButton *Button1;
    QPushButton *Button2;
    QPushButton *Button9;
    QPushButton *ButtonEnter;
    QPushButton *Button0;
    QPushButton *ButtonExit;
    QLineEdit *LineEditPwd;

    void setupUi(QDialog *ManagerDialog)
    {
        if (ManagerDialog->objectName().isEmpty())
            ManagerDialog->setObjectName(QString::fromUtf8("ManagerDialog"));
        ManagerDialog->resize(701, 665);
        ManagerDialog->setStyleSheet(QString::fromUtf8("background-color: rgb(43, 125, 173);"));
        Button4 = new QPushButton(ManagerDialog);
        Button4->setObjectName(QString::fromUtf8("Button4"));
        Button4->setGeometry(QRect(250, 320, 82, 65));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        Button4->setFont(font);
        Button4->setStyleSheet(QString::fromUtf8(""));
        Button3 = new QPushButton(ManagerDialog);
        Button3->setObjectName(QString::fromUtf8("Button3"));
        Button3->setGeometry(QRect(450, 240, 82, 65));
        Button3->setFont(font);
        ButtonReturn = new QPushButton(ManagerDialog);
        ButtonReturn->setObjectName(QString::fromUtf8("ButtonReturn"));
        ButtonReturn->setGeometry(QRect(551, 240, 122, 65));
        ButtonReturn->setFont(font);
        Button8 = new QPushButton(ManagerDialog);
        Button8->setObjectName(QString::fromUtf8("Button8"));
        Button8->setGeometry(QRect(351, 400, 82, 65));
        Button8->setFont(font);
        ButtonPageDown = new QPushButton(ManagerDialog);
        ButtonPageDown->setObjectName(QString::fromUtf8("ButtonPageDown"));
        ButtonPageDown->setGeometry(QRect(552, 400, 122, 65));
        ButtonPageDown->setFont(font);
        ButtonCancel = new QPushButton(ManagerDialog);
        ButtonCancel->setObjectName(QString::fromUtf8("ButtonCancel"));
        ButtonCancel->setGeometry(QRect(251, 480, 82, 65));
        ButtonCancel->setFont(font);
        ButtonCancel->setStyleSheet(QString::fromUtf8(""));
        Button6 = new QPushButton(ManagerDialog);
        Button6->setObjectName(QString::fromUtf8("Button6"));
        Button6->setGeometry(QRect(450, 320, 82, 65));
        Button6->setFont(font);
        ButtonPoint = new QPushButton(ManagerDialog);
        ButtonPoint->setObjectName(QString::fromUtf8("ButtonPoint"));
        ButtonPoint->setGeometry(QRect(451, 480, 82, 65));
        ButtonPoint->setFont(font);
        Button5 = new QPushButton(ManagerDialog);
        Button5->setObjectName(QString::fromUtf8("Button5"));
        Button5->setGeometry(QRect(350, 320, 82, 65));
        Button5->setFont(font);
        Button7 = new QPushButton(ManagerDialog);
        Button7->setObjectName(QString::fromUtf8("Button7"));
        Button7->setGeometry(QRect(251, 400, 82, 65));
        Button7->setFont(font);
        Button7->setStyleSheet(QString::fromUtf8(""));
        ButtonPageUp = new QPushButton(ManagerDialog);
        ButtonPageUp->setObjectName(QString::fromUtf8("ButtonPageUp"));
        ButtonPageUp->setGeometry(QRect(551, 320, 122, 65));
        ButtonPageUp->setFont(font);
        Button1 = new QPushButton(ManagerDialog);
        Button1->setObjectName(QString::fromUtf8("Button1"));
        Button1->setGeometry(QRect(250, 240, 82, 65));
        Button1->setFont(font);
        Button1->setStyleSheet(QString::fromUtf8(""));
        Button2 = new QPushButton(ManagerDialog);
        Button2->setObjectName(QString::fromUtf8("Button2"));
        Button2->setGeometry(QRect(350, 240, 82, 65));
        Button2->setFont(font);
        Button9 = new QPushButton(ManagerDialog);
        Button9->setObjectName(QString::fromUtf8("Button9"));
        Button9->setGeometry(QRect(451, 400, 82, 65));
        Button9->setFont(font);
        ButtonEnter = new QPushButton(ManagerDialog);
        ButtonEnter->setObjectName(QString::fromUtf8("ButtonEnter"));
        ButtonEnter->setGeometry(QRect(552, 480, 122, 65));
        ButtonEnter->setFont(font);
        Button0 = new QPushButton(ManagerDialog);
        Button0->setObjectName(QString::fromUtf8("Button0"));
        Button0->setGeometry(QRect(350, 480, 82, 65));
        Button0->setFont(font);
        ButtonExit = new QPushButton(ManagerDialog);
        ButtonExit->setObjectName(QString::fromUtf8("ButtonExit"));
        ButtonExit->setGeometry(QRect(550, 560, 121, 65));
        ButtonExit->setFont(font);
        LineEditPwd = new QLineEdit(ManagerDialog);
        LineEditPwd->setObjectName(QString::fromUtf8("LineEditPwd"));
        LineEditPwd->setGeometry(QRect(250, 160, 421, 60));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        LineEditPwd->setFont(font1);

        retranslateUi(ManagerDialog);
        QObject::connect(ButtonExit, SIGNAL(clicked()), ManagerDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(ManagerDialog);
    } // setupUi

    void retranslateUi(QDialog *ManagerDialog)
    {
        ManagerDialog->setWindowTitle(QApplication::translate("ManagerDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        Button4->setText(QApplication::translate("ManagerDialog", "4", 0, QApplication::UnicodeUTF8));
        Button3->setText(QApplication::translate("ManagerDialog", "3", 0, QApplication::UnicodeUTF8));
        ButtonReturn->setText(QApplication::translate("ManagerDialog", "\350\277\224\345\233\236", 0, QApplication::UnicodeUTF8));
        Button8->setText(QApplication::translate("ManagerDialog", "8", 0, QApplication::UnicodeUTF8));
        ButtonPageDown->setText(QApplication::translate("ManagerDialog", "\344\270\213\347\277\273", 0, QApplication::UnicodeUTF8));
        ButtonCancel->setText(QApplication::translate("ManagerDialog", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Button6->setText(QApplication::translate("ManagerDialog", "6", 0, QApplication::UnicodeUTF8));
        ButtonPoint->setText(QApplication::translate("ManagerDialog", ".", 0, QApplication::UnicodeUTF8));
        Button5->setText(QApplication::translate("ManagerDialog", "5", 0, QApplication::UnicodeUTF8));
        Button7->setText(QApplication::translate("ManagerDialog", "7", 0, QApplication::UnicodeUTF8));
        ButtonPageUp->setText(QApplication::translate("ManagerDialog", "\344\270\212\347\277\273", 0, QApplication::UnicodeUTF8));
        Button1->setText(QApplication::translate("ManagerDialog", "1", 0, QApplication::UnicodeUTF8));
        Button2->setText(QApplication::translate("ManagerDialog", "2", 0, QApplication::UnicodeUTF8));
        Button9->setText(QApplication::translate("ManagerDialog", "9", 0, QApplication::UnicodeUTF8));
        ButtonEnter->setText(QApplication::translate("ManagerDialog", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        Button0->setText(QApplication::translate("ManagerDialog", "0", 0, QApplication::UnicodeUTF8));
        ButtonExit->setText(QApplication::translate("ManagerDialog", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ManagerDialog);
    } // retranslateUi

};

namespace Ui {
    class ManagerDialog: public Ui_ManagerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGERDIALOG_H
