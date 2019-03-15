#include "loadingpprogess.h"


LoadingpProgess::LoadingpProgess(QWidget *parent) : QWidget(parent)
{
    mLoadingItemInfo = new QLabel(this);
    mProgressBar = new QProgressBar(this);
    layout =new QVBoxLayout(this);
    initLayout();
}

void LoadingpProgess::setLoadingItemInfo(QString info)
{
    mLoadingItemInfo->setText(info);
}

void LoadingpProgess::setProgessRange(int minimum, int maximum)
{
  //  mProgressBar->setRange(minimum, maximum);
    mMaximum = maximum;
    mMinimum = minimum;

    mProgressBar->setMaximum(maximum);
    mProgressBar->setMinimum(minimum);
}

int LoadingpProgess::getMaxmun()
{
    return mMaximum;
}

void LoadingpProgess::setProgessValue(int value)
{
    mProgressBar->setValue(value);
    mProgressBar->setFormat((QString("%1/%2").arg(QString::number(value)).arg(QString::number(mMaximum))));
}

void LoadingpProgess::initLayout()
{
    setStyleSheet("QProgressBar{border: none;color: white;text-align: right;background: rgb(68, 69, 73);font-size:20px;}"
                  "QProgressBar::chunk { border: none;background: rgb(0, 160, 230);}"
                  "QLabel{font-size:22px;color:white; }"       );

    mProgressBar->setFixedHeight(25);
    layout->addWidget(mLoadingItemInfo);
    layout->addWidget(mProgressBar);
    layout->setContentsMargins(0,0,0,0);

    setLayout(layout);
}
