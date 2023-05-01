#include "defenseDataManager.h"

#include <QTimer>
#include <QDebug>
#include <chrono>
#include <thread>
#include <QTime>

#include <osg/Math>

DefenseDataManager::DefenseDataManager(QObject *parent):
    QObject(parent)
{

}
Demo::Demo(DefenseDataManager *defenseDataManager)
{
    uint seed = static_cast<uint>(QDateTime::currentMSecsSinceEpoch() / 1000);
    qsrand(seed);

    mDefenseDataManager = defenseDataManager;
    //run demo ------------------------------------------------
    QTimer *timer = new QTimer();

    createStationInfo();
    createSystemInfo();
    createAircraftInfo();
    //----------------------------------------------------------
    QObject::connect(timer, &QTimer::timeout, [this](){
        //---------------------------------------------
        for(auto station:stationList)
            emit mDefenseDataManager->stationInfoChanged(station);
        //---------------------------------------------
        for(auto system:systemList)
            emit mDefenseDataManager->systemInfoChanged(system);
        for(auto systemStatus:systemStatusList)
            emit mDefenseDataManager->systemStatusInfoChanged(systemStatus);

//        createAircraftInfo();
        if(mAircraftList.count() > 0)
        {
            emit mDefenseDataManager->clearAircraft(mAircraftList.first().TN);
            mAircraftList.removeFirst();
        }
    });
    timer->start(10000);
    //--update aircraft info--------------------------------------------------------
    QTimer *timerUpdateAircraft = new QTimer();
    QObject::connect(timerUpdateAircraft, &QTimer::timeout, [this](){
        //---------------------------------------------
        createAircraftInfo();
        updateAircraftInfo();
        for(auto aircraft:mAircraftList)
            emit mDefenseDataManager->aircraftInfoChanged(aircraft);
    });
    timerUpdateAircraft->start(1000);
    //--update cambat------------------------------------------
    QTimer *timerUpdateCambat = new QTimer();
    QObject::connect(timerUpdateCambat, &QTimer::timeout, [this](){
        updateSystemCambatInfo();
        for(auto systemCambat:SystemCambatList)
            emit mDefenseDataManager->systemCambatInfoChanged(systemCambat);
    });
    timerUpdateCambat->start(20000);
    //---------------------------------------------------------
    QObject::connect(mDefenseDataManager, &DefenseDataManager::aircraftAssigned,[=](int tn, int systemNo){
//        qDebug() << "aircraftAssigned: "<<tn<<", "<<systemNo;
        auto aircraftInfo = std::find_if(mAircraftList.begin(), mAircraftList.end(), [tn](AircraftInfo& item){
            return item.TN == tn;
        });
        auto systemInfo = std::find_if(systemList.begin(), systemList.end(), [systemNo](SystemInfo& item){
            return item.Number == systemNo;
        });

        auto systemCambatInfo = std::find_if(SystemCambatList.begin(), SystemCambatList.end(), [systemNo](SystemCambatInfo& item){
            return item.Number == systemNo;
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        if(systemCambatInfo->Phase != SystemCambatInfo::Search) {
            emit mDefenseDataManager->aircraftAssignedResponse(tn, systemNo, false);
        } else
        {
            emit mDefenseDataManager->aircraftAssignedResponse(tn, systemNo, true);
            //-----------------------------------
            SystemInfo tmpSys;
            AircraftInfo tmpAir;
            for (int i = 0; i < systemList.count(); ++i){
                if (systemList[i].Number == systemNo)
                    tmpSys = systemList[i];
            }
            for (int i = 0; i < mAircraftList.count(); ++i){
                if (mAircraftList[i].TN == tn){
                    mAircraftList[i].assignedSystems.append(tmpSys);
                    tmpAir = mAircraftList[i];
                }
            }
            for (int i = 0; i < systemList.count(); ++i){
                if (systemList[i].Number == systemNo)
                    systemList[i].assignedAircrafts.append(tmpAir);
            }
//            if (aircraftInfo != mAircraftList.end() && systemInfo != systemList.end()) {
//                aircraftInfo->assignedSystems.append(*systemInfo);
//                systemInfo->assignedAircrafts.append(*aircraftInfo);
//            }
        }
    });
    //-------------------------------------------------------------
    QObject::connect(mDefenseDataManager, &DefenseDataManager::cancelAircraftAssigned,[=](int tn, int systemNo){
        auto aircraftInfo = std::find_if(mAircraftList.begin(), mAircraftList.end(), [tn](AircraftInfo& item){
            return item.TN == tn;
        });
        auto systemInfo = std::find_if(systemList.begin(), systemList.end(), [systemNo](SystemInfo& item){
            return item.Number == systemNo;
        });
        int tmpSys = 0;
        int tmpAir = 0;
        for (int i = 0; i < systemList.count(); ++i){
            if (systemList[i].Number == systemNo)
                tmpSys = i;
        }
        for (int i = 0; i < mAircraftList.count(); ++i){
            if (mAircraftList[i].TN == tn){
                mAircraftList[i].assignedSystems.removeAt(tmpSys);
                tmpAir = i;
            }
        }
        for (int i = 0; i < systemList.count(); ++i){
            if (systemList[i].Number == systemNo)
                systemList[i].assignedAircrafts.removeAt(tmpAir);
        }
//        aircraftInfo->assignedSystems.removeAll(*systemInfo);
//        systemInfo->assignedAircrafts.removeAll(*aircraftInfo);
    });

}

Demo::~Demo()
{
    qDebug()<<"~Demo";
}

static int aircraftNumber = 0;
const int systemNum = 20;
const int stationNum = 5;
AircraftInfo Demo::createAircraftInfo()
{
    AircraftInfo aircraftInfo;
    if(aircraftNumber > 20)
        return aircraftInfo;
    int tn = 10000 + aircraftNumber++;
    aircraftInfo.TN = tn;
    aircraftInfo.IFFCode="a12345";
    aircraftInfo.CallSign="cls";
    int type =  (qrand() % (5)) + 1;
    aircraftInfo.Type= static_cast<AircraftInfo::AircraftType>(type);
    aircraftInfo.MasterRadar="radar1";
    int identy = (qrand() % (6));
    aircraftInfo.Identification = static_cast<AircraftInfo::Identify>(identy);// F, K, Z, X, U, H
    aircraftInfo.IdentificationMethod="mt1";//3 char
    aircraftInfo.Time=1678005787;//epoch
    aircraftInfo.Pos="pos";
    //
    double longitude = 48 + (qrand() % (59 - 48));
    double latitude = 27 + (qrand() % (38 - 27));

    double altitude = (2000 + (qrand() % (9000 - 2000)));

    aircraftInfo.Latitude=latitude;
    aircraftInfo.Longitude=longitude;
    aircraftInfo.Altitude=altitude;//meter
    aircraftInfo.Heading = (0 + (qrand() % 361));
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

//        int randomX = (100 + (qrand() % 19));
//        int randomY = (100 + (qrand() % 19));

        //altitude = (2000 + (qrand() % (9000 - 2000)));
        int rn = (0 + (qrand() % 10000));
        if(rn < 1)
            heading = (0 + (qrand() % 361));
        speed = (138 + (qrand() % 137));
        double teta = osg::DegreesToRadians(90 - heading);
        double step = 10.0/10000.0;
        longitude += step * std::cos(teta);
        latitude += step * std::sin(teta);
//        heading = (0 + (qrand() % 361));
//        int val = qrand() % 4;
//        if(val == 1)
//        {
//            latitude += randomX/10000.0;
//            longitude += randomY/10000.0;
//        }
//        else if(val == 2)
//        {
//            latitude += randomX/10000.0;
//            longitude -= randomY/10000.0;
//        }
//        else if(val == 3)
//        {
//            latitude -= randomX/10000.0;
//            longitude += randomY/10000.0;
//        }
//        else
//        {
//            latitude -= randomX/10000.0;
//            longitude -= randomY/10000.0;
//        }
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
    for(int i = 0; i < stationNum; ++i)
    {
        StationInfo stationInfo;
        QString name = "Station" + QString::number(stationList.count());
        double longitude = 48 + (qrand() % (59 - 48));
        double latitude = 27 + (qrand() % (38 - 27));
        double radius = (200000 + (qrand() % (500000 - 200000)));

        stationInfo.Name = name;
        stationInfo.RadarSearchStatus = StationInfo::S;
        stationInfo.Number = stationList.count() + 2000;
        stationInfo.Type = "Type1";
        stationInfo.Latitude = latitude;
        stationInfo.Longitude = longitude;
        stationInfo.PrimSec = "secondary";
        stationInfo.Radius = radius;//meter
        stationInfo.CycleTime = 123;//

        stationList.append(stationInfo);
    }
}

void Demo::createSystemInfo()
{
    for(int i = 0; i < systemNum; ++i)
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
//        systemInfo.Active = true;
        //status info
        systemStatusInfo.Number = systemInfo.Number;
        systemStatusInfo.ReceiveTime = "1392/12/01 12:10";
        systemStatusInfo.Simulation = "simulation";
        systemStatusInfo.BCCStatus = SystemStatusInfo::S;//s, us
        systemStatusInfo.RadarSearchStatus = SystemStatusInfo::S;//s, us
        systemStatusInfo.Operational = static_cast<SystemStatusInfo::OperationStatus>(qrand() % 3);
        systemStatusInfo.MissileCount = 6;
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

void Demo::updateSystemCambatInfo()
{
    for(int i = 0; i < systemNum; ++i)
    {
        auto phase = SystemCambatList[i].Phase;
        SystemCambatInfo::Phases newPhase = SystemCambatInfo::Search;
        if(systemList[i].assignedAircrafts.count() > 0)
        {
            int rn = (0 + (qrand() % systemList[i].assignedAircrafts.count()));
            switch (phase) {
            case SystemCambatInfo::Search:
                newPhase = SystemCambatInfo::Lock;
                SystemCambatList[i].TN = systemList[i].assignedAircrafts[rn].TN;
                break;
            case SystemCambatInfo::Lock:
                newPhase = SystemCambatInfo::Fire;
                break;
            case SystemCambatInfo::Fire:
                if(rn % 2 == 0)
                    newPhase = SystemCambatInfo::Kill;
                else
                    newPhase = SystemCambatInfo::NoKill;
            }
        }
        SystemCambatList[i].Phase = newPhase;
    }
}
