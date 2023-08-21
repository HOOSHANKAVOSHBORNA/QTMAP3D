#ifndef BOX_H
#define BOX_H

#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "plugininterface.h"
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>


class Box : public osgEarth::Annotation::GeoPositionNode
{

public:
    Box();
    osg::ref_ptr<osgEarth::Annotation::ModelNode> model;
    void construct();
    void compile();


    void setFillColor(osgEarth::Color color);
    void setCenter(osg::Vec3 center);
    osgEarth::Color getFillColor();
    osg::Vec3f getCenter();

    void setHeight(const osgEarth::Linear &height);
    void setWidth(const osgEarth::Linear &width);
    void setLength(const osgEarth::Linear &length);

    const osgEarth::Linear &getHeight() const;
    const osgEarth::Linear &getWidth() const;
    const osgEarth::Linear &getLength() const;

private:
    osg::Node *installTwoPassAlpha(osg::Node *node);



private:
    MapItem* mMapItem{nullptr};
    osgEarth::Symbology::Style style;
    osg::ref_ptr<osg::Box> pBoxShape;
    osgEarth::Symbology::Style  mStyle;
    osgEarth::Color mColor = osgEarth::Color::Blue;
    osg::Vec3d mCenter {osg::Vec3d(0, 0, 0)};
    osg::ref_ptr<osg::ShapeDrawable> mShapeDrawable;
    osgEarth::Distance mRadius ;
    osgEarth::Distance mHeight ;
    osgEarth::Distance mWidth  ;
    osgEarth::Distance mLength ;
};

#endif // BOX_H
