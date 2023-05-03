#ifndef SYSTEMDATAMANAGER_H
#define SYSTEMDATAMANAGER_H

#include <QMap>
#include <set>
#include <QObject>

#include "defenseDataManager.h"
#include "systemModelNode.h"
#include "defenseModelLayer.h"

namespace System {

struct Information
{
    SystemInfo systemInfo;
    SystemStatusInfo systemStatusInfo;
    SystemCombatInfo systemCombatInfo;
};

struct Assignment
{
    osg::ref_ptr<AircraftModelNode> modelNode;
    AircraftInfo *info;

    bool operator==(const Assignment &assign) {
        return info->TN == assign.info->TN;
    }
};

struct Data
{
    Information information;
    osg::ref_ptr<SystemModelNode> systemModelNode{nullptr};
    QList<Assignment> assignments;
};
}

class SystemDataManager: public QObject
{
    Q_OBJECT

public:
    SystemDataManager(DefenseModelLayer* defenseModelLayer);
    void addAssignment(int systemNo, System::Assignment assignment);
    System::Data *getSystemData(int number);

public slots:
    void onInfoChanged(SystemInfo& systemInfo);
    void onStatusInfoChanged(SystemStatusInfo &systemStatusInfo);
    void onCombatInfoChanged(SystemCombatInfo &systemCombatInfo);
    void onAssignmentResponse(int tn, int systemNo, bool result);

signals:
    void systemDoubleClicked(const int&);
private:
    void addSystemTab();
private:
    DefenseModelLayer* mDefenseModelLayer;
    QMap<int, System::Data> mSystemData;
    SystemTableModel *mSystemTableModel;
};

#endif // SYSTEMDATAMANAGER_H
