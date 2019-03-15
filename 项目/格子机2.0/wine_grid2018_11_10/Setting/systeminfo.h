#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QWidget>
#include <QLabel>

class SystemInfo : public QWidget
{
    Q_OBJECT
public:
    explicit SystemInfo(QWidget *parent = 0);
private:
    QLabel *label;
};

#endif // SYSTEMINFO_H
