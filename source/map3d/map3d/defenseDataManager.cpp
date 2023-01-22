#include "defenseDataManager.h"

#include <QTimer>

DefenseDataManager::DefenseDataManager(QObject *parent):
    QObject(parent)
{

}
Demo::Demo(DefenseDataManager *defenseDataManager)
{
    mDefenseDataManager = defenseDataManager;
    //run demo ------------------------------------------------
    QTimer *timer = new QTimer();

//    createData.createStationInfo();
//    createData.createSystemInfo();
    createAircraftInfo();
    //----------------------------------------------------------
    QObject::connect(timer, &QTimer::timeout, [&](){
        //---------------------------------------------
//        for(auto station:createData.stationList)
//            server.sendMessageToAll(station);
//        //---------------------------------------------
//        for(auto system:createData.systemList)
//            server.sendMessageToAll(system);
        //---------------------------------------------
        updateAircraftInfo();
        createAircraftInfo();
        for(auto aircraft:mAircraftList)
            emit mDefenseDataManager->aircraftInfoChanged(aircraft);
    });
    timer->start(10000);
    //---------------------------------------------------------

}

AircraftInfo Demo::createAircraftInfo()
{
    AircraftInfo aircraftInfo;
    int tn = 10000 + mAircraftList.count();
    aircraftInfo.TN = QString::number(tn);
    aircraftInfo.IFFCode="a12345";
    aircraftInfo.CallSign="cls";
    aircraftInfo.Type="type1";
    aircraftInfo.MasterRadar="radar1";
    aircraftInfo.Identification="F";// F, K, Z, X, U, H
    aircraftInfo.IdentificationMethod="mt1";//3 char
    aircraftInfo.Time="12345678954213";//epoch
    aircraftInfo.Pos="pos";
    //
    //    int latitude = ((qrand() % 360) - 180);
    double latitude = 35 + (qrand() % (75 - 35));
    //    int longitude = ((qrand() % 180) - 90);
    double longitude = 25 + (qrand() % (43 - 25));

    double altitude = (2000 + (qrand() % (9000 - 2000)));

    aircraftInfo.Latitude=latitude;
    aircraftInfo.Longitude=longitude;
    aircraftInfo.Altitude=altitude;//meter
    aircraftInfo.Heading=30;
    aircraftInfo.Speed=150;//m/s
    //
//    for(auto system: systemList
//    {
//        QJsonObject mainObject = system.object(;
//        QJsonObject data = mainObject.value("Data".toObject(;
//        //------------------------
//        QString name = data.value("Name".toString(;
//        detectSystems.push_back(name;
//    }
    //
//    for(auto station: stationList
//    {
//        QJsonObject mainObject = station.object(;
//        QJsonObject data = mainObject.value("Data".toObject(;
//        //------------------------
//        QString name = data.value("Name".toString(;
//        sends.push_back(name;
//    }

    mAircraftList.append(aircraftInfo);
    return aircraftInfo;

}

void Demo::updateAircraftInfo()
{
    for(int i = 0; i < mAircraftList.count(); ++i)
    {
        //------------------------
        double latitude = mAircraftList[i].Latitude;
        double longitude = mAircraftList[i].Longitude;
        double altitude = mAircraftList[i].Altitude;
        double speed = mAircraftList[i].Speed;
        double heading = mAircraftList[i].Heading;

        //    double latitudeDiff = latitude;
        //    double longitudeDiff = longitude;
        //    double altitudeDiff = altitude;

        int randomX = (100 + (qrand() % 19));
        int randomY = (100 + (qrand() % 19));

        //altitude = (2000 + (qrand() % (9000 - 2000)));

        speed = (138 + (qrand() % 137));
        heading = (0 + (qrand() % 361));
        int val = qrand() % 4;
        if(val == 1)
        {
            latitude += randomX/10000.0;
            longitude += randomY/10000.0;
        }
        else if(val == 2)
        {
            latitude += randomX/10000.0;
            longitude -= randomY/10000.0;
        }
        else if(val == 3)
        {
            latitude -= randomX/10000.0;
            longitude += randomY/10000.0;
        }
        else
        {
            latitude -= randomX/10000.0;
            longitude -= randomY/10000.0;
        }
        //calculat heading----------------------
        //    latitudeDiff = latitude - latitudeDiff;
        //    longitudeDiff = longitude - longitudeDiff;
        //    altitudeDiff = altitude - altitudeDiff;
        //    //north =(0, 1, 0)
        //    heading = acos(longitudeDiff/sqrt(latitudeDiff * latitudeDiff + longitudeDiff * longitudeDiff + altitudeDiff * altitudeDiff));
        //    heading *= (180.0/3.141592653589793238463);
        mAircraftList[i].Latitude = latitude;
        mAircraftList[i].Longitude = longitude;
        mAircraftList[i].Altitude = altitude;
        mAircraftList[i].Speed = speed;
        mAircraftList[i].Heading = heading;
    }
}
