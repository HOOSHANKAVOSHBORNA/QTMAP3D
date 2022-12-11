#ifndef MODEL_H
#define MODEL_H

#include "plugininterface.h"
#include "airplane.h"

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

class Model :public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "model.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit Model(QObject *parent = nullptr);
public:
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *pDesc) override;
    virtual void onSideItemCreated(int index, QObject *pSideItem) override;
    virtual void onToolboxItemClicked(const QString& name, const QString& category) override;
    virtual bool setup(MapController *pMapController,
                       NetworkManager *networkManager,
                       InfoWidgetHandle *infoWidgetHandle) override;

    void flyTo(QString type, QString name, const osg::Vec3d& pos, double speed);
    void addTruckModel();
    void addAirplaineModel(QString name, osg::Vec3d position, double heading);
    void addRocketModel(osg::Vec3d position = osg::Vec3d(52.8601, 35.277, 846));
    void addSystemModel(osg::Vec3d position);
    void addStationModel(osg::Vec3d position);
public slots:
    void clickedTrackNode(QString type ,QString name ,bool isClick);
    void positionChanged(QString type, QString name, osgEarth::GeoPoint position);
    void onClickedWorldPos(double latitude ,double longitude, double altitude);
    void onMessageReceived(const QJsonDocument &message);
private:
    void demo();
    void onToolBarWidgetPin(bool isPin);
private:
    QMap<QString,QMap<QString, osgEarth::Annotation::ModelNode*>>  mModels;
    osgEarth::Annotation::ModelNode* mCurrentModel;
    osg::PositionAttitudeTransform* modelNode;

    MapController *mMapController;
};

#endif // MODEL_H
