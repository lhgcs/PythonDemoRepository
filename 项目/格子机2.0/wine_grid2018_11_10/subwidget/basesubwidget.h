#ifndef BASESUBWIDGET_H
#define BASESUBWIDGET_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

class BaseSubWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseSubWidget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *){
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // BASESUBWIDGET_H
