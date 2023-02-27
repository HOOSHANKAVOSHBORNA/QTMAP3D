#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "defenseModelLayer.h"
#include "listManager.h"
#include "defenseDataManager.h"

class DataManager: public QObject
{
    Q_OBJECT
public:
    DataManager(DefenseDataManager *defenseDataManager, ListManager *listManager, DefenseModelLayer *defenseModelLayer);
public slots:
    void onAircraftInfoChanged(AircraftInfo& aircraftInfo);
    void onSystemInfoChanged(SystemInfo& systemInfo);
    void onSystemStatusInfoChanged(SystemStatusInfo& systemStatusInfo);
    void onSystemCambatInfoChanged(SystemCambatInfo& systemCambatInfo);
    void onStationInfoChanged(StationInfo& stationInfo);
    void onClearAircraft(int tn);
    void onAircraftAssignedResponse(int tn, int systemNo, bool result);
public:
    void aircraftAssign(AircraftModelNode *aircraftModelNode, SystemModelNode *systemModelNode);
    void cancelAircraftAssign(AircraftModelNode *aircraftModelNode);
    void clear();
private:
    DefenseDataManager *mDefenseDataManager{nullptr};
    ListManager *mListManager{nullptr};
    DefenseModelLayer *mDefenseModelLayer{nullptr};
};

#endif // DATAMANAGER_H
