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
    mDefenseModelLayer->addUpdateAircraft(aircraftInfo);
    //add update list view-----------------------------------------------------------------
    if (mListManager)
    {
        mListManager->setAircraftInfo(aircraftInfo);
    }
}

void DataManager::onSystemInfoChanged(SystemInfo &systemInfo)
{
    mDefenseModelLayer->addUpdateSystem(systemInfo);
    //add update list view-----------------------------------------------------------------
    if (mListManager)
    {
        mListManager->setSystemInfo(systemInfo);
    }
}

void DataManager::onSystemStatusInfoChanged(SystemStatusInfo &systemStatusInfo)
{
    SystemModelNode *systemModelNode = mDefenseModelLayer->getSystemModelNode(systemStatusInfo.Number);
    //update information-----------------------------------------------------
    if(systemModelNode)
        systemModelNode->setStatusInfo(systemStatusInfo);
    //add update list view-----------------------------------------------------------------
    if (mListManager)
    {
        mListManager->setSystemStatusInfo(systemStatusInfo);
    }
}

void DataManager::onSystemCambatInfoChanged(SystemCambatInfo &systemCambatInfo)
{
    SystemModelNode *systemModelNode = mDefenseModelLayer->getSystemModelNode(systemCambatInfo.Number);
    //update information-----------------------------------------------------
    if(systemModelNode)
        systemModelNode->setCambatInfo(systemCambatInfo);
    //add update list view-----------------------------------------------------------------
    if (mListManager)
    {
        mListManager->setSystemCombatInfo(systemCambatInfo);
    }
}

void DataManager::onStationInfoChanged(StationInfo &stationInfo)
{
    mDefenseModelLayer->addUpdateStation(stationInfo);
    //add update list view-----------------------------------------------------------------
    if (mListManager)
    {
        mListManager->setStationInfo(stationInfo);
    }
}

void DataManager::onClearAircraft(int tn)
{
    auto aircraftModelNode = mDefenseModelLayer->getAircraftModelNode(tn);
    if(aircraftModelNode){
        aircraftModelNode->onLeftButtonClicked(false);
        aircraftModelNode->setNodeMask(false);
    }
    if (mListManager)
        mListManager->deleteAircraftInfo(tn);
}

void DataManager::onAircraftAssignedResponse(int tn, int systemNo, bool result)
{
    //    qDebug()<<"onAircraftAssignedResponse:"<<tn<< ", "<< systemNo<<", "<<result;
    SystemModelNode *systemModelNode = mDefenseModelLayer->getSystemModelNode(systemNo);
    AircraftModelNode *aircraftModelNode = mDefenseModelLayer->getAircraftModelNode(tn);

    if(systemModelNode)
        systemModelNode->acceptAssignedModelNode(tn, result);
    //---if rejected then unassinment from aircraft----------------------------------------------------
    if(!result)
    {
        if(aircraftModelNode)
            aircraftModelNode->removeAssignmentModelNode(systemNo);
        if (mListManager)
            mListManager->cancelAssign(tn, systemNo);
    }
}

void DataManager::aircraftAssign(AircraftModelNode *aircraftModelNode, SystemModelNode *systemModelNode)
{
    if(!aircraftModelNode || !systemModelNode)
        return;
    systemModelNode->addAssignedModelNode(aircraftModelNode->getInformation().TN, aircraftModelNode);
    aircraftModelNode->addAssignmentModelNode(systemModelNode->getInformation().Number, systemModelNode);
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
        auto systemModelNodes = aircraftModelNode->getAssignmentModelNondes();
        for(auto systemModelNode: systemModelNodes)
        {
            if(systemModelNode)
            {
                emit mDefenseDataManager->cancelAircraftAssigned(aircraftModelNode->getInformation().TN,
                        systemModelNode->getInformation().Number);
                systemModelNode->removeAssignedModelNode(aircraftModelNode->getInformation().TN);
            }
        }
        aircraftModelNode->clearAssignmentModelNodes();
        mListManager->cancelAssign(aircraftModelNode->getInformation().TN, -1);
    }
}

void DataManager::clear()
{
    mListManager->clearAll();
}
