#ifndef CONE_H
#define CONE_H

#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "plugininterface.h"
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>

class Cone : public osg::ClipNode
{

public:
    Cone(MapController *mapController,float radius, float height, bool clamp);
    osg::ref_ptr<osgEarth::Annotation::ModelNode> model;
    void setColor(osg::Vec4 color);
    void setCenter(osg::Vec3 center);
    void setRadius(float radius);
    void setClamp(bool clamp);
private:
    MapController* mMapController{nullptr};
    osg::ref_ptr<osg::ShapeDrawable> pShapeDrawable;
    osgEarth::Symbology::Style style;
    osg::ref_ptr<osg::Cone> pConeShape;
};

#endif // CONE_H
