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
   osgEarth::Color getColor();
   void setSphereShape(SphereShape sphereShape);
   SphereNode::SphereShape getSphereShape() const;
   void setCenter(osg::Vec3f center);
   osg::Vec3f getCenter();



private:
   void construct();
   void compile();
    osg::Node* installTwoPassAlpha(osg::Node* node);
private:
    osgEarth::Symbology::Style  mStyle;
    osgEarth::Distance mRadius;
    osg::ref_ptr<osg::ShapeDrawable> mShapeDrawable{nullptr};
    osgEarth::Color mColor;
    osg::Vec3d mCenter {osg::Vec3d(0, 0, 0)};

    SphereShape mSphereShape;
};

#endif // SPHERENODE_H
