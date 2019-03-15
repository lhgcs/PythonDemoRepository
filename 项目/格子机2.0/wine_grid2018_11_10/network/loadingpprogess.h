#ifndef LOADINGPPROGESS_H
#define LOADINGPPROGESS_H

#include <QProgressBar>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class LoadingpProgess :public QWidget
{
    Q_OBJECT

public:
    LoadingpProgess(QWidget *parent = NULL);
    void setLoadingItemInfo(QString info);
    void setProgessRange(int minimum, int maximum);
    void setProgessValue(int value);
    int  getMaxmun();

private:
    void initLayout();

    QLabel       *mLoadingItemInfo;
    QProgressBar *mProgressBar;
    QVBoxLayout *layout;
    int mMaximum;
    int mMinimum;

};

#endif // LOADINGPPROGESS_H
