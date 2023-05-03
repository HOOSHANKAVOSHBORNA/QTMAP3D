#include "systemDataManager.h"
#include "aircraftDataManager.h"

SystemDataManager::SystemDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;

    addSystemTab();
//    mSystemTableModel->setSystemInfos(mSystemInfos);
//    mSystemTableModel->setSystemStatusInfos(mSystemStatusInfos);
//    mSystemTableModel->setSystemCombatInfos(mSystemCombatInfos);
}

void SystemDataManager::addAssignment(int systemNo, System::Assignment assignment)
{
    if (!mSystemData[systemNo].assignments.contains(assignment))
    {
        mSystemData[systemNo].assignments.push_back(assignment);
    }
}

System::Data *SystemDataManager::getSystemData(int number)
{
    return mSystemData.contains(number) ? &mSystemData[number] : nullptr;
}

void SystemDataManager::onInfoChanged(SystemInfo &systemInfo)
{
    //--list-----------------------------------------------------------
    mSystemData[systemInfo.Number].information.systemInfo = systemInfo;
    //--model node-----------------------------------------------------
    osg::ref_ptr<SystemModelNode> systemModelNode;
    osgEarth::GeoPoint geographicPosition(mDefenseModelLayer->mMapController->getMapSRS()->getGeographicSRS(),
                                          systemInfo.Longitude, systemInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    if(mSystemData[systemInfo.Number].systemModelNode.valid())
    {
        systemModelNode = mSystemData[systemInfo.Number].systemModelNode;
    }
    else
    {
        //create and setting model-------------------------------------------
        systemModelNode = new SystemModelNode(mDefenseModelLayer->mMapController, mDefenseModelLayer->mQmlEngine, mDefenseModelLayer->mUIHandle);
        systemModelNode->setQStringName(systemInfo.Name);
        systemModelNode->setGeographicPosition(geographicPosition, 0.0);
        //add to container---------------------------------------------------
        mSystemData[systemInfo.Number].systemModelNode = systemModelNode;
        //add to map --------------------------------------------------------
        mDefenseModelLayer->mMapController->addNodeToLayer(systemModelNode, SYSTEMS_LAYER_NAME);
    }
    //update information-----------------------------------------------------
    systemModelNode->setInformation(systemInfo);
//    mSystemTableModel->updateTable(systemInfo.Number);
}

void SystemDataManager::onStatusInfoChanged(SystemStatusInfo &systemStatusInfo)
{
    if(mSystemData[systemStatusInfo.Number].systemModelNode.valid())
    {
        mSystemData[systemStatusInfo.Number].information.systemStatusInfo = systemStatusInfo;

        auto systemModelNode =  mSystemData[systemStatusInfo.Number].systemModelNode;
        systemModelNode->setStatusInfo(systemStatusInfo);
//        mSystemTableModel->updateTable(systemStatusInfo.Number);
    }
}

void SystemDataManager::onCombatInfoChanged(SystemCombatInfo &systemCombatInfo)
{
    if(mSystemData[systemCombatInfo.Number].systemModelNode.valid())
    {
        mSystemData[systemCombatInfo.Number].information.systemCombatInfo = systemCombatInfo;

        auto systemModelNode = mSystemData[systemCombatInfo.Number].systemModelNode;
        systemModelNode->setCombatInfo(systemCombatInfo);
//        mSystemTableModel->updateTable(systemCombatInfo.Number);
    }
}

void SystemDataManager::onAssignmentResponse(int tn, int systemNo, bool result)
{
    System::Assignment s;
    s.info = new AircraftInfo;
    s.info->TN = tn;
//    if(result && mSystemData[systemNo].assignments.contains(s))
//        mSystemData[systemNo].systemModelNode->acceptAssignment(tn, result);
    if(!result && mSystemData.contains(systemNo)){
        if (mSystemData[systemNo].assignments.contains(s)){
            int indexx = mSystemData[systemNo].assignments.indexOf(s);
            mSystemData[systemNo].assignments.removeAt(indexx);
        }
    }
}

void SystemDataManager::addSystemTab()
{
    QQmlComponent *comp3 = new QQmlComponent(mDefenseModelLayer->mQmlEngine);
    QObject::connect(comp3, &QQmlComponent::statusChanged, [this, comp3](){
//        qDebug() << comp3->errorString();

        if (comp3->status() == QQmlComponent::Ready) {
            QQuickItem *systemTab = static_cast<QQuickItem*>(comp3->create(nullptr));
            mSystemTableModel = new SystemTableModel;
            mSystemTableModel->setMode("TableModel");

            QObject::connect(systemTab,
                             SIGNAL(filterTextChanged(const QString&)),
                             mSystemTableModel,
                             SLOT(setFilterWildcard(const QString&)));

            QObject::connect(systemTab,
                             SIGNAL(systemDoubleClicked(const int&)),
                             this,
                             SIGNAL(systemDoubleClicked(const int&)));

            systemTab->setProperty("model", QVariant::fromValue<SystemTableModel*>(mSystemTableModel));
            mDefenseModelLayer->mUIHandle->lwAddTab("Systems", systemTab);
        }

    });

    comp3->loadUrl(QUrl("qrc:///modelplugin/SystemTableView.qml"));
}
