#ifndef STATIONDATAMANAGER_H
#define STATIONDATAMANAGER_H

#include <QMap>
#include <set>
#include <QObject>

#include "defenseDataManager.h"
#include "stationModelNode.h"
#include "defenseModelLayer.h"
#include "stationTableModel.h"

namespace Station {
struct Data{
    StationInfo info;
    osg::ref_ptr<StationModelNode> modelNode{nullptr};
};
}
class StationDataManager: public QObject
{
    Q_OBJECT
public:
    StationDataManager(DefenseModelLayer* defenseModelLayer);
    void upsertInfo(StationInfo& stationInfo);

signals:
    void infoChanged(int stationNo);
    void stationDoubleClicked(const int&);
private:
    void addStationTab();
private:
    DefenseModelLayer* mDefenseModelLayer;
    QMap<int, Station::Data*> mStationData;

    StationTableModel *mStationTableModel;
};

#endif // STATIONDATAMANAGER_H
