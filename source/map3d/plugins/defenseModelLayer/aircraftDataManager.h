#ifndef AIRCRAFTDATAMANAGER_H
#define AIRCRAFTDATAMANAGER_H

#include <QMap>
#include <QObject>

#include "defenseDataManager.h"
#include "aircraftModelNode.h"
#include "defenseModelLayer.h"

class AircraftDataManager: public QObject
{
    Q_OBJECT
public:
    AircraftDataManager(DefenseModelLayer* defenseModelLayer);

public slots:
    void onInfoChanged(AircraftInfo& aircraftInfo);
    void onClear(int tn);
    void onAssignedResponse(int tn, int systemNo, bool result);

signals:
    void aircraftDoubleClicked(int NT);

private:
    void addAircraftTab();

private:
    DefenseModelLayer* mDefenseModelLayer;
    QMap<int, AircraftInfo> mAircraftInfos;
    QMap<int, osg::ref_ptr<AircraftModelNode>> mAircraftModelNodes;

    AircraftTableModel *mAircraftTableModel;

};

#endif // AIRCRAFTDATAMANAGER_H
