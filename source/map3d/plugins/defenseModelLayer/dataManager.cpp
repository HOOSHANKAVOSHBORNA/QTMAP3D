#include "dataManager.h"
#include "systemModelNode.h"

#include <thread>

DataManager::DataManager(DefenseDataManager *defenseDataManager, DefenseModelLayer *defenseModelLayer):
	mDefenseDataManager(defenseDataManager),
	mDefenseModelLayer(defenseModelLayer),
	mAircraftDataManager(new AircraftDataManager(defenseModelLayer)),
	mSystemDataManager(new SystemDataManager(defenseModelLayer)),
	mStationDataManager(new StationDataManager(defenseModelLayer))
{
	//--aircraft--------------------------------------------------------
	QObject::connect(defenseDataManager, &DefenseDataManager::aircraftInfoChanged,this ,&DataManager::onAircraftInfoChanged);
	QObject::connect(defenseDataManager, &DefenseDataManager::clearAircraft,this ,&DataManager::onClearAircraft);

	QObject::connect(defenseDataManager, &DefenseDataManager::aircraftAssignedResponse,this ,&DataManager::onAircraftAssignedResponse);
	//--system----------------------------------------------------------
	QObject::connect(defenseDataManager, &DefenseDataManager::systemInfoChanged,this ,&DataManager::onSystemInfoChanged);
	QObject::connect(defenseDataManager, &DefenseDataManager::systemStatusInfoChanged,this ,&DataManager::onSystemStatusInfoChanged);
	QObject::connect(defenseDataManager, &DefenseDataManager::systemCombatInfoChanged,this ,&DataManager::onSystemCombatInfoChanged);
	//--station---------------------------------------------------------
	QObject::connect(defenseDataManager, &DefenseDataManager::stationInfoChanged,this ,&DataManager::onStationInfoChanged);

	//list view---------------------------------------------------------
	mAssignmentTableModel = new AssignmentTableModel(mAircraftDataManager, mSystemDataManager, mDefenseModelLayer);
	//    connect(mListManager, &ListManager::stationDoubleClicked,[=](int number){
	//        StationModelNode* stationModelNode = mDefenseModelLayer->getStationModelNode(number);
	//        mDefenseModelLayer->selectModelNode(stationModelNode);
	//    });
	//    connect(mListManager, &ListManager::systemDoubleClicked,[=](int number){
	//        SystemModelNode* systemModelNode = mDefenseModelLayer->getSystemModelNode(number);
	//        mDefenseModelLayer->selectModelNode(systemModelNode);
	//    });
}

void DataManager::onAircraftInfoChanged(AircraftInfo &aircraftInfo)
{
	mAircraftDataManager->upsertInfo(aircraftInfo);

	//    if(mDefenseModelLayer)
	//        mDefenseModelLayer->addUpdateAircraft(aircraftInfo);
	//    //add update list view-----------------------------------------------------------------
	//    if (mListManager)
	//        mListManager->setAircraftInfo(aircraftInfo);
}

void DataManager::onSystemInfoChanged(SystemInfo &systemInfo)
{
	mSystemDataManager->upsertInfo(systemInfo);
	//    if(mDefenseModelLayer)
	//        mDefenseModelLayer->addUpdateSystem(systemInfo);
	//    //add update list view-----------------------------------------------------------------
	//    if (mListManager)
	//        mListManager->setSystemInfo(systemInfo);
}

void DataManager::onSystemStatusInfoChanged(SystemStatusInfo &systemStatusInfo)
{
	mSystemDataManager->updateStatusInfo(systemStatusInfo);

	//    SystemModelNode *systemModelNode = mDefenseModelLayer->getSystemModelNode(systemStatusInfo.Number);
	//    //update information-----------------------------------------------------
	//    if(systemModelNode)
	//        systemModelNode->setStatusInfo(systemStatusInfo);
	//    //add update list view-----------------------------------------------------------------
	//    if (mListManager)
	//        mListManager->setSystemStatusInfo(systemStatusInfo);
}

void DataManager::onSystemCombatInfoChanged(SystemCombatInfo &systemCombatInfo)
{
	mSystemDataManager->updateCombatInfo(systemCombatInfo);

	//--------------------------------------------------------------------
	if(systemCombatInfo.Phase == SystemCombatInfo::Lock || systemCombatInfo.Phase == SystemCombatInfo::Fire){
		if(mSystemDataManager->getSystemsData().contains(systemCombatInfo.Number)){
			for(auto assignment: mSystemDataManager->getSystemsData()[systemCombatInfo.Number]->assignments){
				if(assignment->info->TN != systemCombatInfo.TN)
				{
					mSystemDataManager->removeAssignment(assignment->info->TN, systemCombatInfo.Number);
					mAircraftDataManager->removeAssignment(assignment->info->TN, systemCombatInfo.Number);

				}
			}
		}
	}
	//-----------------------------------------------------------------
	if(systemCombatInfo.Phase == SystemCombatInfo::Kill)
	{
		onClearAircraft(systemCombatInfo.TN);
	}
	if(systemCombatInfo.Phase == SystemCombatInfo::NoKill)
	{
		mAircraftDataManager->removeAssignment(systemCombatInfo.TN, systemCombatInfo.Number);
		mSystemDataManager->removeAssignment(systemCombatInfo.TN, systemCombatInfo.Number);
	}
	//    SystemModelNode *systemModelNode = mDefenseModelLayer->getSystemModelNode(systemCombatInfo.Number);
	//    AircraftModelNode *aircraftModelNode = mDefenseModelLayer->getAircraftModelNode(systemCombatInfo.TN);
	//    if(systemModelNode && aircraftModelNode)
	//    {
	//        if(systemCombatInfo.Phase == SystemCombatInfo::Lock || systemCombatInfo.Phase == SystemCombatInfo::Fire)
	//        {
	//            //--remove system assignment except lock or fire TN----------------------------------
	//            auto aircrafts = systemModelNode->getAssignments();
	//            for(auto aircraft: aircrafts)
	//            {
	//                if(systemCombatInfo.TN != aircraft->getInformation().TN)
	//                {
	//                    emit mDefenseDataManager->cancelAircraftAssigned(aircraft->getInformation().TN,
	//                                                                     systemModelNode->getInformation().Number);

	//                    aircraft->removeAssignment(systemModelNode->getInformation().Number);
	//                    mListManager->cancelAssign(aircraft->getInformation().TN, systemModelNode->getInformation().Number);
	//                }
	//            }
	//            systemModelNode->clearAssignments(systemCombatInfo.TN);
	//            mListManager->cancelSystemAssignmentsExcept(systemCombatInfo.TN, systemCombatInfo.Number);
	//            //-------------
	//        }
	//        //update information-----------------------------------------------------
	//        systemModelNode->setCombatInfo(systemCombatInfo);
	//        //add update list view-----------------------------------------------------------------
	//        if (mListManager)
	//            mListManager->setSystemCombatInfo(systemCombatInfo);

	//        if(systemCombatInfo.Phase == SystemCombatInfo::Kill)
	//        {
	//            //--remove aircraft assignment---------------------------------------
	//            auto systems = aircraftModelNode->getAssignments();
	//            for(auto system: systems)
	//            {
	//                emit mDefenseDataManager->cancelAircraftAssigned(aircraftModelNode->getInformation().TN,
	//                                                                 system->getInformation().Number);

	//                system->removeAssignment(aircraftModelNode->getInformation().TN);
	//                mListManager->cancelAssign(aircraftModelNode->getInformation().TN, system->getInformation().Number);
	//            }
	//            aircraftModelNode->clearAssignments();
	//            mListManager->cancelAircraftAssignmentsExcept(aircraftModelNode->getInformation().TN, -1);
	//        }
	//    }
}

void DataManager::onStationInfoChanged(StationInfo &stationInfo)
{
	mStationDataManager->upsertInfo(stationInfo);
	//    if(mDefenseModelLayer)
	//        mDefenseModelLayer->addUpdateStation(stationInfo);
	//    //add update list view-----------------------------------------------------------------
	//    if (mListManager)
	//        mListManager->setStationInfo(stationInfo);
}

void DataManager::onClearAircraft(int tn)
{
	//----------- first clear from assignment
	mSystemDataManager->clearAssignments(tn);
	mAircraftDataManager->remove(tn);


	//    mAircraftAssignmentTableModel->updateTable(tn);
	//    if(mDefenseModelLayer)
	//        mDefenseModelLayer->clearAircraft(tn);
	//    if (mListManager)
	//        mListManager->deleteAircraftInfo(tn);
}

void DataManager::onAircraftAssignedResponse(int tn, int systemNo, bool accept)
{
	mAircraftDataManager->assignmentResponse(tn, systemNo, accept);
	mSystemDataManager->assignmentResponse(tn, systemNo, accept);
	//    //    qDebug()<<"onAircraftAssignedResponse:"<<tn<< ", "<< systemNo<<", "<<result;
	//    SystemModelNode *systemModelNode = mDefenseModelLayer->getSystemModelNode(systemNo);
	//    AircraftModelNode *aircraftModelNode = mDefenseModelLayer->getAircraftModelNode(tn);

	//    if(systemModelNode)
	//        systemModelNode->acceptAssignment(tn, result);
	//    if(aircraftModelNode)
	//        aircraftModelNode->acceptAssignment(systemNo, result);

	//    if (mListManager)
	//        mListManager->acceptAssign(tn, systemNo, result);
}

void DataManager::assignAircraft2System(int tn, int systemNo)
{
	Aircraft::Assignment* aAssign = new Aircraft::Assignment();
	if (mSystemDataManager->getSystemsData().contains(systemNo)) {
		auto sData = mSystemDataManager->getSystemsData()[systemNo];
		aAssign->info = sData->information;
		aAssign->modelNode = sData->systemModelNode;
		mAircraftDataManager->addAssignment(tn, aAssign);
	}
	System::Assignment* sAssign = new System::Assignment();
	if (mAircraftDataManager->getAircraftsData().contains(tn)){
		auto aData = mAircraftDataManager->getAircraftsData()[tn];
		sAssign->info = &aData->info;
		sAssign->modelNode = aData->modelNode;
		mSystemDataManager->addAssignment(systemNo, sAssign);
	}

	//    systemModelNode->addAssignment(aircraftModelNode->getInformation().TN, aircraftModelNode);
	//    aircraftModelNode->addAssignment(systemModelNode->getInformation().Number, systemModelNode);
	//--TODO manage memory---------------------------------------
	std::thread* t1 = new std::thread([=](){
		if(mDefenseDataManager)
			emit mDefenseDataManager->aircraftAssigned(tn, systemNo);
	});
	//    mListManager->assignAirToSystem(aircraftModelNode->getInformation().TN, systemModelNode->getInformation().Number);
}

void DataManager::cancelAircraftAssignments(int tn)
{
	mAircraftDataManager->clearAssignments(tn);
	mSystemDataManager->clearAssignments(tn);
	emit mDefenseDataManager->cancelAircraftAssignments(tn);
	//    if(aircraftModelNode)
	//    {
	//        auto systemModelNodes = aircraftModelNode->getAssignments();
	//        for(auto systemModelNode: systemModelNodes)
	//        {
	//            if(systemModelNode)
	//            {
	//                emit mDefenseDataManager->cancelAircraftAssigned(aircraftModelNode->getInformation().TN,
	//                                                                 systemModelNode->getInformation().Number);
	//                systemModelNode->removeAssignment(aircraftModelNode->getInformation().TN);
	//                mListManager->cancelSystemAssignmentsExcept(-1, systemModelNode->getInformation().Number);
	//            }
	//        }

	//        aircraftModelNode->clearAssignments();
	//        mListManager->cancelAircraftAssignmentsExcept(aircraftModelNode->getInformation().TN, -1);
	//    }
}

void DataManager::clear()
{
	//    mListManager->clearAll();
}

StationDataManager *DataManager::stationDataManager() const
{
	return mStationDataManager;
}

SystemDataManager *DataManager::systemDataManager() const
{
	return mSystemDataManager;
}

AircraftDataManager *DataManager::aircraftDataManager() const
{
	return mAircraftDataManager;
}

