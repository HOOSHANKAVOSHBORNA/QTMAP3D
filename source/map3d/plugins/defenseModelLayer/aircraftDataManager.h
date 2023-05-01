#ifndef AIRCRAFTDATAMANAGER_H
#define AIRCRAFTDATAMANAGER_H

#include <QMap>
#include <set>
#include <QObject>

#include "defenseDataManager.h"
#include "aircraftModelNode.h"
#include "defenseModelLayer.h"

class AircraftDataManager: public QObject
{
    Q_OBJECT
public:
    AircraftDataManager(DefenseModelLayer* defenseModelLayer);
    void assignToSystem(int tn, int systemNo);
public slots:
    void onInfoChanged(AircraftInfo& aircraftInfo);
    void onClear(int tn);
    void onAssignedResponse(int tn, int systemNo, bool result);
private:
    DefenseModelLayer* mDefenseModelLayer;
    QMap<int, AircraftInfo> mAircraftInfos;
    QMap<int, osg::ref_ptr<AircraftModelNode>> mAircraftModelNodes;
    QMap<int, std::set<int>> mAssignments;
};

#endif // AIRCRAFTDATAMANAGER_H
