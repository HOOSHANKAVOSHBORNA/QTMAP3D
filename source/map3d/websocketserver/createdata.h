#ifndef CREATEDATA_H
#define CREATEDATA_H

#include <QJsonDocument>
#include <QList>



class CreateData
{
public:
    CreateData();
    QJsonDocument createTargetinfo();
    void updateTargetinfo();
    QJsonDocument createRinfo();
    QJsonDocument createSinfo();
    QJsonDocument createMinfo();
public:
    QList<QJsonDocument> targetList;
};

#endif // CREATEDATA_H
