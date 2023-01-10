#include "spherenode.h"

#include <osg/ShapeDrawable>

SphereNode::SphereNode(): osgEarth::Annotation::GeoPositionNode()
{
    construct();
    compile();
}

void SphereNode::setColor(osgEarth::Color color)
{
    if(mShapeDrawable)
        mShapeDrawable->setColor(color);
}

void SphereNode::construct()
{
    setName("Sphere");
    mRadius.set(3.0, osgEarth::Units::KILOMETERS);
}

const osgEarth::Linear& SphereNode::getRadius() const
{
    return mRadius;
}

void SphereNode::setRadius( const osgEarth::Linear& radius )
{
    if (mRadius != radius )
    {
        mRadius = radius;
        compile();
    }
}

void SphereNode::compile()
{
    // clear out old node:
    osg::Group* pat = getPositionAttitudeTransform();
    pat->removeChildren(0, pat->getNumChildren());

    osg::Vec3 eye(0.f,0.f,0.f);

    osg::ref_ptr<osg::Sphere> sphereShape = new osg::Sphere(eye, mRadius.as(osgEarth::Units::METERS));
    mShapeDrawable = new osg::ShapeDrawable(sphereShape.get());

    // construct a local-origin circle.
    if (mShapeDrawable)
    {
        getPositionAttitudeTransform()->addChild( mShapeDrawable.get() );
    }
    setDefaultLighting( false );
    //applyRenderSymbology(mStyle);
    //applyStyle(mStyle);
}
