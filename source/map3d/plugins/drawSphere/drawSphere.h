#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include <osgEarthAnnotation/PlaceNode>
#include <osgEarth/GLUtils>
#include <drawShape.h>

#include "plugininterface.h"
#include "sphereProperties.h"
#include "sphereNode.h"

#define SPHERE "Sphere"
class ParenticAnnotationLayer;

class DrawSphere : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawSphere.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawSphere(QObject *parent = nullptr);
    bool setup() override;
private slots:
    void onSphereItemCheck (bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;
    void createProperty();

private:
    osg::ref_ptr<SphereNode> mSphere{nullptr};
    SphereProperties *mSphereProperties{nullptr};
    osg::ref_ptr<CompositeAnnotationLayer> mCompositeSphereLayer;
    osg::ref_ptr<ParenticAnnotationLayer> mSphereLayer = nullptr;
    static int mCount;

};
#endif // DRAWCIRCLE_H
