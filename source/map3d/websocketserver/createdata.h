#ifndef CREATEDATA_H
#define CREATEDATA_H

#include <QJsonDocument>



class CreateData
{
public:
    CreateData();
    QJsonDocument createTargetinfo();
    QJsonDocument updateTargetinfo();
    QJsonDocument createRinfo();
    QJsonDocument createSinfo();
    QJsonDocument createMinfo();
private:
    QJsonDocument targetInfo;
};

#endif // CREATEDATA_H
