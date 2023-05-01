#ifndef MODEL_H
#define MODEL_H

#include "plugininterface.h"
#include "aircraftModelNode.h"
#include "listManager.h"
#include "stationModelNode.h"

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

class DataManager;

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
    SystemModelNode *getSystemModelNode(int number)const;
    AircraftModelNode *getAircraftModelNode(int tn) const;
    StationModelNode *getStationModelNode(int number) const;
    void selectModelNode(DefenseModelNode* defenseModelNode);
    void clearAircraft(int tn);
public slots:
    void onMapClear();

    static osgEarth::Symbology::Style& getDefaultStyle();
protected:
    virtual void frameEvent() override;
    virtual void mousePressEvent(QMouseEvent* event)override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event)override;
    virtual void mouseMoveEvent(QMouseEvent* event)override;
public:
    MapController *mMapController{nullptr};
    UIHandle* mUIHandle{nullptr};
    QQmlEngine *mQmlEngine{nullptr};
private:
    DefenseModelNode* pick(float x, float y);
    void findSceneModels(osgViewer::Viewer *viewer);
private:
    QMap<QString,QMap<int, osg::ref_ptr<DefenseModelNode>>>  mModelNodes;
    DefenseModelNode* mSelectedModelNode{nullptr};
    DefenseModelNode* mOnMoveModelNode{nullptr};
    int mPreCameraRange{0};

//    ListManager *mListManager = nullptr;
    osgEarth::Annotation::ModelNode* mDragAircraftModelNode{nullptr};

    //DefenseDataManager *mDefenseDataManager;
    DataManager *mDataManager;

};

#endif // MODEL_H
