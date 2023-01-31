#include "defenseDataManager.h"

#include <QTimer>
#include <QDebug>
#include <chrono>
#include <thread>

static int aircraftNumber = 0;
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
    createAircraftInfo();
    //----------------------------------------------------------
    QObject::connect(timer, &QTimer::timeout, [&](){
        //---------------------------------------------
        for(auto station:stationList)
            emit mDefenseDataManager->stationInfoChanged(station);
        //---------------------------------------------
        for(auto system:systemList)
            emit mDefenseDataManager->systemInfoChanged(system);
        for(auto systemStatus:systemStatusList)
            emit mDefenseDataManager->systemStatusInfoChanged(systemStatus);
        for(auto systemCambat:SystemCambatList)
            emit mDefenseDataManager->systemCambatInfoChanged(systemCambat);
        //---------------------------------------------
        updateAircraftInfo();
        createAircraftInfo();
        for(auto aircraft:mAircraftList)
            emit mDefenseDataManager->aircraftInfoChanged(aircraft);

        //emit mDefenseDataManager->clearAircraft(mAircraftList.first().TN);
        //mAircraftList.removeFirst();
    });
    timer->start(10000);
    //---------------------------------------------------------
    QObject::connect(mDefenseDataManager, &DefenseDataManager::aircraftAssigned,[=](int tn, int systemNo){
        qDebug() << "aircraftAssigned: "<<tn<<", "<<systemNo;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        emit mDefenseDataManager->aircraftAssignedResponse(tn, systemNo, true);
    });

    QObject::connect(mDefenseDataManager, &DefenseDataManager::cancelAircraftAssigned,[=](int tn, int systemNo){
        qDebug() << "cancelAircraftAssigned: "<<tn<<", "<<systemNo;
    });

}

AircraftInfo Demo::createAircraftInfo()
{
    AircraftInfo aircraftInfo;
    int tn = 10000 + aircraftNumber++;
    aircraftInfo.TN = tn;
    aircraftInfo.IFFCode="a12345";
    aircraftInfo.CallSign="cls";
    aircraftInfo.Type="type1";
    aircraftInfo.MasterRadar="radar1";
    int identy = (qrand() % (6));
    aircraftInfo.Identification = (AircraftInfo::Identify)identy;// F, K, Z, X, U, H
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
        aircraftInfo.Sends.append(system.Name);
    }

    for(auto station: stationList)
    {
        aircraftInfo.DetectionSystems.append(station.Name);
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
        stationInfo.Number = stationList.count() + 2000;
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
        SystemCambatInfo systemCambatInfo;
        SystemStatusInfo systemStatusInfo;

        QString name = "System" + QString::number(systemList.count());
        double longitude = 48 + (qrand() % (59 - 48));
        double latitude = 27 + (qrand() % (38 - 27));
        double viewRange = (100000 + (qrand() % (300000 - 100000)));
        double mezRange = viewRange - 10000;

        systemInfo.Terminal = "terminal1";
        systemInfo.Name = name;
        systemInfo.Number = systemList.count() + 3000;
        systemInfo.Type = "Type1";
        //
        systemInfo.Latitude = latitude;
        systemInfo.Longitude = longitude;
        systemInfo.Altitude = 4000;//meter
        systemInfo.ViewRange = viewRange;//meter
        systemInfo.MezRange = mezRange;//meter
        systemInfo.Active = true;
        //status info
        systemStatusInfo.Number = systemInfo.Number;
        systemStatusInfo.ReceiveTime = "1392/12/01: 12:10";
        systemStatusInfo.Simulation = "simulation";
        systemStatusInfo.BCCStatus = "s";//s, us
        systemStatusInfo.RadarSearchStatus = "us";//s, us
        systemStatusInfo.Operational = "operational";
        systemStatusInfo.MissileCount = 5;
        systemStatusInfo.RadarMode = "rMode";
        //combat info
        systemCambatInfo.Number = systemInfo.Number;
        systemCambatInfo.TN = 10000;
        systemCambatInfo.Acceptance = "acceptance1";
        systemCambatInfo.Phase = SystemCambatInfo::Search;//search, lock, ...
        systemCambatInfo.Antenna = 50;//degree (lock sight
        systemCambatInfo.ChanelNo = "123014s";
        systemCambatInfo.Inrange = "inrange";

        systemList.append(systemInfo);
        systemStatusList.append(systemStatusInfo);
        SystemCambatList.append(systemCambatInfo);
    }
}
