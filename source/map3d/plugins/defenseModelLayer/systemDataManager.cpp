#include "systemDataManager.h"
#include "aircraftDataManager.h"

SystemDataManager::SystemDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;

    addSystemTab();
    mSystemTableModel->setSystemInfos(mSystemData);
}

void SystemDataManager::addAssignment(int systemNo, System::Assignment* assignment)
{
    if (mSystemData.contains(systemNo) && !mSystemData[systemNo]->assignments.contains(assignment))
    {
        assignment->line = new LineNode(mDefenseModelLayer->mMapController);
        assignment->line->setPointVisible(true);
        assignment->line->setColor(osgEarth::Color::White);
        assignment->line->setPointColor(osgEarth::Color::Olive);
        assignment->line->setWidth(1);
        assignment->line->setPointWidth(5);
        assignment->line->setTessellation(15);
        assignment->line->setShowBearing(true);
        mDefenseModelLayer->mMapController->addNodeToLayer(assignment->line, SYSTEMS_LAYER_NAME);

        mSystemData[systemNo]->assignments.push_back(assignment);
        //----------------------------------------------------
        mSystemData[systemNo]->systemModelNode->assignmentChanged();
    }
}

void SystemDataManager::removeAssignments(int tn)
{
    for(auto& data: mSystemData)
    {
        auto index = data->findAssignment(tn);
        if(index != -1){
            mDefenseModelLayer->mMapController->removeNodeFromLayer(data->assignments.at(index)->line, SYSTEMS_LAYER_NAME);
            data->assignments.removeAt(index);
            data->systemModelNode->assignmentChanged();
        }
    }
}

void SystemDataManager::removeAssignment(int tn, int systemNo)
{
    if(mSystemData.contains(systemNo)){
        auto index = mSystemData[systemNo]->findAssignment(tn);
        if(index != -1){
            mDefenseModelLayer->mMapController->removeNodeFromLayer(mSystemData[systemNo]->assignments.at(index)->line, SYSTEMS_LAYER_NAME);
            mSystemData[systemNo]->assignments.removeAt(index);
            mSystemData[systemNo]->systemModelNode->assignmentChanged();
        }
    }
}

//System::Data *SystemDataManager::getSystemData(int number)
//{
//    return mSystemData.contains(number) ? &mSystemData[number] : nullptr;
//}

const QMap<int, System::Data*> &SystemDataManager::getSystemsData() const
{
    return mSystemData;
}

void SystemDataManager::onInfoChanged(SystemInfo &systemInfo)
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
    osgEarth::GeoPoint geographicPosition(mDefenseModelLayer->mMapController->getMapSRS()->getGeographicSRS(),
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
        mDefenseModelLayer->mMapController->addNodeToLayer(systemModelNode, SYSTEMS_LAYER_NAME);
    }
    //update information-----------------------------------------------------
    systemModelNode->informationChanged();
    mSystemTableModel->updateTable(systemInfo.Number);
}

void SystemDataManager::onStatusInfoChanged(SystemStatusInfo &systemStatusInfo)
{
    if(mSystemData[systemStatusInfo.Number]->systemModelNode.valid())
    {
        mSystemData[systemStatusInfo.Number]->information->systemStatusInfo = systemStatusInfo;

        auto systemModelNode =  mSystemData[systemStatusInfo.Number]->systemModelNode;
        systemModelNode->statusInfoChanged();
        mSystemTableModel->updateTable(systemStatusInfo.Number);
    }
}

void SystemDataManager::onCombatInfoChanged(SystemCombatInfo &systemCombatInfo)
{
    if(mSystemData[systemCombatInfo.Number]->systemModelNode.valid())
    {
        mSystemData[systemCombatInfo.Number]->information->systemCombatInfo = systemCombatInfo;

        auto systemModelNode = mSystemData[systemCombatInfo.Number]->systemModelNode;
        systemModelNode->combatInfoChanged();
        mSystemTableModel->updateTable(systemCombatInfo.Number);
    }
}

void SystemDataManager::onAssignmentResponse(int tn, int systemNo, bool accept)
{
    if(mSystemData.contains(systemNo)){
        auto index = mSystemData[systemNo]->findAssignment(tn);
        if (index != -1){
            auto assignment = mSystemData[systemNo]->assignments.at(index);
            if(!accept){
                mDefenseModelLayer->mMapController->removeNodeFromLayer(assignment->line, SYSTEMS_LAYER_NAME);
                mSystemData[systemNo]->assignments.removeAt(index);
                mSystemData[systemNo]->systemModelNode->assignmentChanged();
            }
            else {
                assignment->line->setColor(osgEarth::Color::Olive);
                assignment->line->setPointVisible(false);
                assignment->line->setWidth(5);
            }
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

bool System::Assignment::operator==(System::Assignment *assign)
{
    return info->TN == assign->info->TN;
}
