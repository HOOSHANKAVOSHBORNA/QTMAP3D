#include "stationDataManager.h"

StationDataManager::StationDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;
    mStationTableModel = new StationTable(this, mDefenseModelLayer);
}

void StationDataManager::upsertInfo(StationInfo &stationInfo)
{
    //--list------------------------------------------------------------------
    if(mStationData.contains(stationInfo.Number)){
        mStationData[stationInfo.Number]->info = stationInfo;
    }
    else {
        Station::Data* data = new Station::Data();
        data->info = stationInfo;
        mStationData[stationInfo.Number] = data;
    }
    //--model node------------------------------------------------------------
    osg::ref_ptr<StationModelNode> stationModelNode;
    osgEarth::GeoPoint geographicPosition(mDefenseModelLayer->mapItem()->getMapSRS()->getGeographicSRS(),
                                          stationInfo.Longitude, stationInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    if(mStationData[stationInfo.Number]->modelNode.valid())
    {
        stationModelNode = mStationData[stationInfo.Number]->modelNode;
    }
    else
    {
        //create and setting model-------------------------------------------
		stationModelNode = new StationModelNode(mDefenseModelLayer, mStationData[stationInfo.Number]);
        stationModelNode->setQStringName(stationInfo.Name);
        stationModelNode->setGeographicPosition(geographicPosition, 0.0);
        //add to container---------------------------------------------------
        mStationData[stationInfo.Number]->modelNode = stationModelNode;
        //add to map --------------------------------------------------------
        CompositeAnnotationLayer *composit = dynamic_cast<CompositeAnnotationLayer*>(mDefenseModelLayer->mapItem()->getMapObject()->getLayerByName("Defense"));
        if(composit){
            ParenticAnnotationLayer *parentic = dynamic_cast<ParenticAnnotationLayer*>(composit->getLayerByName("Station"));
            parentic->addChild(stationModelNode);
        }
//        mDefenseModelLayer->mapItem()->addNodeToLayer(stationModelNode,STATION_LAYER);
    }
    //update information-----------------------------------------------------
	stationModelNode->dataChanged();

    emit infoChanged(stationInfo.Number);
}

const QMap<int, Station::Data *> &StationDataManager::getStationsData() const
{
    return mStationData;
}
