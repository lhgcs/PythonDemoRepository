#include "systeminfo.h"
#include "app.h"
SystemInfo::SystemInfo(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("SystemInfo");
    this->setFixedSize(App::DeskWidth-250-40,App::DeskHeight-40);

    label=new QLabel(this);
    label->setObjectName("SystemInfo");
    label->setFixedSize(App::DeskWidth-250-40,App::DeskHeight-40);
    label->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    QString str=QString("<table align=top > \
                         <tr><td align=right>机台号: </td>  <td>%1</td></tr> \
                         <tr><td align=right>用户名: </td>  <td>%2</td></tr> \
                         </table>").arg(App::macId,App::user);
    label->setText(str);
}

