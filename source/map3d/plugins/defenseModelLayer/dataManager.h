#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "defenseModelLayer.h"
#include "defenseDataManager.h"
#include "aircraftDataManager.h"
#include "systemDataManager.h"
#include "stationDataManager.h"

class DataManager: public QObject
{
    Q_OBJECT
public:
    DataManager(DefenseDataManager *defenseDataManager, DefenseModelLayer *defenseModelLayer);
public slots:
    void onAircraftInfoChanged(AircraftInfo& aircraftInfo);
    void onSystemInfoChanged(SystemInfo& systemInfo);
    void onSystemStatusInfoChanged(SystemStatusInfo& systemStatusInfo);
    void onSystemCombatInfoChanged(SystemCombatInfo& systemCambatInfo);
    void onStationInfoChanged(StationInfo& stationInfo);
    void onClearAircraft(int tn);
    void onAircraftAssignedResponse(int tn, int systemNo, bool accept);
public:
    void assignAircraft2System(int tn, int systemNo);
    void cancelAircraftAssignments(int tn);
    void clear();
    AircraftDataManager *aircraftDataManager() const;

    SystemDataManager *systemDataManager() const;

    StationDataManager *stationDataManager() const;

private:
    void addAssignmentTab();
private:
    DefenseDataManager *mDefenseDataManager{nullptr};
    DefenseModelLayer *mDefenseModelLayer{nullptr};
    AircraftDataManager *mAircraftDataManager;
    SystemDataManager *mSystemDataManager;
    StationDataManager *mStationDataManager;
    AircraftTableModel *mAircraftAssignmentTableModel;
    SystemTableModel *mSystemAssignmentTableModel;
};

#endif // DATAMANAGER_H
