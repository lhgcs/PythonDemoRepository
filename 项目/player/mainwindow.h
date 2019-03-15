#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_open_clicked();
    void on_pushButton_previous_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_next_clicked();

    void on_time_out();
    void on_horizontalSlider_sliderReleased();
    void on_volum_valueChanged(int value);
    void on_list_doubleClicked(const QModelIndex &index);
    void on_rate_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    QVideoWidget *videow;
    QMediaPlayer *m_player;

    QTimer * timer;     //定时刷新进度条、当前时间
    int maxValue = 1000;//设置进度条的最大值

    QStringList fileNames;
    int fileCount;
};

#endif // MAINWINDOW_H
