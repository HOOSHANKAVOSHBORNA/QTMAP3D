#ifndef SPHERENODE_H
#define SPHERENODE_H

#include <osgEarthAnnotation/GeoPositionNode>
#include <osg/ShapeDrawable>

class SphereNode: public osgEarth::Annotation::GeoPositionNode
{
public:
    SphereNode();
   const osgEarth::Distance& getRadius() const;
   void setRadius(const osgEarth::Distance& radius);
   void setColor(osgEarth::Color color);
private:
    void construct();
    void compile();
private:
    osgEarth::Symbology::Style  mStyle;
    osgEarth::Distance mRadius;
    osg::ref_ptr<osg::ShapeDrawable> mShapeDrawable{nullptr};
};

#endif // SPHERENODE_H
