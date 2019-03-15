#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
class BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *){
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // BASEWIDGET_H
