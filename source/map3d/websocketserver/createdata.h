#ifndef CREATEDATA_H
#define CREATEDATA_H

#include <QJsonDocument>



class CreateData
{
public:
    CreateData();
    QJsonDocument createTargetinfo();
    QJsonDocument createRinfo();
    QJsonDocument createSinfo();
    QJsonDocument createMinfo();
};

#endif // CREATEDATA_H
