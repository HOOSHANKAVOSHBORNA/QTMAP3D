#include "stationDataManager.h"

StationDataManager::StationDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;
}

void StationDataManager::onInfoChanged(StationInfo &stationInfo)
{
    //--list------------------------------------------------------------------
    mStationInfos[stationInfo.Number] = stationInfo;
    //--model node------------------------------------------------------------
    osg::ref_ptr<StationModelNode> stationModelNode;
    osgEarth::GeoPoint geographicPosition(mDefenseModelLayer->mMapController->getMapSRS()->getGeographicSRS(),
                                          stationInfo.Longitude, stationInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    if(mStationModelNodes.contains(stationInfo.Number))
    {
        stationModelNode = mStationModelNodes[stationInfo.Number];
    }
    else
    {
        //create and setting model-------------------------------------------
        stationModelNode = new StationModelNode(mDefenseModelLayer->mMapController, mDefenseModelLayer->mQmlEngine, mDefenseModelLayer->mUIHandle);
        stationModelNode->setQStringName(stationInfo.Name);
        stationModelNode->setGeographicPosition(geographicPosition, 0.0);
        //add to container---------------------------------------------------
        mStationModelNodes[stationInfo.Number] = stationModelNode;
        //add to map --------------------------------------------------------
        mDefenseModelLayer->mMapController->addNodeToLayer(stationModelNode,STATIONS_LAYER_NAME);
    }
    //update information-----------------------------------------------------
    stationModelNode->setInformation(stationInfo);
}


