#ifndef SPHERENODE_H
#define SPHERENODE_H

#include <osgEarthAnnotation/GeoPositionNode>
#include <osg/ShapeDrawable>

class SphereNode: public osgEarth::Annotation::GeoPositionNode
{
public:
    enum SphereShape { Sphere, SphereTopHalf, SphereBottomHalf };

    SphereNode();
   const osgEarth::Distance& getRadius() const;
   void setRadius(const osgEarth::Distance& radius);
   void setColor(osgEarth::Color color);
   void setSphereShape(SphereShape sphereShape);
   SphereNode::SphereShape getSphereShape() const;

private:
   void construct();
   void compile();
    osg::Node* installTwoPassAlpha(osg::Node* node);
private:
    osgEarth::Symbology::Style  mStyle;
    osgEarth::Distance mRadius;
    osg::ref_ptr<osg::ShapeDrawable> mShapeDrawable{nullptr};
    osgEarth::Color mColor;
    SphereShape mSphereShape;
};

#endif // SPHERENODE_H
