#include "systemDataManager.h"

SystemDataManager::SystemDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;
}

void SystemDataManager::addAssignment(int tn, int systemNo)
{
    mAssignments[systemNo].insert(tn);
}

void SystemDataManager::onInfoChanged(SystemInfo &systemInfo)
{
    //--list-----------------------------------------------------------
    mSystemInfos[systemInfo.Number] = systemInfo;
    //--model node-----------------------------------------------------
    osg::ref_ptr<SystemModelNode> systemModelNode;
    osgEarth::GeoPoint geographicPosition(mDefenseModelLayer->mMapController->getMapSRS()->getGeographicSRS(),
                                          systemInfo.Longitude, systemInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    if(mSystemModelNodes.contains(systemInfo.Number))
    {
        systemModelNode = mSystemModelNodes[systemInfo.Number].get();
    }
    else
    {
        //create and setting model-------------------------------------------
        systemModelNode = new SystemModelNode(mDefenseModelLayer->mMapController, mDefenseModelLayer->mQmlEngine, mDefenseModelLayer->mUIHandle);
        systemModelNode->setQStringName(systemInfo.Name);
        systemModelNode->setGeographicPosition(geographicPosition, 0.0);
        //add to container---------------------------------------------------
        mSystemModelNodes[systemInfo.Number] = systemModelNode;
        //add to map --------------------------------------------------------
        mDefenseModelLayer->mMapController->addNodeToLayer(systemModelNode, SYSTEMS_LAYER_NAME);
    }
    //update information-----------------------------------------------------
    systemModelNode->setInformation(systemInfo);
}

void SystemDataManager::onStatusInfoChanged(SystemStatusInfo &systemStatusInfo)
{
    if(mSystemModelNodes.contains(systemStatusInfo.Number))
    {
        mSystemStatusInfos[systemStatusInfo.Number] = systemStatusInfo;

        auto systemModelNode = mSystemModelNodes[systemStatusInfo.Number].get();
        systemModelNode->setStatusInfo(systemStatusInfo);
    }
}

void SystemDataManager::onCombatInfoChanged(SystemCombatInfo &systemCombatInfo)
{
    if(mSystemModelNodes.contains(systemCombatInfo.Number))
    {
        mSystemCombatInfos[systemCombatInfo.Number] = systemCombatInfo;

        auto systemModelNode = mSystemModelNodes[systemCombatInfo.Number].get();
        systemModelNode->setCombatInfo(systemCombatInfo);
    }
}

void SystemDataManager::onAssignmentResponse(int tn, int systemNo, bool result)
{
    if(result && mSystemModelNodes.contains(systemNo))
        mSystemModelNodes[systemNo]->acceptAssignment(tn, result);
    else if(mAssignments.contains(systemNo))
        mAssignments[systemNo].erase(tn);
}
