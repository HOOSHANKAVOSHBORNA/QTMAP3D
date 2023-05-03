#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "defenseModelLayer.h"
#include "listManager.h"
#include "defenseDataManager.h"
#include "aircraftDataManager.h"
#include "systemDataManager.h"
#include "stationDataManager.h"

class DataManager: public QObject
{
    Q_OBJECT
public:
    DataManager(DefenseDataManager *defenseDataManager, ListManager *listManager, DefenseModelLayer *defenseModelLayer);
public slots:
    void onAircraftInfoChanged(AircraftInfo& aircraftInfo);
    void onSystemInfoChanged(SystemInfo& systemInfo);
    void onSystemStatusInfoChanged(SystemStatusInfo& systemStatusInfo);
    void onSystemCombatInfoChanged(SystemCombatInfo& systemCambatInfo);
    void onStationInfoChanged(StationInfo& stationInfo);
    void onClearAircraft(int tn);
    void onAircraftAssignedResponse(int tn, int systemNo, bool result);
public:
    void assignAircraft2System(int tn, int systemNo);
    void cancelAircraftAssign(AircraftModelNode *aircraftModelNode);
    void clear();
private:
    DefenseDataManager *mDefenseDataManager{nullptr};
    ListManager *mListManager{nullptr};
    DefenseModelLayer *mDefenseModelLayer{nullptr};
    AircraftDataManager *mAircraftDataManager;
    SystemDataManager *mSystemDataManager;
    StationDataManager *mStationDataManager;
};

#endif // DATAMANAGER_H
