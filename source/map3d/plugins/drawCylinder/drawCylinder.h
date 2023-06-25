#ifndef DRAWCylinder_H
#define DRAWCylinder_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>

#include "drawShape.h"
#include "cylinder.h"
#include "cylinderProperties.h"

#define CYLINDER "Cylinder"

class DrawCylinder : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawCylinder.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawCylinder(QObject *parent = nullptr);
    bool setup() override;
private slots:
    void onCylinderItemCheck (bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;

private:
    osg::ref_ptr<Cylinder> mCylinder{nullptr};
    CylinderProperties *mCylinderProperties{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mCylinderLayer;
    static int mCount;
};
#endif // DRAWCylinder_H
