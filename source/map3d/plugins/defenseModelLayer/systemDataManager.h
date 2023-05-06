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

    std::tuple <int, Assignment*> findAssignment(int tn)
    {
        std::tuple <int, Assignment*> result = std::make_tuple(-1, nullptr);
        System::Assignment s;
        s.info = new AircraftInfo;
        s.info->TN = tn;
         if (assignments.contains(s))
         {
             int index = assignments.indexOf(s);
             std::get<0>(result) = index;
             std::get<1>(result) = &assignments[index];
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
