#ifndef BASESTACKEDWIDGET_H
#define BASESTACKEDWIDGET_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QStackedWidget>

class baseStackedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit baseStackedWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *){
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // BASESTACKEDWIDGET_H
