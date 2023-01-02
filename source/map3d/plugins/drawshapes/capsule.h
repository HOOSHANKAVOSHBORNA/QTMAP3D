#ifndef CAPSULE_H
#define CAPSULE_H

#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "plugininterface.h"
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>

class Capsule : public osg::ClipNode
{

public:
    Capsule(MapController *mapController,float radius, float height, bool clamp);
    osg::ref_ptr<osgEarth::Annotation::ModelNode> model;
    void setColor(osg::Vec4 color);
    void setCenter(osg::Vec3 center);
    void setRadius(float radius);
    void setClamp(bool clamp);
private:
    MapController* mMapController{nullptr};
    osg::ref_ptr<osg::ShapeDrawable> pShapeDrawable;
    osgEarth::Symbology::Style style;
    osg::ref_ptr<osg::Capsule> pCapsuleShape;
};

#endif // CAPSULE_H
