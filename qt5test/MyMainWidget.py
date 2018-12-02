#!/usr/bin/python3
#-*- coding:utf-8 -*-

from PyQt5.QtWidgets import QApplication,QMainWindow,QToolTip,QPushButton,QMessageBox,QDesktopWidget
from PyQt5.QtWidgets import qApp,QAction,QLabel
from PyQt5.QtGui import QFont,QIcon
from PyQt5.QtCore import QCoreApplication,pyqtSignal,QObject,Qt
import main_widget
import subwidget

class MyMainWidget(QMainWindow):

    def __init__(self):
        super().__init__() #构造函数

        ui = main_widget.Ui_MainWindow()
        ui.setupUi(self)
        self.setWindowTitle('main')
        self.statusBar().showMessage('状态栏')
        self.setWindowIcon(QIcon('1.jpg'))
        self.mysub = subwidget.Widget()
        self.mysub.mycloseSignal.connect(self.my_sub_slot)
        QToolTip.setFont(QFont('SansSerif', 10))
        ui.pushButton.setToolTip("btn <b>1111</b>")
        ui.pushButton.clicked.connect(QCoreApplication.instance().quit)  # 信号和槽
        ui.pushButton_2.clicked.connect(self.buttonClicked)

        exitAction = QAction(QIcon("1.jpg"), '&Exit')
        exitAction.setShortcut('Ctrl+Q')
        exitAction.setStatusTip('Exit')
        exitAction.triggered.connect(qApp.quit)

        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(exitAction)

        toolBar = self.addToolBar('quit')
        toolBar.addAction(exitAction)

        #self.setCentralWidget(self.mysub)

    def buttonClicked(self):
        sender = self.sender()
        print('clicked slot')
        print(sender.text())
        self.mysub.show()
        self.statusBar().showMessage("打开新窗口")

    def my_sub_slot(self):
        print('sub slot')
        #self.mysub.close()
        self.mysub.hide()
        self.statusBar().showMessage("关闭新窗口")

    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.resize(300,200)
        self.move(qr.topLeft())

    def keyPressEvent(self,e):
        if e.key == Qt.key_Escape:
            self.close()
