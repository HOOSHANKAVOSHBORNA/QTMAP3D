#include "dataManager.h"
#include "systemModelNode.h"

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
