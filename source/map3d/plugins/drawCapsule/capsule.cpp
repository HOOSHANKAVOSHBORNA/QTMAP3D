#include "capsule.h"
#include <osgEarth/GLUtils>
#include <osg/BlendFunc>
#include <osg/LightModel>
#include <osg/CullFace>
#include <osg/Depth>
#include "mapcontroller.h"
#include <QDebug>

Capsule::Capsule()
{
    setName("Capsule");
    mRadius.set(1.0, osgEarth::Units::KILOMETERS);
    mColor = osgEarth::Color::White;
    compile();
}

void Capsule::compile()
{
    osg::Group* pat = getPositionAttitudeTransform();
    pat->removeChildren(0, pat->getNumChildren());
    float radius = static_cast<float>(mRadius.as(osgEarth::Units::METERS));
    float height = static_cast<float>(mHeight.as(osgEarth::Units::METERS));
    mCenter = osg::Vec3(0.f,0.f,radius + height);
    osg::ref_ptr<osg::CompositeShape> compositeShape = new osg::CompositeShape;
    osg::ref_ptr<osg::TessellationHints> tessellationHints = new osg::TessellationHints;

    osg::ref_ptr<osg::Capsule> capsuleShape = new osg::Capsule(mCenter, radius, height);
    compositeShape->addChild(capsuleShape);

    mShapeDrawable = new osg::ShapeDrawable(compositeShape.get(), tessellationHints);
    mShapeDrawable->setColor(mColor);
    mShapeDrawable->setUseVertexBufferObjects(true);

    // construct a local-origin circle.
    if (mShapeDrawable)
    {
        getPositionAttitudeTransform()->addChild(installTwoPassAlpha(mShapeDrawable));
    }

}

osg::Node* Capsule::installTwoPassAlpha(osg::Node* node)
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
    frontPass->getOrCreateStateSet()->setAttributeAndModes( new osg::Depth(osg::Depth::LEQUAL,0,1,false), 1);
    g2->addChild( frontPass );

    // finally, attach the geometry to both passes.
    backPass->addChild( node );
    frontPass->addChild( node );

    return g1;
}

void Capsule::setColor(osgEarth::Color color)
{
    mColor = color;
    if(mShapeDrawable)
    {
        mShapeDrawable->setColor(color);
        //compile();
    }
}

osgEarth::Color Capsule::getColor()
{
    if(mShapeDrawable)
    {
       return  mShapeDrawable->getColor();


    }
}

void Capsule::setCenter(osg::Vec3f center)
{
    if (mCenter != center )
    {
        mCenter = center;
        compile();
    }

}

osg::Vec3f Capsule::getCenter()
{
    return mCenter;
}

void Capsule::setRadius( const osgEarth::Linear& radius )
{
    if (mRadius != radius )
    {
        mRadius = radius;
        compile();
    }
}

void Capsule::setHeight(const osgEarth::Linear &height)
{
    if (mHeight != height) {
        mHeight = height;
        compile();
    }
}

const osgEarth::Linear& Capsule::getRadius() const
{
    return mRadius;
}

const osgEarth::Linear &Capsule::getHeight() const
{
    return mHeight;
}
