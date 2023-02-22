#ifndef MODEL_H
#define MODEL_H

#include "plugininterface.h"
#include "aircraftModelNode.h"
#include "dataManager.h"

#include<osg/Array>
#include <osg/AnimationPath>
#include <osgAnimation/EaseMotion>

#include <QMap>

namespace osgEarth {
namespace  Annotation{
class ModelNode;
}

}
namespace osg{
class PositionAttitudeTransform;
class Vec3d;
class Node;
}

#define AIRCRAFTS_LAYER_NAME "Aircrafts"
#define SYSTEMS_LAYER_NAME "Systems"
#define STATIONS_LAYER_NAME "Stations"

class DefenseModelLayer :public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "defenseModelLayer.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit DefenseModelLayer(QObject *parent = nullptr);
public:
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *pDesc) override;
    virtual void onSideItemCreated(int index, QObject *pSideItem) override;
    virtual void onToolboxItemClicked(const QString& name, const QString& category) override;
    virtual bool setup(MapController *mapController,
                       UIHandle *UIHandle) override;
    virtual void setDefenseDataManager(DefenseDataManager *defenseDataManager) override;

    void addUpdateAircraft(AircraftInfo aircraftInfo);
    void addUpdateSystem(SystemInfo systemInfo);
    void addUpdateStation(StationInfo stationInfo);
public slots:
    void onAircraftInfoChanged(AircraftInfo& aircraftInfo);
    void onSystemInfoChanged(SystemInfo& systemInfo);
    void onSystemStatusInfoChanged(SystemStatusInfo& systemStatusInfo);
    void onSystemCambatInfoChanged(SystemCambatInfo& systemCambatInfo);
    void onStationInfoChanged(StationInfo& stationInfo);
    void onClearAircraft(int tn);
    void onAircraftAssignedResponse(int tn, int systemNo, bool result);
    void onClear();
protected:
    virtual void frameEvent() override;
    virtual void mousePressEvent(QMouseEvent* event)override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event)override;
    virtual void mouseMoveEvent(QMouseEvent* event)override;
private:
    void aircraftAssign(AircraftModelNode *aircraftModelNode, SystemModelNode *systemModelNode);
    void cancelAircraftAssign(AircraftModelNode *aircraftModelNode);
    DefenseModelNode* pick(float x, float y);
    void findSceneModels(osgViewer::Viewer *viewer);
private:
    QMap<QString,QMap<int, osg::ref_ptr<DefenseModelNode>>>  mModelNodes;
    DefenseModelNode* mSelectedModelNode{nullptr};
    DefenseModelNode* mOnMoveModelNode{nullptr};

    MapController *mMapController;
    UIHandle* mUIHandle;
    QQmlEngine *mQmlEngine = nullptr;
    int mPreCameraRange{0};

    DataManager *mDataManager = nullptr;
    osgEarth::Annotation::ModelNode* mDragAircraftModelNode{nullptr};

    DefenseDataManager *mDefenseDataManager;
};

#endif // MODEL_H
