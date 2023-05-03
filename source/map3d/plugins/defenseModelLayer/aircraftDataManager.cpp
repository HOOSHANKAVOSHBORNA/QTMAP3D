#include "aircraftDataManager.h"
#include "systemDataManager.h"

bool Aircraft::Assignment::operator==(const Aircraft::Assignment &assignment)
{
    return (info->systemInfo.Number == assignment.info->systemInfo.Number);
}
//----------------------------------------------------------------------------------------
AircraftDataManager::AircraftDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;

    addAircraftTab();
    //mAircraftTableModel->setAircraftInfos(mAircraftInfos);

}

void AircraftDataManager::addAssignment(int tn, Aircraft::Assignment assignment)
{
    if(mAircraftData.contains(tn) && !mAircraftData[tn].assigments.contains(assignment))
        mAircraftData[tn].assigments.append(assignment);

    //    if(mAircraftModelNodes.contains(tn))
    //        mAircraftModelNodes[tn]->addAssignment(systemNo, );
}

Aircraft::Data *AircraftDataManager::getAircraftData(int &tn)
{
    return mAircraftData.contains(tn) ? &mAircraftData[tn] : nullptr;
}

void AircraftDataManager::onInfoChanged(AircraftInfo &aircraftInfo)
{
    //--list---------------------------------------------------------------------------------------------------------
    mAircraftData[aircraftInfo.TN].info = aircraftInfo;
    //--model node----------------------------------------------------------------------------------------------------
    osg::ref_ptr<AircraftModelNode> aircraftModelNode;
    osgEarth::GeoPoint geographicPosition(mDefenseModelLayer->mMapController->getMapSRS()->getGeographicSRS(),
                                          aircraftInfo.Longitude, aircraftInfo.Latitude, aircraftInfo.Altitude);

    if(mAircraftData[aircraftInfo.TN].modelNode.valid())
    {
        aircraftModelNode = mAircraftData[aircraftInfo.TN].modelNode;
        aircraftModelNode->flyTo(geographicPosition, aircraftInfo.Heading, aircraftInfo.Speed);

    }
    else
    {
        //create and model node------------------------------------------------
        aircraftModelNode = new AircraftModelNode(mDefenseModelLayer,&mAircraftData[aircraftInfo.TN], aircraftInfo.Type);
        aircraftModelNode->setQStringName(QString::number(aircraftInfo.TN));
        aircraftModelNode->setGeographicPosition(geographicPosition, aircraftInfo.Heading);

        //add to container-----------------------------------------------------
        mAircraftData[aircraftInfo.TN].modelNode = aircraftModelNode;
        //add to map ---------------------------------------------------------
        mDefenseModelLayer->mMapController->addNodeToLayer(aircraftModelNode, AIRCRAFTS_LAYER_NAME);
    }
    //update information------------------------------------------------------------------
    aircraftModelNode->setInformation(aircraftInfo);
    mAircraftTableModel->updateTable(aircraftInfo.TN);
}

void AircraftDataManager::onClear(int tn)
{
    //-----------------------------------------
    mDefenseModelLayer->mMapController->removeNodeFromLayer(mAircraftData[tn].modelNode, AIRCRAFTS_LAYER_NAME);
    //--TODO remove system assignment

    mAircraftTableModel->updateTable(tn);

    mAircraftData.remove(tn);
}

void AircraftDataManager::onAssignmentResponse(int tn, int systemNo, bool result)
{
    if(!result && mAircraftData.contains(tn))
    {
        Aircraft::Assignment assignment;
        assignment.info->systemInfo.Number = systemNo;
        auto list = mAircraftData[tn].assigments;
        list.removeAt(list.indexOf(assignment));
    }

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

