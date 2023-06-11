#include "box.h"
#include <osgEarth/GLUtils>
#include <osg/BlendFunc>
#include <osg/LightModel>
#include <osg/CullFace>
#include <osg/Depth>
#include <QDebug>

Box::Box()
{
    construct();
    compile();
}

void Box::construct()
{
    setName("Box");
    mColor = osgEarth::Color::White;

}

void Box::compile()
{
    osg::Group* pat = getPositionAttitudeTransform();
    pat->removeChildren(0, pat->getNumChildren());
    float height = static_cast<float>(mHeight.as(osgEarth::Units::METERS));
    float width = static_cast<float>(mWidth.as(osgEarth::Units::METERS));
    float length = static_cast<float>(mLength.as(osgEarth::Units::METERS));
    osg::ref_ptr<osg::CompositeShape> compositeShape = new osg::CompositeShape;
    osg::ref_ptr<osg::TessellationHints> tessellationHints = new osg::TessellationHints;

    osg::ref_ptr<osg::Box> boxShape = new osg::Box(mCenter, length, width, height);
    compositeShape->addChild(boxShape);

    mShapeDrawable = new osg::ShapeDrawable(compositeShape.get(), tessellationHints);
    mShapeDrawable->setColor(mColor);
    mShapeDrawable->setUseVertexBufferObjects(true);

    // construct a local-origin box.
    if (mShapeDrawable)
    {
        getPositionAttitudeTransform()->addChild(installTwoPassAlpha(mShapeDrawable));
    }
}


osg::Node* Box::installTwoPassAlpha(osg::Node* node)
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



void Box::setColor(osgEarth::Color color)
{
    mColor = color;
    if(mShapeDrawable)
    {
        mShapeDrawable->setColor(color);
    }
}

osgEarth::Color Box::getColor()
{
    if(mShapeDrawable)
    {
       return  mShapeDrawable->getColor();
    }
    return mColor;
}

osg::Vec3f Box::getCenter()
{
    return mCenter;
}
void Box::setCenter(osg::Vec3 center)
{
    if (mCenter != center )
    {
        mCenter = center;
        compile();
    }
}

void Box::setHeight(double height)
{
    if(mHeight != height)
    {
        mHeight = height;
        compile();
    }
}
void Box::setWidth(double width)
{
    if(mWidth != width)
    {
        mWidth = width;
        compile();
    }
}
void Box::setLength(double length)
{
    if(mLength != length)
    {
        mLength = length;
        compile();
    }
}



