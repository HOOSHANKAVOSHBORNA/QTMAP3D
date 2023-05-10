#ifndef AIRCRAFTDATAMANAGER_H
#define AIRCRAFTDATAMANAGER_H

#include <QMap>
#include <QObject>

#include "defenseDataManager.h"
#include "aircraftModelNode.h"
#include "defenseModelLayer.h"
#include "aircraftTableModel.h"

namespace System{
struct Information;
};

namespace Aircraft {
struct Assignment{
    osg::ref_ptr<SystemModelNode> modelNode;
    System::Information* info;
    bool operator== (const Assignment &assignment );
};

struct Data{
    AircraftInfo info;
    osg::ref_ptr<AircraftModelNode> modelNode{nullptr};
    QList<Assignment*> assignments;
    int findAssignment(int systemNo);

};
}

class AircraftDataManager: public QObject
{
    Q_OBJECT
public:
    AircraftDataManager(DefenseModelLayer* defenseModelLayer);
    void upsertInfo(AircraftInfo& aircraftInfo);
    void remove(int tn);
    void addAssignment(int tn, Aircraft::Assignment *assignment);
    void assignmentResponse(int tn, int systemNo, bool accept);
    void clearAssignments(int tn);
    void removeAssignment(int tn, int systemNo);
    const QMap<int, Aircraft::Data*> &getAircraftsData() const;

signals:
    void infoChanged(int tn);
    void removed(int tn);
    void assignmentChanged(int tn);

private:
    DefenseModelLayer* mDefenseModelLayer;
    QMap<int, Aircraft::Data*> mAircraftData;

    AircraftTable *mAircraftsTable;
    AircraftTableModel *mAircraftAssignmentTableModel;
};

#endif // AIRCRAFTDATAMANAGER_H
