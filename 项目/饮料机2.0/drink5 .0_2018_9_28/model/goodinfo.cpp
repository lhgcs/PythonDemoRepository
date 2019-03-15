#include "goodinfo.h"

GoodInfo::GoodInfo()
{

}

QString GoodInfo::getName() const
{
    return name;
}

void GoodInfo::setName(const QString &value)
{
    name = value;
}

QString GoodInfo::getPrice() const
{
    return price;
}

void GoodInfo::setPrice(const QString &value)
{
    price = value;
}

int GoodInfo::getStock() const
{
    return stock;
}

void GoodInfo::setStock(int value)
{
    stock = value;
}

QString GoodInfo::getTimeStamp() const
{
    return timeStamp;
}

void GoodInfo::setTimeStamp(const QString &value)
{
    timeStamp = value;
}

QString GoodInfo::getImageUrl() const
{
    return imageUrl;
}

void GoodInfo::setImageUrl(const QString &value)
{
    imageUrl = value;
}

int GoodInfo::getId() const
{
    return id;
}

void GoodInfo::setId(int value)
{
    id = value;
}

int GoodInfo::getPoints() const
{
    return points;
}

void GoodInfo::setPoints(int value)
{
    points = value;
}

int GoodInfo::getMotor() const
{
    return motor;
}

void GoodInfo::setMotor(int value)
{
    motor = value;
}

