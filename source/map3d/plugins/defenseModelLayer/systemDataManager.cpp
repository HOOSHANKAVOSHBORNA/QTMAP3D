#include "systemDataManager.h"

SystemDataManager::SystemDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;

    addSystemTab();
    mSystemTableModel->setSystemInfos(mSystemInfos);
    mSystemTableModel->setSystemStatusInfos(mSystemStatusInfos);
    mSystemTableModel->setSystemCombatInfos(mSystemCombatInfos);
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
    mSystemTableModel->updateTable(systemInfo.Number);
}

void SystemDataManager::onStatusInfoChanged(SystemStatusInfo &systemStatusInfo)
{
    if(mSystemModelNodes.contains(systemStatusInfo.Number))
    {
        mSystemStatusInfos[systemStatusInfo.Number] = systemStatusInfo;

        auto systemModelNode = mSystemModelNodes[systemStatusInfo.Number].get();
        systemModelNode->setStatusInfo(systemStatusInfo);
        mSystemTableModel->updateTable(systemStatusInfo.Number);
    }
}

void SystemDataManager::onCombatInfoChanged(SystemCombatInfo &systemCombatInfo)
{
    if(mSystemModelNodes.contains(systemCombatInfo.Number))
    {
        mSystemCombatInfos[systemCombatInfo.Number] = systemCombatInfo;

        auto systemModelNode = mSystemModelNodes[systemCombatInfo.Number].get();
        systemModelNode->setCombatInfo(systemCombatInfo);
        mSystemTableModel->updateTable(systemCombatInfo.Number);
    }
}

void SystemDataManager::onAssignmentResponse(int tn, int systemNo, bool result)
{
    if(result && mSystemModelNodes.contains(systemNo))
        mSystemModelNodes[systemNo]->acceptAssignment(tn, result);
    else if(mAssignments.contains(systemNo))
        mAssignments[systemNo].erase(tn);
}

void SystemDataManager::addSystemTab()
{
    QQmlComponent *comp3 = new QQmlComponent(mDefenseModelLayer->mQmlEngine);
    QObject::connect(comp3, &QQmlComponent::statusChanged, [this, comp3](){
//        qDebug() << comp3->errorString();

        if (comp3->status() == QQmlComponent::Ready) {
            QQuickItem *systemTab = (QQuickItem*) comp3->create(nullptr);
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
