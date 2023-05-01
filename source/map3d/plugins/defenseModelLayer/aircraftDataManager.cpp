#include "aircraftDataManager.h"

AircraftDataManager::AircraftDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;
}

void AircraftDataManager::onInfoChanged(AircraftInfo &aircraftInfo)
{
    //--list---------------------------------------------------------------------------------------------------------
    mAircraftInfos[aircraftInfo.TN] = aircraftInfo;
    //--model node----------------------------------------------------------------------------------------------------
    osg::ref_ptr<AircraftModelNode> aircraftModelNode;
    osgEarth::GeoPoint geographicPosition(mDefenseModelLayer->mMapController->getMapSRS()->getGeographicSRS(),
                                          aircraftInfo.Longitude, aircraftInfo.Latitude, aircraftInfo.Altitude);

    if(mAircraftModelNodes.contains(aircraftInfo.TN))
    {
        aircraftModelNode = mAircraftModelNodes[aircraftInfo.TN];
        aircraftModelNode->flyTo(geographicPosition, aircraftInfo.Heading, aircraftInfo.Speed);

    }
    else
    {
        //create and model node------------------------------------------------
        aircraftModelNode = new AircraftModelNode(mDefenseModelLayer->mMapController, aircraftInfo.Type, mDefenseModelLayer->mQmlEngine,mDefenseModelLayer->mUIHandle);
        aircraftModelNode->setQStringName(QString::number(aircraftInfo.TN));
        aircraftModelNode->setGeographicPosition(geographicPosition, aircraftInfo.Heading);

        //add to container-----------------------------------------------------
        mAircraftModelNodes[aircraftInfo.TN] = aircraftModelNode;
        //add to map ---------------------------------------------------------
        mDefenseModelLayer->mMapController->addNodeToLayer(aircraftModelNode, AIRCRAFTS_LAYER_NAME);
    }
    //update information------------------------------------------------------------------
    aircraftModelNode->setInformation(aircraftInfo);
}

void AircraftDataManager::onClear(int tn)
{
    mAircraftInfos.remove(tn);
    //-----------------------------------------
    mDefenseModelLayer->mMapController->removeNodeFromLayer(mAircraftModelNodes[tn], AIRCRAFTS_LAYER_NAME);
    mAircraftModelNodes.remove(tn);
}

void AircraftDataManager::onAssignedResponse(int tn, int systemNo, bool result)
{

}


