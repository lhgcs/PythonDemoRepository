#include "selectgoods.h"

SelectGoods::SelectGoods(QWidget *parent) : QWidget(parent)
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setFixedSize(App::DeskWidth,App::DeskHeight*2/3);

    buttonGroup = new QButtonGroup(this);
    goodsGrid=new GoodsGrid;
    QObject::connect(goodsGrid,SIGNAL(mapped(int)),this,SLOT(keyPress(int)));

    //QObject::connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(keyPress(int)));

    gl=new QGridLayout(this);
    gl->addWidget(goodsGrid);

    this->setLayout(gl);
    this->show();
}
