#include "dataManager.h"
#include "systemModelNode.h"

#include <thread>

DataManager::DataManager(DefenseDataManager *defenseDataManager, ListManager *listManager, DefenseModelLayer *defenseModelLayer):
    mDefenseDataManager(defenseDataManager),
    mListManager(listManager),
    mDefenseModelLayer(defenseModelLayer)
{
    //--aircraft--------------------------------------------------------
    QObject::connect(defenseDataManager, &DefenseDataManager::aircraftInfoChanged,this ,&DataManager::onAircraftInfoChanged);
    QObject::connect(defenseDataManager, &DefenseDataManager::clearAircraft,this ,&DataManager::onClearAircraft);
    QObject::connect(defenseDataManager, &DefenseDataManager::aircraftAssignedResponse,this ,&DataManager::onAircraftAssignedResponse);
    //--system----------------------------------------------------------
    QObject::connect(defenseDataManager, &DefenseDataManager::systemInfoChanged,this ,&DataManager::onSystemInfoChanged);
    QObject::connect(defenseDataManager, &DefenseDataManager::systemStatusInfoChanged,this ,&DataManager::onSystemStatusInfoChanged);
    QObject::connect(defenseDataManager, &DefenseDataManager::systemCambatInfoChanged,this ,&DataManager::onSystemCambatInfoChanged);
    //--station---------------------------------------------------------
    QObject::connect(defenseDataManager, &DefenseDataManager::stationInfoChanged,this ,&DataManager::onStationInfoChanged);

    //list view---------------------------------------------------------
    connect(mListManager, &ListManager::aircraftDoubleClicked,[=](int TN){
        AircraftModelNode* aircraftModelNode = mDefenseModelLayer->getAircraftModelNode(TN);
        mDefenseModelLayer->selectModelNode(aircraftModelNode);
    });
    connect(mListManager, &ListManager::stationDoubleClicked,[=](int number){
        StationModelNode* stationModelNode = mDefenseModelLayer->getStationModelNode(number);
        mDefenseModelLayer->selectModelNode(stationModelNode);
    });
    connect(mListManager, &ListManager::systemDoubleClicked,[=](int number){
        SystemModelNode* systemModelNode = mDefenseModelLayer->getSystemModelNode(number);
        mDefenseModelLayer->selectModelNode(systemModelNode);
    });
}

void DataManager::onAircraftInfoChanged(AircraftInfo &aircraftInfo)
{
    if(mDefenseModelLayer)
        mDefenseModelLayer->addUpdateAircraft(aircraftInfo);
    //add update list view-----------------------------------------------------------------
    if (mListManager)
        mListManager->setAircraftInfo(aircraftInfo);
}

void DataManager::onSystemInfoChanged(SystemInfo &systemInfo)
{
    if(mDefenseModelLayer)
        mDefenseModelLayer->addUpdateSystem(systemInfo);
    //add update list view-----------------------------------------------------------------
    if (mListManager)
        mListManager->setSystemInfo(systemInfo);
}

void DataManager::onSystemStatusInfoChanged(SystemStatusInfo &systemStatusInfo)
{
    SystemModelNode *systemModelNode = mDefenseModelLayer->getSystemModelNode(systemStatusInfo.Number);
    //update information-----------------------------------------------------
    if(systemModelNode)
        systemModelNode->setStatusInfo(systemStatusInfo);
    //add update list view-----------------------------------------------------------------
    if (mListManager)
        mListManager->setSystemStatusInfo(systemStatusInfo);
}

void DataManager::onSystemCambatInfoChanged(SystemCambatInfo &systemCambatInfo)
{
    SystemModelNode *systemModelNode = mDefenseModelLayer->getSystemModelNode(systemCambatInfo.Number);
    AircraftModelNode *aircraftModelNode = mDefenseModelLayer->getAircraftModelNode(systemCambatInfo.TN);
    if(systemModelNode && aircraftModelNode)
    {
        if(systemCambatInfo.Phase == SystemCambatInfo::Lock || systemCambatInfo.Phase == SystemCambatInfo::Fire)
        {
            //--remove system assignment except lock or fire TN----------------------------------
            auto aircrafts = systemModelNode->getAssignments();
            for(auto aircraft: aircrafts)
            {
                if(systemCambatInfo.TN != aircraft->getInformation().TN)
                {
                    emit mDefenseDataManager->cancelAircraftAssigned(aircraft->getInformation().TN,
                                                                     systemModelNode->getInformation().Number);

                    aircraft->removeAssignment(systemModelNode->getInformation().Number);
                    mListManager->cancelAssign(aircraft->getInformation().TN, systemModelNode->getInformation().Number);
                }
            }
            systemModelNode->clearAssignments(systemCambatInfo.TN);
            mListManager->cancelSystemAssignmentsExcept(systemCambatInfo.TN, systemCambatInfo.Number);
            //-------------
        }
        //update information-----------------------------------------------------
        systemModelNode->setCambatInfo(systemCambatInfo);
        //add update list view-----------------------------------------------------------------
        if (mListManager)
            mListManager->setSystemCombatInfo(systemCambatInfo);

        if(systemCambatInfo.Phase == SystemCambatInfo::Kill)
        {
            //--remove aircraft assignment---------------------------------------
            auto systems = aircraftModelNode->getAssignments();
            for(auto system: systems)
            {
                emit mDefenseDataManager->cancelAircraftAssigned(aircraftModelNode->getInformation().TN,
                                                                 system->getInformation().Number);

                system->removeAssignment(aircraftModelNode->getInformation().TN);
                mListManager->cancelAssign(aircraftModelNode->getInformation().TN, system->getInformation().Number);
            }
            aircraftModelNode->clearAssignments();
            mListManager->cancelAircraftAssignmentsExcept(aircraftModelNode->getInformation().TN, -1);
        }
    }
}

void DataManager::onStationInfoChanged(StationInfo &stationInfo)
{
    if(mDefenseModelLayer)
        mDefenseModelLayer->addUpdateStation(stationInfo);
    //add update list view-----------------------------------------------------------------
    if (mListManager)
        mListManager->setStationInfo(stationInfo);
}

void DataManager::onClearAircraft(int tn)
{
    if(mDefenseModelLayer)
        mDefenseModelLayer->clearAircraft(tn);
    if (mListManager)
        mListManager->deleteAircraftInfo(tn);
}

void DataManager::onAircraftAssignedResponse(int tn, int systemNo, bool result)
{
    //    qDebug()<<"onAircraftAssignedResponse:"<<tn<< ", "<< systemNo<<", "<<result;
    SystemModelNode *systemModelNode = mDefenseModelLayer->getSystemModelNode(systemNo);
    AircraftModelNode *aircraftModelNode = mDefenseModelLayer->getAircraftModelNode(tn);

    if(systemModelNode)
        systemModelNode->acceptAssignment(tn, result);
    if(aircraftModelNode)
        aircraftModelNode->acceptAssignment(systemNo, result);

    if (mListManager)
        mListManager->acceptAssign(tn, systemNo, result);
}

void DataManager::aircraftAssign(AircraftModelNode *aircraftModelNode, SystemModelNode *systemModelNode)
{
    if(!aircraftModelNode || !systemModelNode)
        return;
    systemModelNode->addAssignment(aircraftModelNode->getInformation().TN, aircraftModelNode);
    aircraftModelNode->addAssignment(systemModelNode->getInformation().Number, systemModelNode);
    //--TODO manage memory---------------------------------------
    std::thread* t1 = new std::thread([=](){
        if(mDefenseDataManager)
            emit mDefenseDataManager->aircraftAssigned(aircraftModelNode->getInformation().TN,
                                                       systemModelNode->getInformation().Number);
    });

    mListManager->assignAirToSystem(aircraftModelNode->getInformation().TN, systemModelNode->getInformation().Number);
}

void DataManager::cancelAircraftAssign(AircraftModelNode *aircraftModelNode)
{
    if(aircraftModelNode)
    {
        auto systemModelNodes = aircraftModelNode->getAssignments();
        for(auto systemModelNode: systemModelNodes)
        {
            if(systemModelNode)
            {
                emit mDefenseDataManager->cancelAircraftAssigned(aircraftModelNode->getInformation().TN,
                                                                 systemModelNode->getInformation().Number);
                systemModelNode->removeAssignment(aircraftModelNode->getInformation().TN);
                mListManager->cancelSystemAssignmentsExcept(-1, systemModelNode->getInformation().Number);
            }
        }

        aircraftModelNode->clearAssignments();
        mListManager->cancelAircraftAssignmentsExcept(aircraftModelNode->getInformation().TN, -1);
    }
}

void DataManager::clear()
{
    mListManager->clearAll();
}