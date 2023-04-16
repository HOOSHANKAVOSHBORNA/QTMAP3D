#include "cone.h"
#include <osgEarth/GLUtils>
#include <osg/BlendFunc>
#include <osg/LightModel>
#include <osg/CullFace>
#include <osg/Depth>
#include "mapcontroller.h"
#include <QDebug>

Cone::Cone()
{
    setName("Cone");
    mRadius.set(1.0, osgEarth::Units::KILOMETERS);
    mColor = osgEarth::Color::White;
    compile();
}

void Cone::compile()
{
    osg::Group* pat = getPositionAttitudeTransform();
    pat->removeChildren(0, pat->getNumChildren());

    float radius = static_cast<float>(mRadius.as(osgEarth::Units::METERS));
    float height = static_cast<float>(mHeight.as(osgEarth::Units::METERS));
    osg::ref_ptr<osg::CompositeShape> compositeShape = new osg::CompositeShape;
    osg::ref_ptr<osg::TessellationHints> tessellationHints = new osg::TessellationHints;

    osg::ref_ptr<osg::Cone> coneShape = new osg::Cone(mCenter, radius, height);
    compositeShape->addChild(coneShape);

    mShapeDrawable = new osg::ShapeDrawable(compositeShape.get(), tessellationHints);
    mShapeDrawable->setColor(mColor);
    mShapeDrawable->setUseVertexBufferObjects(true);

    // construct a local-origin circle.
    if (mShapeDrawable)
    {
        getPositionAttitudeTransform()->addChild(installTwoPassAlpha(mShapeDrawable));
    }

}

osg::Node* Cone::installTwoPassAlpha(osg::Node* node)
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

void Cone::setColor(osgEarth::Color color)
{
    mColor = color;
    if(mShapeDrawable)
    {
        mShapeDrawable->setColor(color);
        //compile();
    }
}

osgEarth::Color Cone::getColor()
{
    if(mShapeDrawable)
    {
       return  mShapeDrawable->getColor();


    }
}

void Cone::setCenter(osg::Vec3f center)
{
    if (mCenter != center )
    {
        mCenter = center;
        compile();
    }

}

osg::Vec3f Cone::getCenter()
{
    return mCenter;
}

void Cone::setRadius( const osgEarth::Linear& radius )
{
    if (mRadius != radius )
    {
        mRadius = radius;
        compile();
    }
}

void Cone::setHeight(const osgEarth::Linear &height)
{
    if (mHeight != height) {
        mHeight = height;
        compile();
    }
}

const osgEarth::Linear& Cone::getRadius() const
{
    return mRadius;
}

const osgEarth::Linear &Cone::getHeight() const
{
    return mHeight;
}
