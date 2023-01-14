#include "spherenode.h"

#include <osg/LineWidth>
#include <osg/ShapeDrawable>
#include <osg/BlendFunc>
#include <osg/LightModel>
#include <osg/CullFace>
#include <osg/Depth>

#include <osgEarthAnnotation/AnnotationUtils>

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
