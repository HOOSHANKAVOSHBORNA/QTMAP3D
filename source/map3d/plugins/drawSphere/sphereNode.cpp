#include "sphereNode.h"


#include <osg/LineWidth>
#include <osg/ShapeDrawable>
#include <osg/BlendFunc>
#include <osg/LightModel>
#include <osg/CullFace>
#include <osg/Depth>
#include <osg/ClipNode>

#include <osgEarthAnnotation/AnnotationUtils>

SphereNode::SphereNode(): osgEarth::Annotation::GeoPositionNode()
{
    construct();
    compile();
}

void SphereNode::setColor(osgEarth::Color color)
{
    mColor = color;
    if(mShapeDrawable)
    {
        mShapeDrawable->setColor(color);
        //compile();
    }
}

osgEarth::Color SphereNode::getColor()
{
    if(mShapeDrawable)
    {
       return  mShapeDrawable->getColor();


    }
}

void SphereNode::setSphereShape(SphereNode::SphereShape sphereShape)
{
    if(mSphereShape != sphereShape)
    {
        mSphereShape = sphereShape;
        compile();
    }
}

void SphereNode::construct()
{
    setName("Sphere");
    mRadius.set(1.0, osgEarth::Units::KILOMETERS);
    mColor = osgEarth::Color::White;
    mSphereShape = SphereShape::Sphere;
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

    float radius = static_cast<float>(mRadius.as(osgEarth::Units::METERS));
    osg::ref_ptr<osg::CompositeShape> compositeShape = new osg::CompositeShape;
    osg::ref_ptr<osg::TessellationHints> tessellationHints = new osg::TessellationHints;

    switch (mSphereShape)
    {
    case Sphere:
    {
  //      osg::Vec3 eye(0.f,0.f,radius);
        //mCenter.z() = radius;
        osg::ref_ptr<osg::Sphere> sphereShape = new osg::Sphere(mCenter, radius);
        compositeShape->addChild(sphereShape);
        break;
    }
    case SphereTopHalf:
    {
 //       osg::Vec3 eye(0.f,0.f,0.f);
         //mCenter.z() = 0;
        osg::ref_ptr<osg::Capsule> capsuleShape = new osg::Capsule(mCenter, radius, 0);
        tessellationHints->setCreateBottom(false);
        tessellationHints->setCreateBody(false);

        osg::ref_ptr<osg::Cylinder> cylinderShape = new osg::Cylinder(mCenter, radius, 0);

        compositeShape->addChild(capsuleShape);
        compositeShape->addChild(cylinderShape);
        break;
    }
    case SphereBottomHalf:
    {
//        osg::Vec3 eye(0.f,0.f,radius);
         //mCenter.z() = radius;
        osg::ref_ptr<osg::Capsule> capsuleShape = new osg::Capsule(mCenter, radius, 0);
        tessellationHints->setCreateTop(false);
        tessellationHints->setCreateBody(false);

        osg::ref_ptr<osg::Cylinder> cylinderShape = new osg::Cylinder(mCenter, radius, 0);

        compositeShape->addChild(capsuleShape);
        compositeShape->addChild(cylinderShape);
        break;
    }
    }

    mShapeDrawable = new osg::ShapeDrawable(compositeShape.get(), tessellationHints);
    mShapeDrawable->setColor(mColor);
    mShapeDrawable->setUseVertexBufferObjects(true);

    // construct a local-origin circle.
    if (mShapeDrawable)
    {
        getPositionAttitudeTransform()->addChild(installTwoPassAlpha(mShapeDrawable));
    }
    //    setDefaultLighting( false );
    //    applyRenderSymbology(mStyle);
    //applyStyle(mStyle);
}
osg::Node* SphereNode::installTwoPassAlpha(osg::Node* node)
{
    // first, get the whole thing under a depth-sorted bin:
    osg::Group* g1 = new osg::Group();
    g1->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    g1->getOrCreateStateSet()->setAttributeAndModes( new osg::BlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA), 1);

#ifdef OSG_GL_FIXED_FUNCTION_AVAILABLE
    // for semi-transpareny items, we want the lighting to "shine through"
    osg::LightModel* lm = new osg::LightModel();
    lm->setTwoSided( true );
    g1->getOrCreateStateSet()->setAttributeAndModes( lm );
#endif

    // next start a traversal order bin so we draw in the proper order:
    osg::Group* g2 = new osg::Group();
    g2->getOrCreateStateSet()->setRenderBinDetails(0, "TraversalOrderBin");
    g1->addChild( g2 );

    // next, create a group for the first pass (backfaces only):
    osg::Group* backPass = new osg::Group();
    backPass->getOrCreateStateSet()->setAttributeAndModes( new osg::CullFace(osg::CullFace::FRONT), 1 );
    backPass->getOrCreateStateSet()->setAttributeAndModes( new osg::Depth(osg::Depth::LEQUAL,0,1,false), 1);
    g2->addChild( backPass );

    // and a group for the front-face pass:
    osg::Group* frontPass = new osg::Group();
    frontPass->getOrCreateStateSet()->setAttributeAndModes( new osg::CullFace(osg::CullFace::BACK), 1 );
    g2->addChild( frontPass );

    // finally, attach the geometry to both passes.
    backPass->addChild( node );
    frontPass->addChild( node );

    return g1;
}

SphereNode::SphereShape SphereNode::getSphereShape() const
{
    return mSphereShape;
}

void SphereNode::setCenter(osg::Vec3f center)
{
    if (mCenter != center )
    {
        mCenter = center;
        compile();
    }

}

osg::Vec3f SphereNode::getCenter()
{
    return mCenter;
}
