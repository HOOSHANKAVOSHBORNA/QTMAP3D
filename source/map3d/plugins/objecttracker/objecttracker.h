#ifndef OBJECTTRACKER_H
#define OBJECTTRACKER_H

#include "plugininterface.h"
#include<osg/Array>

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

class ObjectTracker : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "objecttracker.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit ObjectTracker(QWidget *parent = nullptr);
    void setUpUI() override;
    void setPosition(const osg::Vec3d& pos, float speed);
    void addModel();
    osg::Node* createLine(osg::Vec3Array* vertex , float lineWidth);
    void drawCordination(const osg::Vec3d& pos);

private:
    void trackObject();
    osgEarth::Annotation::ModelNode*  model;
    osg::PositionAttitudeTransform* modelNode;
    int de{0};
};

#endif // OBJECTTRACKER_H
