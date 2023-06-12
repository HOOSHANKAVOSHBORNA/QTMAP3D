#ifndef MODEL_H
#define MODEL_H

#include "defenseModelNode.h"
#include "plugininterface.h"
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
//    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *pDesc) override;
//    virtual void onSideItemCreated(int index, QObject *pSideItem) override;
//    virtual void onToolboxItemClicked(const QString& name, const QString& category) override;
    virtual bool setup() override;
//    virtual void setDefenseDataManager(DefenseDataManager *defenseDataManager) override;

    //void addUpdateAircraft(AircraftInfo aircraftInfo);
//    void addUpdateSystem(SystemInfo systemInfo);
//    void addUpdateStation(StationInfo stationInfo);
//    SystemModelNode *getSystemModelNode(int number)const;
//    AircraftModelNode *getAircraftModelNode(int tn) const;
//    StationModelNode *getStationModelNode(int number) const;
//    void selectModelNode(DefenseModelNode* defenseModelNode);
    //void clearAircraft(int tn);
	void selectModelNode(DefenseModelNode* defenseModelNode);
	void modelNodeDeleted(DefenseModelNode* defenseModelNode);
public slots:
    void onMapClear();

    static osgEarth::Symbology::Style& getDefaultStyle();
protected:
    virtual bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override;
    virtual bool mouseReleaseEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseDoubleClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override;
    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override;

private slots:
    void onAircraftItemClick();
    void onSystemItemClick();
    void onStationItemClick();
    void onFireItemClick();
    void onKillItemClick();
private:
    DefenseModelNode* pick(float x, float y);
    void findSceneModels(osgViewer::Viewer *viewer);
private:
    //QMap<QString,QMap<int, osg::ref_ptr<DefenseModelNode>>>  mModelNodes;
	DefenseModelNode* mSelectedModelNode{nullptr};//do not define as ref_ptr
	DefenseModelNode* mOnMoveModelNode{nullptr};
    int mPreCameraRange{0};

//    ListManager *mListManager = nullptr;
    osgEarth::Annotation::ModelNode* mDragAircraftModelNode{nullptr};

    //DefenseDataManager *mDefenseDataManager;
    DataManager *mDataManager;

};

#endif // MODEL_H
