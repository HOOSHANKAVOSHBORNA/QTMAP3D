#ifndef MODEL_H
#define MODEL_H

#include "plugininterface.h"
#include "trackmodelwidget.h"
#include "flyingmodel.h"

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

class Model : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "model.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit Model(QWidget *parent = nullptr);
    void setUpUI() override;
    //    osg::AnimationPath* createAnimationPath(const osg::Vec3d &pos1, const osg::Vec3d &pos2, float speed);
    void flyTo(QString type, QString name, const osg::Vec3d& pos, double speed);
    void addTruckModel();
    void addAirplaineModel();
    void addRocketModel();
public slots:
    void clickedTrackNode(QString type ,QString name ,bool isClick);
    void positionChanged(QString type, QString name, osgEarth::GeoPoint position);
    void onClickedWorldPos(double latitude ,double longitude, double altitude);
private:
    void demo();
    void onToolBarWidgetPin(bool isPin);
private:
    QMap<QString,QMap<QString, osgEarth::Annotation::ModelNode*>>  mModels;
    osgEarth::Annotation::ModelNode* mCurrentModel;
    osg::PositionAttitudeTransform* modelNode;

    TrackModelWidget* mTrackModelWidget{nullptr};
    QDockWidget* mDockTrackModelWidget;

    bool mIsPin{false};
    //    friend class MyAnimationPathCallback;
};

#endif // MODEL_H
