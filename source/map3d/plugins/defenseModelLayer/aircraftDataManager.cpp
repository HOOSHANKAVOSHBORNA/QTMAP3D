#include "aircraftDataManager.h"
#include "systemDataManager.h"

bool Aircraft::Assignment::operator==(const Aircraft::Assignment &assignment)
{
    return (info->systemInfo.Number == assignment.info->systemInfo.Number);
}

int Aircraft::Data::findAssignment(int systemNo)
{
    int result = -1;
    Aircraft::Assignment s;
    s.info = new  System::Information();
    s.info->systemInfo.Number = systemNo;
     if (assignments.contains(s))
     {
         result = assignments.indexOf(s);
     }

     return result;
}
//----------------------------------------------------------------------------------------
AircraftDataManager::AircraftDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;

    addAircraftTab();
    mAircraftTableModel->setAircraftInfos(mAircraftData);

}

void AircraftDataManager::addAssignment(int tn, Aircraft::Assignment assignment)
{
    if(mAircraftData.contains(tn) && !mAircraftData[tn].assignments.contains(assignment))
    {
        mAircraftData[tn].assignments.append(assignment);
        //-----------------------------
        mAircraftData[tn].modelNode->dataChanged();
    }
}

void AircraftDataManager::clearAssignment(int tn)
{
    if(mAircraftData.contains(tn))
    {
        mAircraftData[tn].assignments.clear();
        mAircraftData[tn].modelNode->dataChanged();
    }
}

void AircraftDataManager::removeAssignment(int tn, int systemNo)
{
    if(mAircraftData.contains(tn)){
        auto index = mAircraftData[tn].findAssignment(systemNo);
        if(index != -1){
            mAircraftData[tn].assignments.removeAt(index);
            mAircraftData[tn].modelNode->dataChanged();
        }
    }
}

Aircraft::Data *AircraftDataManager::getAircraftData(int &tn)
{
    return mAircraftData.contains(tn) ? &mAircraftData[tn] : nullptr;
}

QMap<int, Aircraft::Data> &AircraftDataManager::getAircraftsData()
{
    return mAircraftData;
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
    aircraftModelNode->dataChanged();
    mAircraftTableModel->updateTable(aircraftInfo.TN);
}

void AircraftDataManager::onClear(int tn)
{
    //-----------------------------------------
    mDefenseModelLayer->mMapController->removeNodeFromLayer(mAircraftData[tn].modelNode, AIRCRAFTS_LAYER_NAME);

    mAircraftData.remove(tn);
    mAircraftTableModel->updateTable(tn);
}

void AircraftDataManager::onAssignmentResponse(int tn, int systemNo, bool accept)
{
    if(!accept && mAircraftData.contains(tn))
    {
        auto index = mAircraftData[tn].findAssignment(systemNo);
        if(index != -1){
            mAircraftData[tn].assignments.removeAt(index);
            mAircraftData[tn].modelNode->dataChanged();
        }
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
                             SIGNAL(doubleClicked(const int&)));

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
