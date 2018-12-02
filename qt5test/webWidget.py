from PyQt5.QtWidgets import QWidget,QLabel,QVBoxLayout,QPushButton,QMessageBox
from PyQt5.QtCore import pyqtSignal,QTimer
from PyQt5.QtGui import QPixmap,QPainter, QColor
from PyQt5.QtCore import QCoreApplication,pyqtSignal,QObject

#from PyQt5.QtWebEngineWidgets import QWebEngineView

from PyQt5.QtCore import QUrl,QEventLoop,QByteArray
from PyQt5.QtWebEngineWidgets import QWebEngineView,QWebEnginePage
from PyQt5.QtNetwork import QNetworkAccessManager,QNetworkRequest,QSslConfiguration,QSslSocket,QSsl
#from PyQt5.QtWebKitWidgets import *
#在 PyQt 5.6(+) 版本中, 新增 QtWebEngineWidgets 代替QtWebKitWidgets
from PyQt5.QtWebEngineWidgets import *

class WebWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.manager=QNetworkAccessManager()
        self.reply = None
        self.get_html()

    def get_html(self):
        strUrl='www.baidu.com'
        request=QNetworkRequest()
        request.setUrl(QUrl(strUrl)); # 加载网页

        config=QSslConfiguration
        #config.setPeerVerifyMode(QSslSocket.VerifyNone)
        #config.setProtocol(QSsl.TlsV1SslV3) # 设置ssl

        #request.setSslConfiguration(config);
        self.reply = self.manager.get(request);

        loop=QByteArray()
        loop=QEventLoop()# 启动循环
        self.reply.finished.connect(loop.quit)#读取完成后退出循环
        loop.exec()
        data = self.reply.readAll() # 接收源码
        UrlPage =QWebEnginePage()

#        UrlPage.toHtml(data)
        #UrlPage=QWebPage()
        #UrlPage.setHtml(data)
        #UrlPage.mainFrame().setHtml(data)

        UrlFrame= UrlPage.currentFrame()
        #QWebFrame

        #QWebElement
        doc = UrlFrame.documentElement();
        #id="user-content-ss-or-ssr免费账号长期更新"
        #QWebElement
        ID = doc.findFirst("#wiki-body");#ID =  过滤节点

        #QWebElementCollection
        tmp = ID.findAll("p");
        #html不熟，下面的用字符串处理
        for i in tmp.count():
            print(tmp.at(i).toPlainText())
            if tmp.at(i).toPlainText().startsWith("服务器"):
                print(tmp.at(i).toPlainText())

    #writeFile(strData)








