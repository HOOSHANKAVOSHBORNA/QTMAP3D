#include "aircraftDataManager.h"

AircraftDataManager::AircraftDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;

    addAircraftTab();
    mAircraftTableModel->setAircraftInfos(mAircraftInfos);

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
    mAircraftTableModel->updateTable(aircraftInfo.TN);
}

void AircraftDataManager::onClear(int tn)
{
    mAircraftInfos.remove(tn);
    //-----------------------------------------
    mDefenseModelLayer->mMapController->removeNodeFromLayer(mAircraftModelNodes[tn], AIRCRAFTS_LAYER_NAME);
    mAircraftModelNodes.remove(tn);
    mAircraftTableModel->updateTable(tn);
}

void AircraftDataManager::onAssignedResponse(int tn, int systemNo, bool result)
{

}

void AircraftDataManager::addAircraftTab()
{
    QQmlComponent *comp = new QQmlComponent(mDefenseModelLayer->mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
//        qDebug() << comp->errorString();

        if (comp->status() == QQmlComponent::Ready) {
            QQuickItem *aircraftTab = (QQuickItem*) comp->create(nullptr);
            mAircraftTableModel = new AircraftTableModel;
            mAircraftTableModel->setMode("TableModel");

            QObject::connect(aircraftTab,
                             SIGNAL(filterTextChanged(const QString&)),
                             mAircraftTableModel,
                             SLOT(setFilterWildcard(const QString&)));

            QObject::connect(aircraftTab,
                             SIGNAL(aircraftDoubleClicked(const int&)),
                             this,
                             SIGNAL(aircraftDoubleClicked(const int&)));

            QObject::connect(aircraftTab,
                             SIGNAL(sortWithHeader(int)),
                             mAircraftTableModel,
                             SLOT(sortWithHeader(int)));
            aircraftTab->setProperty("model", QVariant::fromValue<AircraftTableModel*>(mAircraftTableModel));
            mDefenseModelLayer->mUIHandle->lwAddTab("Aircrafts", aircraftTab);
        }

    });

    comp->loadUrl(QUrl("qrc:///modelplugin/AircraftTableView.qml"));
}


