#ifndef CONE_H
#define CONE_H

#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "plugininterface.h"
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>

class Cone : public osgEarth::Annotation::GeoPositionNode
{

public:
    Cone();

    void compile();

    void setColor(osgEarth::Color color);
    void setCenter(osg::Vec3f center);
    void setRadius(const osgEarth::Linear &radius);
    void setHeight(const osgEarth::Linear &height);

    osgEarth::Color getColor();
    osg::Vec3f getCenter();
    const osgEarth::Linear &getRadius() const;
    const osgEarth::Linear &getHeight() const;

private:
    osg::Node *installTwoPassAlpha(osg::Node *node);

private:
    osg::ref_ptr<osg::ShapeDrawable> mShapeDrawable;
    osgEarth::Symbology::Style mStyle;
    osgEarth::Distance mRadius;
    osgEarth::Distance mHeight;
    osgEarth::Color mColor;
    osg::Vec3 mCenter {osg::Vec3(0, 0, 0)};
};

#endif // CONE_H
