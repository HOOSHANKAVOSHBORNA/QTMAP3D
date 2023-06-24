#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include <osgEarthAnnotation/PlaceNode>
#include <osgEarth/GLUtils>
#include <drawShape.h>

#include "plugininterface.h"
#include "sphereProperties.h"
#include "sphereNode.h"

#define SPHERE "Sphere"

class DrawSphere : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawSphere.json")
    Q_INTERFACES(PluginInterface)
public:
    DrawSphere(QObject *parent = nullptr);
    virtual bool setup() override;
private slots:
    void onSphereItemCheck(bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;

private:
    osg::ref_ptr<SphereNode> mSphere{nullptr};
    SphereProperties* mSphereProperties{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mSphereLayer;
    static int mCount;
};

#endif // DRAWCIRCLE_H
