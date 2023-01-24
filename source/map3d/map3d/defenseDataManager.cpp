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

    createStationInfo();
    createSystemInfo();
    //createAircraftInfo();
    //----------------------------------------------------------
    QObject::connect(timer, &QTimer::timeout, [&](){
        //---------------------------------------------
        for(auto station:stationList)
            emit mDefenseDataManager->stationInfoChanged(station);
        //---------------------------------------------
        for(auto system:systemList)
            emit mDefenseDataManager->systemInfoChanged(system);
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
    double longitude = 35 + (qrand() % (75 - 35));
    //    int longitude = ((qrand() % 180) - 90);
    double latitude = 25 + (qrand() % (43 - 25));

    double altitude = (2000 + (qrand() % (9000 - 2000)));

    aircraftInfo.Latitude=latitude;
    aircraftInfo.Longitude=longitude;
    aircraftInfo.Altitude=altitude;//meter
    aircraftInfo.Heading=30;
    aircraftInfo.Speed=150;//m/s
    //
    for(auto system: systemList)
    {
        aircraftInfo.DetectionSystems.append(system.Name);
    }

    for(auto station: stationList)
    {
        aircraftInfo.Sends.append(station.Name);
    }

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

void Demo::createStationInfo()
{
    for(int i = 0; i < 5; ++i)
    {
        StationInfo stationInfo;
        QString name = "Station" + QString::number(stationList.count());
        double longitude = 48 + (qrand() % (59 - 48));
        double latitude = 27 + (qrand() % (38 - 27));
        double radius = (200000 + (qrand() % (500000 - 200000)));

        stationInfo.Name = name;
        stationInfo.Active = true;
        stationInfo.Number = 123456789;
        stationInfo.Type = "Type1";
        stationInfo.Latitude = latitude;
        stationInfo.Longitude = longitude;
        stationInfo.PrimSec = "secondary";
        stationInfo.Radius = radius;//meter
        stationInfo.CycleTime = 123123456;//

        stationList.append(stationInfo);
    }
}

void Demo::createSystemInfo()
{
    for(int i = 0; i < 10; ++i)
    {
        SystemInfo  systemInfo;
        QString name = "System" + QString::number(systemList.count());
        double longitude = 48 + (qrand() % (59 - 48));
        double latitude = 27 + (qrand() % (38 - 27));
        double viewRange = (100000 + (qrand() % (300000 - 100000)));
        double mezRange = viewRange - 10000;

        systemInfo.Terminal = "terminal1";
        systemInfo.Name = name;
        systemInfo.Number = 123456;
        systemInfo.Type = "Type1";
        systemInfo.Active = true;
        //
        systemInfo.Latitude = latitude;
        systemInfo.Longitude = longitude;
        systemInfo.Altitude = 4000;//meter
        systemInfo.ViewRange = viewRange;//meter
        systemInfo.MezRange = mezRange;//meter
        //status info
        systemInfo.ReceiveTime = "1392/12/01: 12:10";
        systemInfo.Simulation = "simulation";
        systemInfo.BCCStatus = "s";//s, us
        systemInfo.RadarSearchStatus = "us";//s, us
        systemInfo.Operational = "operational";
        systemInfo.MissileCount = 5;
        systemInfo.RadarMode = "rMode";
        //combat info
        systemInfo.TN = 10000;
        systemInfo.Acceptance = "acceptance1";
        systemInfo.Phase = "search";//search, lock, ...
        systemInfo.Antenna = 50;//degree (lock sight
        systemInfo.ChanelNo = "123014s";
        systemInfo.Inrange = "inrange";

        systemList.append(systemInfo);
    }
}
