#ifndef MODEL_H
#define MODEL_H

#include "plugininterface.h"
#include<osg/Array>
#include <osg/AnimationPath>
#include <osgAnimation/EaseMotion>

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
    void setPosition(const osg::Vec3d& pos, double speed);
    void addModel();

private:
    void demo();
    osgEarth::Annotation::ModelNode*  model;
    osg::PositionAttitudeTransform* modelNode;
    osg::Vec3d mCurrentWorldPoint;

//    friend class MyAnimationPathCallback;
};

#endif // MODEL_H
