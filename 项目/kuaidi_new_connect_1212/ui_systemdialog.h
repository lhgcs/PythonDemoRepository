/********************************************************************************
** Form generated from reading ui file 'systemdialog.ui'
**
** Created: Thu Aug 18 07:10:41 2016
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SYSTEMDIALOG_H
#define UI_SYSTEMDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_SystemDialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QTextEdit *textEditRemoteIP;
    QTextEdit *textEditPort;
    QTextEdit *textEditLocalIP;
    QTextEdit *textEditGateWay;
    QTextEdit *textEditMAC;
    QTextEdit *textEditBoardCount;
    QTextEdit *textEditLockCount;
    QLabel *label_7;
    QPushButton *SaveButton;
    QPushButton *ExitButton;

    void setupUi(QDialog *SystemDialog)
    {
        if (SystemDialog->objectName().isEmpty())
            SystemDialog->setObjectName(QString::fromUtf8("SystemDialog"));
        SystemDialog->resize(732, 563);
        SystemDialog->setStyleSheet(QString::fromUtf8("background-color: rgb(43, 125, 173);"));
        label = new QLabel(SystemDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, 1, 91, 31));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_2 = new QLabel(SystemDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(190, 41, 91, 31));
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_3 = new QLabel(SystemDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(190, 81, 91, 31));
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_4 = new QLabel(SystemDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(190, 121, 81, 31));
        label_4->setFont(font);
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_5 = new QLabel(SystemDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(190, 161, 81, 31));
        label_5->setFont(font);
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_6 = new QLabel(SystemDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(190, 201, 81, 31));
        label_6->setFont(font);
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        textEditRemoteIP = new QTextEdit(SystemDialog);
        textEditRemoteIP->setObjectName(QString::fromUtf8("textEditRemoteIP"));
        textEditRemoteIP->setGeometry(QRect(310, 0, 171, 35));
        QFont font1;
        font1.setFamily(QString::fromUtf8("DejaVu Sans"));
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setWeight(50);
        textEditRemoteIP->setFont(font1);
        textEditRemoteIP->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        textEditPort = new QTextEdit(SystemDialog);
        textEditPort->setObjectName(QString::fromUtf8("textEditPort"));
        textEditPort->setGeometry(QRect(310, 40, 91, 35));
        textEditPort->setFont(font1);
        textEditPort->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        textEditLocalIP = new QTextEdit(SystemDialog);
        textEditLocalIP->setObjectName(QString::fromUtf8("textEditLocalIP"));
        textEditLocalIP->setGeometry(QRect(310, 80, 171, 35));
        QFont font2;
        font2.setFamily(QString::fromUtf8("DejaVu Sans"));
        font2.setPointSize(12);
        textEditLocalIP->setFont(font2);
        textEditLocalIP->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        textEditGateWay = new QTextEdit(SystemDialog);
        textEditGateWay->setObjectName(QString::fromUtf8("textEditGateWay"));
        textEditGateWay->setGeometry(QRect(310, 120, 171, 35));
        textEditGateWay->setFont(font2);
        textEditGateWay->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        textEditMAC = new QTextEdit(SystemDialog);
        textEditMAC->setObjectName(QString::fromUtf8("textEditMAC"));
        textEditMAC->setGeometry(QRect(310, 160, 251, 35));
        textEditMAC->setFont(font2);
        textEditMAC->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        textEditBoardCount = new QTextEdit(SystemDialog);
        textEditBoardCount->setObjectName(QString::fromUtf8("textEditBoardCount"));
        textEditBoardCount->setGeometry(QRect(310, 200, 40, 35));
        textEditBoardCount->setFont(font2);
        textEditBoardCount->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        textEditLockCount = new QTextEdit(SystemDialog);
        textEditLockCount->setObjectName(QString::fromUtf8("textEditLockCount"));
        textEditLockCount->setGeometry(QRect(520, 200, 40, 35));
        textEditLockCount->setFont(font2);
        textEditLockCount->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_7 = new QLabel(SystemDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(420, 201, 81, 31));
        label_7->setFont(font);
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        SaveButton = new QPushButton(SystemDialog);
        SaveButton->setObjectName(QString::fromUtf8("SaveButton"));
        SaveButton->setGeometry(QRect(190, 260, 162, 41));
        SaveButton->setFont(font);
        ExitButton = new QPushButton(SystemDialog);
        ExitButton->setObjectName(QString::fromUtf8("ExitButton"));
        ExitButton->setGeometry(QRect(390, 260, 162, 41));
        ExitButton->setFont(font);

        retranslateUi(SystemDialog);
        QObject::connect(ExitButton, SIGNAL(clicked()), SystemDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(SystemDialog);
    } // setupUi

    void retranslateUi(QDialog *SystemDialog)
    {
        SystemDialog->setWindowTitle(QApplication::translate("SystemDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SystemDialog", "\346\234\215\345\212\241\345\231\250IP:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SystemDialog", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SystemDialog", "\346\234\254\346\234\272IP\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("SystemDialog", "\347\275\221\345\205\263\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("SystemDialog", "\347\211\251\347\220\206\345\234\260\345\235\200\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("SystemDialog", "\351\224\201\346\211\243\346\235\277\346\225\260\351\207\217\357\274\232", 0, QApplication::UnicodeUTF8));
        textEditRemoteIP->setHtml(QApplication::translate("SystemDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:8pt;\">192.168.1.15</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        textEditPort->setHtml(QApplication::translate("SystemDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:8pt;\">80</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        textEditLocalIP->setHtml(QApplication::translate("SystemDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:8pt;\">192.168.1.25</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        textEditGateWay->setHtml(QApplication::translate("SystemDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:8pt;\">192.168.1.1</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        textEditMAC->setHtml(QApplication::translate("SystemDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:8pt;\">10:23:45:67:89:AB</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        textEditBoardCount->setHtml(QApplication::translate("SystemDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:8pt;\">4</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        textEditLockCount->setHtml(QApplication::translate("SystemDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:8pt;\">5</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("SystemDialog", "\351\224\201\346\225\260\351\207\217\357\274\232", 0, QApplication::UnicodeUTF8));
        SaveButton->setText(QApplication::translate("SystemDialog", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
        ExitButton->setText(QApplication::translate("SystemDialog", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SystemDialog);
    } // retranslateUi

};

namespace Ui {
    class SystemDialog: public Ui_SystemDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEMDIALOG_H
