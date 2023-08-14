#ifndef DRAWCylinder_H
#define DRAWCylinder_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>
#include "drawShape.h"

#define CYLINDER "Cylinder"

class CylinderProperty;
class Cylinder;
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
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;


private:
    osg::ref_ptr<Cylinder> mCylinder{nullptr};
    CylinderProperty *mCylinderProperty{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mCylinderLayer = nullptr;
    static int mCount;
    QQuickItem *mItem;

};
#endif // DRAWCylinder_H
