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
    Sphere(MapController *mapController,float radius);
    void setProp(osg::Vec4 color, osg::Vec3 center, float radius);
    osg::ref_ptr<osgEarth::Annotation::ModelNode> model;

private:
    MapController* mMapController{nullptr};
    osg::ref_ptr<osg::ShapeDrawable> pShapeDrawable;
    osg::ref_ptr<osg::Geode> geode;
    osgEarth::Symbology::Style style;
    osg::ClipNode* clipnode;
};


#endif // SPHERE_H
