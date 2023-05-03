#ifndef AIRCRAFTDATAMANAGER_H
#define AIRCRAFTDATAMANAGER_H

#include <QMap>
#include <QObject>

#include "defenseDataManager.h"
#include "aircraftModelNode.h"
#include "defenseModelLayer.h"

class AircraftDataManager: public QObject
{
    struct Assignmen{
        osg::ref_ptr<SystemModelNode> modelNode;
        SystemInfo* info;
    };

    struct AircraftData{
        AircraftInfo info;
        osg::ref_ptr<AircraftModelNode> modelNode;
        QList<Assignmen> assigments;
    };

    Q_OBJECT
public:
    AircraftDataManager(DefenseModelLayer* defenseModelLayer);
    void addAssignment(int tn, int systemNo);
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
    QMap<int, AircraftData> mAircraftData;

    AircraftTableModel *mAircraftTableModel;
};

#endif // AIRCRAFTDATAMANAGER_H
