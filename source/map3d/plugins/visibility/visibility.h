#ifndef VISIBILITY_H
#define VISIBILITY_H

#include "plugininterface.h"

namespace osgEarth {
namespace Annotation {
    class FeatureNode;
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
    osg::Geode* makeShape(osg::Vec3d eye,float radius);
};

#endif // VISIBILITY_H
