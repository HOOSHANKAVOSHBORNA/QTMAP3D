#ifndef BOX_H
#define BOX_H


#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "plugininterface.h"
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>

class Box : public osg::ClipNode
{

public:
    Box(MapController *mapController,float length, float width, float height, bool clamp);
    osg::ref_ptr<osgEarth::Annotation::ModelNode> model;
    void setColor(osg::Vec4 color);
    void setCenter(osg::Vec3 center);
    void setRadius(float radius);
    void setClamp(bool clamp);
private:
    MapController* mMapController{nullptr};
    osg::ref_ptr<osg::ShapeDrawable> pShapeDrawable;
    osgEarth::Symbology::Style style;
    osg::ref_ptr<osg::Box> pBoxShape;
};

#endif // BOX_H
