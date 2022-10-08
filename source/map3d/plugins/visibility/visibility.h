#ifndef VISIBILITY_H
#define VISIBILITY_H

#include "plugininterface.h"

namespace osgEarth {
namespace Annotation {
    class FeatureNode;
    class ModelNode;
}
}

namespace osg {
    class Vec3d;
    class Geode;
}
class Visibility : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "visibility.json")
    Q_INTERFACES(PluginInterface)

public:
    Visibility(QWidget *parent = nullptr);
    void setUpUI() override;
private:
    osgEarth::Annotation::FeatureNode* makepolygan(QVector<osg::Vec3d> vertices);
    osgEarth::Annotation::ModelNode* makeBackground(float radius);
private:
    osgEarth::Annotation::ModelNode*  mBackVisibilityNode{nullptr};
    osgEarth::Annotation::FeatureNode* mVisibilityNode{nullptr};
};

#endif // VISIBILITY_H
