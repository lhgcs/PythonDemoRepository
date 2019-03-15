#ifndef GOODINFO_H
#define GOODINFO_H

#include <QString>

class GoodInfo
{
public:
    GoodInfo();

    QString getName() const;
    void setName(const QString &value);

    QString getPrice() const;
    void setPrice(const QString &value);

    int getStock() const;
    void setStock(int value);

    QString getTimeStamp() const;
    void setTimeStamp(const QString &value);

    QString getImageUrl() const;
    void setImageUrl(const QString &value);

    int getId() const;
    void setId(int value);

    int getPoints() const;
    void setPoints(int value);

    int getMotor() const;
    void setMotor(int value);

private:
    int id;
    QString name;
    QString price;
    int stock;
    QString timeStamp;//时间戳
    QString imageUrl;
    int points;
    int motor;
};

#endif // GOODINFO_H
