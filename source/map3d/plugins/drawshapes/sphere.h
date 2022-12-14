#ifndef SPHERE_H
#define SPHERE_H
#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "plugininterface.h"
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>

class Sphere : public osg::ClipNode
{

public:
    Sphere(MapController *mapController,float radius, bool clamp);
    void setProp(osg::Vec4 color, osg::Vec3 center, float radius);
    osg::ref_ptr<osgEarth::Annotation::ModelNode> model;
    void setColor(osg::Vec4 color);
    void setCenter(osg::Vec3 center);
    void setRadius(float radius);
    void setClamp(bool clamp);
private:
    MapController* mMapController{nullptr};
    osg::ref_ptr<osg::ShapeDrawable> pShapeDrawable;
    osgEarth::Symbology::Style style;
    osg::ref_ptr<osg::Sphere> pSphereShape;
};


#endif // SPHERE_H
