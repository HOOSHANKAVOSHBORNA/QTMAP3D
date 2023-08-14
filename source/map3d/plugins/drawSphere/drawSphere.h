#ifndef DRAWSPHERE_H
#define DRAWSPHERE_H

#include <osgEarthAnnotation/PlaceNode>
#include <osgEarth/GLUtils>
#include <drawShape.h>

#include "plugininterface.h"
#include "sphereProperty.h"
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

private:
    osg::ref_ptr<SphereNode> mSphere{nullptr};
    SphereProperty *mSphereProperty{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mSphereLayer ;
    static int mCount;
    QQuickItem *mItem;

};
#endif // DRAWSPHERE_H
