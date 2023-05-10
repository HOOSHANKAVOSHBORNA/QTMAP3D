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

    bool operator== (Assignment *assign){
        return info->TN == assign->info->TN;
    }
};

struct Data
{
    Information* information;
    osg::ref_ptr<SystemModelNode> systemModelNode{nullptr};
    QList<Assignment*> assignments;

    int findAssignment(int tn)
    {
        int result = -1;
        auto fit = std::find_if(assignments.begin(), assignments.end(), [tn](Assignment* as){
            return as->info->TN == tn;
        });
        if(fit != assignments.end())
            result = static_cast<int>(std::distance(assignments.begin(), fit));

         return result;
    }
    ~Data(){
        delete information;
    }
};
}

class SystemDataManager: public QObject
{
    Q_OBJECT

public:
    SystemDataManager(DefenseModelLayer* defenseModelLayer);
    void upsertInfo(SystemInfo& systemInfo);
    void updateStatusInfo(SystemStatusInfo &systemStatusInfo);
    void updateCombatInfo(SystemCombatInfo &systemCombatInfo);

    void addAssignment(int systemNo, System::Assignment *assignment);
    void assignmentResponse(int tn, int systemNo, bool accept);
    void clearAssignments(int tn);
    void removeAssignment(int tn, int systemNo);
//    System::Data *getSystemData(int number);
    const QMap<int, System::Data *> &getSystemsData() const;

signals:
    void systemDoubleClicked(const int&);
    void infoChanged(int systemNo);
    void assignmentChanged(int systemNo);
private:
    void addSystemTab();
private:
    DefenseModelLayer* mDefenseModelLayer;
    QMap<int, System::Data*> mSystemData;
    SystemTableModel *mSystemTableModel;
};

#endif // SYSTEMDATAMANAGER_H
