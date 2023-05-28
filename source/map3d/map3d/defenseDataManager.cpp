#include "defenseDataManager.h"

//#include <QTimer>
//#include <QDebug>
//#include <chrono>
//#include <thread>
//#include <QTime>

//#include <osg/Math>

DefenseDataManager::DefenseDataManager(QObject *parent):
    QObject(parent)
{

}
//Demo::Demo(DefenseDataManager *defenseDataManager)
//{
//    uint seed = static_cast<uint>(QDateTime::currentMSecsSinceEpoch() / 1000);
//    qsrand(seed);

//    mDefenseDataManager = defenseDataManager;
//    //run demo ------------------------------------------------
//    //--stystem and station----------------------------------------------------------------------------
//    QTimer *timerSystem = new QTimer();
//    createStationInfo();
//    createSystemInfo();

//    QObject::connect(timerSystem, &QTimer::timeout, [this](){
//        for(auto station:mStations)
//            emit mDefenseDataManager->stationInfoChanged(station);
//        //-------------------
//        for(auto system:mSystems){
//            emit mDefenseDataManager->systemInfoChanged(system.info);
//            emit mDefenseDataManager->systemStatusInfoChanged(system.statusInfo);
//        }
//    });
//    timerSystem->start(10000);
//    //--update Combat------------------------------------------
//    QTimer *timerUpdateCombat = new QTimer();
//    QObject::connect(timerUpdateCombat, &QTimer::timeout, [this](){
//        updateSystemCombatInfo();
//        for(auto system:mSystems)
//            emit mDefenseDataManager->systemCombatInfoChanged(system.combatInfo);
//    });
//    timerUpdateCombat->start(20000);
//    //--aircraft------------------------------------------------------------------------------------------
//    //--create and update aircraft info------------------------
//    QTimer *timerUpdateAircraft = new QTimer();
//    QObject::connect(timerUpdateAircraft, &QTimer::timeout, [this](){
//        createAircraftInfo();
//        updateAircraftInfo();
//        for(auto aircraft:mAircrafts)
//            emit mDefenseDataManager->aircraftInfoChanged(aircraft);
//    });
//    timerUpdateAircraft->start(1000);
//    //--clear aircraft-----------------------------------------
//    QTimer *timerClearAircraft = new QTimer();
//    QObject::connect(timerClearAircraft, &QTimer::timeout, [this](){
//        if(mAircrafts.count() > 0){
//            emit mDefenseDataManager->clearAircraft(mAircrafts.first().TN);
//            mAircrafts.remove(mAircrafts.first().TN);
////                mAircrafAssignment.remove(mAircrafts.first().TN);
//        }
//    });
//    timerClearAircraft->start(5000);
//    //--assignment -----------------------------------------------------------------------------------------
//    //--add assignment------------------------------------
//    QObject::connect(mDefenseDataManager, &DefenseDataManager::aircraftAssigned,[=](int tn, int systemNo){
//        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

//        if(!mAircrafts.contains(tn) || !mSystems.contains(systemNo))
//            emit mDefenseDataManager->aircraftAssignedResponse(tn, systemNo, false);
//        else if(mSystems[systemNo].combatInfo.Phase != SystemCombatInfo::Search)
//            emit mDefenseDataManager->aircraftAssignedResponse(tn, systemNo, false);
//        else{
//            emit mDefenseDataManager->aircraftAssignedResponse(tn, systemNo, true);

//            mAircrafAssignment[tn].append(&mSystems[systemNo]);
//            mSystemAssignment[systemNo].append(&mAircrafts[tn]);
//        }
//    });
//    //--cancel assignment---------------------------------
//    QObject::connect(mDefenseDataManager, &DefenseDataManager::cancelAircraftAssigned,[=](int tn, int systemNo){
//        if(mAircrafAssignment.contains(tn)){
//            auto& assignmens = mAircrafAssignment[tn];
//            for(int i = 0; i< assignmens.count(); i++)
//                if(assignmens[i]->info.Number == systemNo){
//                    assignmens.removeAt(i);
//                }
//        }
//        if(mSystemAssignment.contains(systemNo)){
//            auto& assignmens = mSystemAssignment[systemNo];
//            for(int i = 0; i< assignmens.count(); i++)
//                if(assignmens[i]->TN == tn){
//                    assignmens.removeAt(i);
//                }
//        }
//    });

//}

//Demo::~Demo()
//{
//    qDebug()<<"~Demo";
//}

//static int aircraftNumber = 0;
//const int systemNum = 20;
//const int stationNum = 5;
//void Demo::createAircraftInfo()
//{
//    AircraftInfo aircraftInfo;
//    if(aircraftNumber > 20)
//        return;
//    int tn = 10000 + aircraftNumber++;
//    aircraftInfo.TN = tn;
//    aircraftInfo.IFFCode="a12345";
//    aircraftInfo.CallSign="cls";
////    int type =  (qrand() % (5)) + 1;
//    aircraftInfo.Type= static_cast<AircraftInfo::AircraftType>(type);
//    aircraftInfo.MasterRadar="radar1";
////    int identy = (qrand() % (6));
//    aircraftInfo.Identification = static_cast<AircraftInfo::Identify>(identy);// F, K, Z, X, U, H
//    aircraftInfo.IdentificationMethod="mt1";//3 char
//    aircraftInfo.Time=1678005787;//epoch
//    aircraftInfo.Pos="pos";
//    //
////    double longitude = 48 + (qrand() % (59 - 48));
////    double latitude = 27 + (qrand() % (38 - 27));

////    double altitude = (2000 + (qrand() % (9000 - 2000)));

////    aircraftInfo.Latitude=latitude;
////    aircraftInfo.Longitude=longitude;
////    aircraftInfo.Altitude=altitude;//meter
////    aircraftInfo.Heading = (0 + (qrand() % 361));
//    aircraftInfo.Speed=150;//m/s
//    //
//    for(const auto& system: mSystems)
//    {
//        aircraftInfo.Sends.append(system.info.Name);
//    }

//    for(const auto& station: mStations)
//    {
//        aircraftInfo.DetectionSystems.append(station.Name);
//    }

//    mAircrafts[tn] = aircraftInfo;
//}

//void Demo::updateAircraftInfo()
//{
//    for(auto& aircraft: mAircrafts)
//    {
//        //------------------------
//        double latitude = aircraft.Latitude;
//        double longitude = aircraft.Longitude;
//        double altitude = aircraft.Altitude;
//        double speed = aircraft.Speed;
//        double heading = aircraft.Heading;

//        //    double latitudeDiff = latitude;
//        //    double longitudeDiff = longitude;
//        //    double altitudeDiff = altitude;

////        int randomX = (100 + (qrand() % 19));
////        int randomY = (100 + (qrand() % 19));

//        //altitude = (2000 + (qrand() % (9000 - 2000)));
////        int rn = (0 + (qrand() % 10000));
////        if(rn < 1)
////            heading = (0 + (qrand() % 361));
////        speed = (138 + (qrand() % 137));
//        double teta = osg::DegreesToRadians(90 - heading);
//        double step = 10.0/10000.0;
//        longitude += step * std::cos(teta);
//        latitude += step * std::sin(teta);
////        heading = (0 + (qrand() % 361));
////        int val = qrand() % 4;
////        if(val == 1)
////        {
////            latitude += randomX/10000.0;
////            longitude += randomY/10000.0;
////        }
////        else if(val == 2)
////        {
////            latitude += randomX/10000.0;
////            longitude -= randomY/10000.0;
////        }
////        else if(val == 3)
////        {
////            latitude -= randomX/10000.0;
////            longitude += randomY/10000.0;
////        }
////        else
////        {
////            latitude -= randomX/10000.0;
////            longitude -= randomY/10000.0;
////        }
//        //calculat heading----------------------
//        //    latitudeDiff = latitude - latitudeDiff;
//        //    longitudeDiff = longitude - longitudeDiff;
//        //    altitudeDiff = altitude - altitudeDiff;
//        //    //north =(0, 1, 0)
//        //    heading = acos(longitudeDiff/sqrt(latitudeDiff * latitudeDiff + longitudeDiff * longitudeDiff + altitudeDiff * altitudeDiff));
//        //    heading *= (180.0/3.141592653589793238463);
//        aircraft.Latitude = latitude;
//        aircraft.Longitude = longitude;
//        aircraft.Altitude = altitude;
//        aircraft.Speed = speed;
//        aircraft.Heading = heading;
//    }
//}

//void Demo::createSystemInfo()
//{
//    for(int i = 0; i < systemNum; ++i)
//    {
//        SystemInfo  systemInfo;
//        SystemCombatInfo systemCombatInfo;
//        SystemStatusInfo systemStatusInfo;

//        QString name = "System" + QString::number(mSystems.count());
////        double longitude = 48 + (qrand() % (59 - 48));
////        double latitude = 27 + (qrand() % (38 - 27));
////        double viewRange = (100000 + (qrand() % (300000 - 100000)));
////        double mezRange = viewRange - 10000;

//        systemInfo.Terminal = "terminal1";
//        systemInfo.Name = name;
//        systemInfo.Number = mSystems.count() + 3000;
//        systemInfo.Type = "Type1";
//        //
////        systemInfo.Latitude = latitude;
////        systemInfo.Longitude = longitude;
//        systemInfo.Altitude = 4000;//meter
////        systemInfo.ViewRange = viewRange;//meter
////        systemInfo.MezRange = mezRange;//meter
////        systemInfo.Active = true;
//        //status info
//        systemStatusInfo.Number = systemInfo.Number;
//        systemStatusInfo.ReceiveTime = "1392/12/01 12:10";
//        systemStatusInfo.Simulation = "simulation";
//        systemStatusInfo.BCCStatus = SystemStatusInfo::S;//s, us
//        systemStatusInfo.RadarSearchStatus = SystemStatusInfo::S;//s, us
//        systemStatusInfo.Operational = static_cast<SystemStatusInfo::OperationStatus>(qrand() % 3);
//        systemStatusInfo.MissileCount = 6;
//        systemStatusInfo.RadarMode = "rMode";
//        //combat info
//        systemCombatInfo.Number = systemInfo.Number;
//        systemCombatInfo.TN = 10000;
//        systemCombatInfo.Acceptance = "acceptance1";
//        systemCombatInfo.Phase = SystemCombatInfo::Search;//search, lock, ...
//        systemCombatInfo.Antenna = 50;//degree (lock sight
//        systemCombatInfo.ChanelNo = "123014s";
//        systemCombatInfo.Inrange = "inrange";

//        mSystems[systemInfo.Number].info = systemInfo;
//        mSystems[systemInfo.Number].statusInfo = systemStatusInfo;
//        mSystems[systemInfo.Number].combatInfo = systemCombatInfo;
//    }
//}

//void Demo::updateSystemCombatInfo()
//{
//    for(auto& system: mSystems)
//    {
//        auto phase = system.combatInfo.Phase;
//        SystemCombatInfo::Phases newPhase = SystemCombatInfo::Search;
//        if(mSystemAssignment[system.combatInfo.Number].count() > 0)
//        {
////            int rn = (0 + (qrand() % mSystemAssignment[system.combatInfo.Number].count()));
//            switch (phase) {
//            case SystemCombatInfo::Search:
//                newPhase = SystemCombatInfo::Lock;
//                system.combatInfo.TN = mSystemAssignment[system.combatInfo.Number].at(rn)->TN;
//                break;
//            case SystemCombatInfo::Lock:
//                newPhase = SystemCombatInfo::Fire;
//                break;
//            case SystemCombatInfo::Fire:
//                if(rn % 2 == 0){
//                    newPhase = SystemCombatInfo::Kill;
//                    mAircrafts.remove(system.combatInfo.TN);
//                    mAircrafAssignment.remove(system.combatInfo.TN);

//                    if(mSystemAssignment.contains(system.combatInfo.Number)){
//                        auto& assignmens = mSystemAssignment[system.combatInfo.Number];
//                        for(int i = 0; i< assignmens.count(); i++)
//                            if(assignmens[i]->TN == system.combatInfo.TN){
//                                assignmens.removeAt(i);
//                            }
//                    }

//                }
//                else{
//                    newPhase = SystemCombatInfo::NoKill;

//                    if(mAircrafAssignment.contains(system.combatInfo.TN)){
//                        auto& assignmens = mAircrafAssignment[system.combatInfo.TN];
//                        for(int i = 0; i< assignmens.count(); i++)
//                            if(assignmens[i]->info.Number == system.combatInfo.Number){
//                                assignmens.removeAt(i);
//                            }
//                    }

//                    if(mSystemAssignment.contains(system.combatInfo.Number)){
//                        auto& assignmens = mSystemAssignment[system.combatInfo.Number];
//                        for(int i = 0; i< assignmens.count(); i++)
//                            if(assignmens[i]->TN == system.combatInfo.TN){
//                                assignmens.removeAt(i);
//                            }
//                    }
//                }
//            }
//        }
//        system.combatInfo.Phase = newPhase;
//    }
//}

//void Demo::createStationInfo()
//{
//    for(int i = 0; i < stationNum; ++i)
//    {
//        StationInfo stationInfo;
//        QString name = "Station" + QString::number(mStations.count());
////        double longitude = 48 + (qrand() % (59 - 48));
////        double latitude = 27 + (qrand() % (38 - 27));
////        double radius = (200000 + (qrand() % (500000 - 200000)));

//        stationInfo.Name = name;
//        stationInfo.RadarSearchStatus = StationInfo::S;
//        stationInfo.Number = mStations.count() + 2000;
//        stationInfo.Type = "Type1";
////        stationInfo.Latitude = latitude;
////        stationInfo.Longitude = longitude;
//        stationInfo.PrimSec = "secondary";
////        stationInfo.Radius = radius;//meter
//        stationInfo.CycleTime = 123;//

//        mStations[stationInfo.Number] = stationInfo;
//    }
//}
