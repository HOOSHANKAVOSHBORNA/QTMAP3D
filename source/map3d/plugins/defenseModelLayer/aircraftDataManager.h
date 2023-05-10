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
    void addAssignment(int tn, Aircraft::Assignment *assignment);
    void clearAssignment(int tn);
    void removeAssignment(int tn, int systemNo);
    const QMap<int, Aircraft::Data*> &getAircraftsData() const;

public slots:
    void onInfoChanged(AircraftInfo& aircraftInfo);
    void onClear(int tn);
    void onAssignmentResponse(int tn, int systemNo, bool accept);

private:
    DefenseModelLayer* mDefenseModelLayer;
    QMap<int, Aircraft::Data*> mAircraftData;

    AircraftTable *mAircraftsTable;
    AircraftTableModel *mAircraftAssignmentTableModel;
};

#endif // AIRCRAFTDATAMANAGER_H
