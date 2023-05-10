#include "stationDataManager.h"

StationDataManager::StationDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;

    addStationTab();
//    mStationTableModel->setStationInfos(mStationInfos);
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
    osgEarth::GeoPoint geographicPosition(mDefenseModelLayer->mMapController->getMapSRS()->getGeographicSRS(),
                                          stationInfo.Longitude, stationInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    if(mStationData[stationInfo.Number]->modelNode.valid())
    {
        stationModelNode = mStationData[stationInfo.Number]->modelNode;
    }
    else
    {
        //create and setting model-------------------------------------------
        stationModelNode = new StationModelNode(mDefenseModelLayer->mMapController, mDefenseModelLayer->mQmlEngine, mDefenseModelLayer->mUIHandle);
        stationModelNode->setQStringName(stationInfo.Name);
        stationModelNode->setGeographicPosition(geographicPosition, 0.0);
        //add to container---------------------------------------------------
        mStationData[stationInfo.Number]->modelNode = stationModelNode;
        //add to map --------------------------------------------------------
        mDefenseModelLayer->mMapController->addNodeToLayer(stationModelNode,STATIONS_LAYER_NAME);
    }
    //update information-----------------------------------------------------
    stationModelNode->setInformation(stationInfo);
    mStationTableModel->updateTable(stationInfo.Number);

    emit infoChanged(stationInfo.Number);
}

void StationDataManager::addStationTab()
{
    QQmlComponent *comp2 = new QQmlComponent(mDefenseModelLayer->mQmlEngine);
    QObject::connect(comp2, &QQmlComponent::statusChanged, [this, comp2](){
//        qDebug() << comp2->errorString();

        if (comp2->status() == QQmlComponent::Ready) {
            QQuickItem *stationTab = (QQuickItem*) comp2->create(nullptr);
            mStationTableModel = new StationTableModel;

            QObject::connect(stationTab,
                             SIGNAL(filterTextChanged(const QString&)),
                             mStationTableModel,
                             SLOT(setFilterWildcard(const QString&)));

            QObject::connect(stationTab,
                             SIGNAL(stationDoubleClicked(const int&)),
                             this,
                             SIGNAL(stationDoubleClicked(const int&)));


            stationTab->setProperty("model", QVariant::fromValue<StationTableModel*>(mStationTableModel));
            mDefenseModelLayer->mUIHandle->lwAddTab("Stations", stationTab);
        }

    });

    comp2->loadUrl(QUrl("qrc:///modelplugin/StationTableView.qml"));
}


