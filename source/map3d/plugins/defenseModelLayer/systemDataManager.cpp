#include "systemDataManager.h"

SystemDataManager::SystemDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;
    mSystemTableModel = new SystemTable(this, mDefenseModelLayer);
}

void SystemDataManager::upsertInfo(SystemInfo &systemInfo)
{
    //--list-----------------------------------------------------------
    if(mSystemData.contains(systemInfo.Number))
        mSystemData[systemInfo.Number]->information->systemInfo = systemInfo;
    else{
        System::Data* data = new System::Data();
        System::Information* information = new System::Information();
        information->systemInfo = systemInfo;
        data->information = information;
        mSystemData[systemInfo.Number] = data;
    }
    //--model node-----------------------------------------------------
    osg::ref_ptr<SystemModelNode> systemModelNode;
    osgEarth::GeoPoint geographicPosition(mDefenseModelLayer->mapItem()->getMapSRS()->getGeographicSRS(),
                                          systemInfo.Longitude, systemInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    if(mSystemData[systemInfo.Number]->systemModelNode.valid())
    {
        systemModelNode = mSystemData[systemInfo.Number]->systemModelNode;
    }
    else
    {
        //create and setting model-------------------------------------------
        systemModelNode = new SystemModelNode(mDefenseModelLayer, mSystemData[systemInfo.Number]);
        systemModelNode->setQStringName(systemInfo.Name);
        systemModelNode->setGeographicPosition(geographicPosition, 0.0);
        //add to container---------------------------------------------------
        mSystemData[systemInfo.Number]->systemModelNode = systemModelNode;
        //add to map --------------------------------------------------------
        CompositeAnnotationLayer *composit = dynamic_cast<CompositeAnnotationLayer*>(mDefenseModelLayer->mapItem()->getMapObject()->getLayerByName("Defense"));
        if(composit){
            ParenticAnnotationLayer *parentic = dynamic_cast<ParenticAnnotationLayer*>(composit->getLayerByName("System"));
            parentic->addChild(systemModelNode);
        }
//        mDefenseModelLayer->mapItem()->addNodeToLayer(systemModelNode, SYSTEM_LAYER);
    }
    //update information-----------------------------------------------------
    systemModelNode->informationChanged();

    emit infoChanged(systemInfo.Number);
}

void SystemDataManager::updateStatusInfo(SystemStatusInfo &systemStatusInfo)
{
    if(mSystemData.contains(systemStatusInfo.Number))
    {
        mSystemData[systemStatusInfo.Number]->information->systemStatusInfo = systemStatusInfo;

        auto systemModelNode =  mSystemData[systemStatusInfo.Number]->systemModelNode;
        if(systemModelNode.valid())
            systemModelNode->statusInfoChanged();

        emit infoChanged(systemStatusInfo.Number);
    }
}

void SystemDataManager::updateCombatInfo(SystemCombatInfo &systemCombatInfo)
{
    if(mSystemData.contains(systemCombatInfo.Number))
    {
        mSystemData[systemCombatInfo.Number]->information->systemCombatInfo = systemCombatInfo;

        auto systemModelNode = mSystemData[systemCombatInfo.Number]->systemModelNode;
        if(systemModelNode.valid())
            systemModelNode->combatInfoChanged();

        emit infoChanged(systemCombatInfo.Number);
    }
}

void SystemDataManager::addAssignment(int systemNo, System::Assignment* assignment)
{
    if (mSystemData.contains(systemNo) &&
            mSystemData[systemNo]->findAssignment(assignment->info->TN) < 0)
    {
        assignment->line = new LineNode(mDefenseModelLayer->mapItem());
        assignment->line->setPointVisible(true);
        assignment->line->setColor(osgEarth::Color::White);
        assignment->line->setPointColor(osgEarth::Color::Olive);
        assignment->line->setWidth(1);
        assignment->line->setPointWidth(5);
        assignment->line->setTessellation(15);
        assignment->line->setShowBearing(true);
		assignment->line->setShowDistance(true);
		assignment->line->setShowSlope(true);
        mDefenseModelLayer->getModelLayer(SYSTEM_LAYER)->addChild(assignment->line);
//        mDefenseModelLayer->mapItem()->addNodeToLayer(assignment->line, SYSTEM_LAYER);

        mSystemData[systemNo]->assignments.push_back(assignment);
        //----------------------------------------------------
        mSystemData[systemNo]->systemModelNode->assignmentChanged();

        emit assignmentChanged(systemNo);
        emit infoChanged(systemNo);
    }
}

void SystemDataManager::assignmentResponse(int tn, int systemNo, bool accept)
{
    if(!accept){
        removeAssignment(tn, systemNo);
    }
    else if(mSystemData.contains(systemNo)){
        auto index = mSystemData[systemNo]->findAssignment(tn);
        if (index >= 0){
            auto assignment = mSystemData[systemNo]->assignments.at(index);
            assignment->line->setColor(osgEarth::Color::Olive);
            assignment->line->setPointVisible(false);
            assignment->line->setWidth(5);
        }
    }
}

void SystemDataManager::clearAssignments(int tn)
{
    for(const auto& data: mSystemData)
    {
        removeAssignment(tn, data->information->systemInfo.Number);
    }
}

void SystemDataManager::removeAssignment(int tn, int systemNo)
{
    if(mSystemData.contains(systemNo)){
        auto index = mSystemData[systemNo]->findAssignment(tn);
        if(index >= 0){
            mDefenseModelLayer->getModelLayer(SYSTEM_LAYER)->removeChild(mSystemData[systemNo]->assignments.at(index)->line);
//            mDefenseModelLayer->mapItem()->removeNodeFromLayer(mSystemData[systemNo]->assignments.at(index)->line, SYSTEM_LAYER);
            mSystemData[systemNo]->assignments.removeAt(index);
            mSystemData[systemNo]->systemModelNode->assignmentChanged();

            emit assignmentChanged(systemNo);
            emit infoChanged(systemNo);
        }
    }
}

const QMap<int, System::Data*> &SystemDataManager::getSystemsData() const
{
    return mSystemData;
}
