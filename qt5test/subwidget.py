
from PyQt5.QtWidgets import QWidget,QLabel,QVBoxLayout,QPushButton,QMessageBox
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtGui import QPixmap

class Widget(QWidget):
    mycloseSignal = pyqtSignal()

    def __init__(self):
        super().__init__()

        self.resize(500,500)

        label=QLabel('1,jpg')
        label.setPixmap(QPixmap('1.jpg'))
        btn = QPushButton('退出')
        vl = QVBoxLayout()
        vl.addWidget(label)
        vl.addWidget(btn)
        self.setLayout(vl)

        btn.clicked.connect(self.btnClicked)

    def btnClicked(self):
        print('subwidget btn clicked')
        self.mycloseSignal.emit()

    def closeEvent(self, event):
        reply = QMessageBox.question('Message', "is quit?", QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
        if reply == QMessageBox.Yes:
            event.accept()
        else:
            event.ignore()