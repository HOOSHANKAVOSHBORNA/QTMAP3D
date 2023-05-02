#ifndef SYSTEMDATAMANAGER_H
#define SYSTEMDATAMANAGER_H

#include <QMap>
#include <set>
#include <QObject>

#include "defenseDataManager.h"
#include "systemModelNode.h"
#include "defenseModelLayer.h"

class SystemDataManager: public QObject
{
    Q_OBJECT
public:
    SystemDataManager(DefenseModelLayer* defenseModelLayer);
    void addAssignment(int tn, int systemNo);
public slots:
    void onInfoChanged(SystemInfo& systemInfo);
    void onStatusInfoChanged(SystemStatusInfo &systemStatusInfo);
    void onCombatInfoChanged(SystemCombatInfo &systemCombatInfo);
    void onAssignmentResponse(int tn, int systemNo, bool result);
private:
    DefenseModelLayer* mDefenseModelLayer;
    QMap<int, SystemInfo> mSystemInfos;
    QMap<int, SystemStatusInfo> mSystemStatusInfos;
    QMap<int, SystemCombatInfo> mSystemCombatInfos;
    QMap<int, osg::ref_ptr<SystemModelNode>> mSystemModelNodes;
    QMap<int, std::set<int>> mAssignments;
};

#endif // SYSTEMDATAMANAGER_H
