#include "aircraftDataManager.h"
#include "systemDataManager.h"

bool Aircraft::Assignment::operator==(const Aircraft::Assignment &assignment)
{
    return (info->systemInfo.Number == assignment.info->systemInfo.Number);
}

int Aircraft::Data::findAssignment(int systemNo)
{
    int result = -1;
    auto fit = std::find_if(assignments.begin(), assignments.end(), [systemNo](Assignment* as){
        return as->info->systemInfo.Number == systemNo;
    });
    if(fit != assignments.end())
        result = static_cast<int>(std::distance(assignments.begin(), fit));
//    Aircraft::Assignment s;
//    s.info = new  System::Information();
//    s.info->systemInfo.Number = systemNo;
//     if (assignments.contains(&s))
//     {
//         result = assignments.indexOf(&s);
//     }

     return result;
}
//----------------------------------------------------------------------------------------
AircraftDataManager::AircraftDataManager(DefenseModelLayer* defenseModelLayer)
{
    mDefenseModelLayer = defenseModelLayer;

    mAircraftsTable = new AircraftTable(this, mDefenseModelLayer);

}
void AircraftDataManager::upsertInfo(AircraftInfo &aircraftInfo)
{
    //--list---------------------------------------------------------------------------------------------------------
    if(mAircraftData.contains(aircraftInfo.TN))
        mAircraftData[aircraftInfo.TN]->info = aircraftInfo;
    else{
        Aircraft::Data* data = new Aircraft::Data();
        data->info = aircraftInfo;
        mAircraftData[aircraftInfo.TN] = data;
    }
    //--model node----------------------------------------------------------------------------------------------------
    osg::ref_ptr<AircraftModelNode> aircraftModelNode;
    osgEarth::GeoPoint geographicPosition(mDefenseModelLayer->mMapController->getMapSRS()->getGeographicSRS(),
                                          aircraftInfo.Longitude, aircraftInfo.Latitude, aircraftInfo.Altitude);

    if(mAircraftData[aircraftInfo.TN]->modelNode.valid())
    {
        aircraftModelNode = mAircraftData[aircraftInfo.TN]->modelNode;
        aircraftModelNode->flyTo(geographicPosition, aircraftInfo.Heading, aircraftInfo.Speed);

    }
    else
    {
        //create and model node------------------------------------------------
        aircraftModelNode = new AircraftModelNode(mDefenseModelLayer,*mAircraftData[aircraftInfo.TN], aircraftInfo.Type);
        aircraftModelNode->setQStringName(QString::number(aircraftInfo.TN));
        aircraftModelNode->setGeographicPosition(geographicPosition, aircraftInfo.Heading);

        //add to container-----------------------------------------------------
        mAircraftData[aircraftInfo.TN]->modelNode = aircraftModelNode;
        //add to map ---------------------------------------------------------
        mDefenseModelLayer->mMapController->addNodeToLayer(aircraftModelNode, AIRCRAFTS_LAYER_NAME);
    }
    //update information------------------------------------------------------------------
    aircraftModelNode->dataChanged();
//    mAircraftTableModel->updateTable(aircraftInfo.TN);
    emit infoChanged(aircraftInfo.TN);
}

void AircraftDataManager::remove(int tn)
{
    if(mAircraftData.contains(tn)){
        mDefenseModelLayer->mMapController->removeNodeFromLayer(mAircraftData[tn]->modelNode, AIRCRAFTS_LAYER_NAME);

        delete mAircraftData[tn];
        mAircraftData.remove(tn);
//        mAircraftTableModel->updateTable(tn);

        emit removed(tn);
    }
}

void AircraftDataManager::addAssignment(int tn, Aircraft::Assignment* assignment)
{
    if(mAircraftData.contains(tn) &&
            mAircraftData[tn]->findAssignment(assignment->info->systemInfo.Number) < 0)
    {
        mAircraftData[tn]->assignments.append(assignment);
        //-----------------------------
        mAircraftData[tn]->modelNode->dataChanged();

        emit assignmentChanged(tn);
    }
}

void AircraftDataManager::assignmentResponse(int tn, int systemNo, bool accept)
{
    if(!accept)
        removeAssignment(tn, systemNo);
}

void AircraftDataManager::clearAssignments(int tn)
{
    if(mAircraftData.contains(tn))
    {
        mAircraftData[tn]->assignments.clear();
        mAircraftData[tn]->modelNode->dataChanged();

        emit assignmentChanged(tn);
    }
}

void AircraftDataManager::removeAssignment(int tn, int systemNo)
{
    if(mAircraftData.contains(tn)){
        auto index = mAircraftData[tn]->findAssignment(systemNo);
        if(index >= 0){
            mAircraftData[tn]->assignments.removeAt(index);
            mAircraftData[tn]->modelNode->dataChanged();

            emit assignmentChanged(tn);
        }
    }
}

const QMap<int, Aircraft::Data *> &AircraftDataManager::getAircraftsData() const
{
    return mAircraftData;
}
