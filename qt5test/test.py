#!/usr/bin/python3
#-*- coding:utf-8 -*-
import sys

from PyQt5.QtWebEngineWidgets import QWebEngineView
from PyQt5.QtCore import QUrl

from PyQt5.QtWidgets import QApplication,QMainWindow,QToolTip,QPushButton,QMessageBox,QDesktopWidget
from PyQt5.QtWidgets import qApp,QAction,QLabel
from PyQt5.QtGui import QFont,QIcon
from PyQt5.QtCore import QCoreApplication,pyqtSignal,QObject

import MyMainWidget
import subwidget
import myWebView
from loadingWidget import LoadingWidget
from webWidget import WebWidget

if __name__ == '__main__':
    app = QApplication(sys.argv)  #应用程序对象

    #mywidget = MyMainWidget.MyMainWidget()#模块名.函数名
    #mywidget.show()

    # view = QWebEngineView()
    # view.load(QUrl("http://www.baidu.com"))
    # view.show()

    w=WebWidget()
    w.show()

    # loadingWidget=LoadingWidget()
    # loadingWidget.show()



    sys.exit(app.exec_())#主窗口销毁或者exit调用时退出主线程循环
