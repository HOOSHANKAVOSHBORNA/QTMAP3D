#ifndef AIRCRAFTDATAMANAGER_H
#define AIRCRAFTDATAMANAGER_H

#include <QMap>
#include <QObject>

#include "defenseDataManager.h"
#include "aircraftModelNode.h"
#include "defenseModelLayer.h"

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
    QList<Assignment> assigments;
};
}

class AircraftDataManager: public QObject
{
    Q_OBJECT
public:
    AircraftDataManager(DefenseModelLayer* defenseModelLayer);
    void addAssignment(int tn, Aircraft::Assignment assignment);
    Aircraft::Data* getAircraftData(int &tn);

signals:
    void aircraftDoubleClicked(int NT);

private:
    void addAircraftTab();

public slots:
    void onInfoChanged(AircraftInfo& aircraftInfo);
    void onClear(int tn);
    void onAssignmentResponse(int tn, int systemNo, bool result);

private:
    DefenseModelLayer* mDefenseModelLayer;
    QMap<int, Aircraft::Data> mAircraftData;

    AircraftTableModel *mAircraftTableModel;
};

#endif // AIRCRAFTDATAMANAGER_H