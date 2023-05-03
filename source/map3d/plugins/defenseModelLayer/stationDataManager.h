#ifndef STATIONDATAMANAGER_H
#define STATIONDATAMANAGER_H

#include <QMap>
#include <set>
#include <QObject>

#include "defenseDataManager.h"
#include "stationModelNode.h"
#include "defenseModelLayer.h"

class StationDataManager: public QObject
{
    Q_OBJECT
public:
    StationDataManager(DefenseModelLayer* defenseModelLayer);
public slots:
    void onInfoChanged(StationInfo& stationInfo);
private:
    void addStationTab();
signals:
    void stationDoubleClicked(const int&);
private:
    DefenseModelLayer* mDefenseModelLayer;
    QMap<int, StationInfo> mStationInfos;
    QMap<int, osg::ref_ptr<StationModelNode>> mStationModelNodes;

    StationTableModel *mStationTableModel;
};

#endif // STATIONDATAMANAGER_H
