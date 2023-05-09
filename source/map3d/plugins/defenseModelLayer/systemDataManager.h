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
    osg::ref_ptr<LineNode> line;

    void updateLine(const osgEarth::GeoPoint& position)
    {
        line->clear();
        line->addPoint(position);
        line->addPoint(modelNode->getPosition());
    }

    bool operator==(const Assignment &assign) {
        return info->TN == assign.info->TN;
    }
};

struct Data
{
    Information information;
    osg::ref_ptr<SystemModelNode> systemModelNode{nullptr};
    QList<Assignment> assignments;

    int findAssignment(int tn)
    {
        int result = -1;
        System::Assignment s;
        s.info = new AircraftInfo;
        s.info->TN = tn;
         if (assignments.contains(s))
         {
             result = assignments.indexOf(s);
         }

         return result;
    }
};
}

class SystemDataManager: public QObject
{
    Q_OBJECT

public:
    SystemDataManager(DefenseModelLayer* defenseModelLayer);
    void addAssignment(int systemNo, System::Assignment assignment);
    void removeAssignments(int tn);
    void removeAssignment(int tn, int systemNo);
//    System::Data *getSystemData(int number);
    const QMap<int, System::Data> &getSystemsData() const;

public slots:
    void onInfoChanged(SystemInfo& systemInfo);
    void onStatusInfoChanged(SystemStatusInfo &systemStatusInfo);
    void onCombatInfoChanged(SystemCombatInfo &systemCombatInfo);
    void onAssignmentResponse(int tn, int systemNo, bool accept);

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
