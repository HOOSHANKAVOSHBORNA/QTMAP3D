#ifndef CREATEDATA_H
#define CREATEDATA_H

#include <QJsonDocument>
#include <QList>



class CreateData
{
public:
    CreateData();
    QJsonDocument createAircraftInfo();
    void updateAircraftInfo();
    void createStationInfo();
    QJsonDocument createSystemInfo();
    QJsonDocument createMinfo();
public:
    QList<QJsonDocument> aircraftList;
    QList<QJsonDocument> stationList;
};

#endif // CREATEDATA_H
