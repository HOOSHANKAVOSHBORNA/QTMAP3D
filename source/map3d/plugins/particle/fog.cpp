#include "fog.h"

#include <osgEarth/Registry>
#include <osgEarth/Utils>


Fog::Fog(MapItem *map)
{
    mFog = new osg::Fog();
    mFogEffect = new osgParticle::PrecipitationEffect;
    mFogEffect->setFog(mFog);
    mFogEffect->setUseFarLineSegments(true);
    mFog->setMode( osg::Fog::Mode::LINEAR );
    mFog->setUseRadialFog(true);
    mFog->setDensity(10);
    mFog->setFogCoordinateSource(osg::Fog::FRAGMENT_DEPTH);
    mFog->setStart( 500.0f );
    mFog->setEnd( 2500.0f );
    mFog->setColor( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    map->getMapNode()->getOrCreateStateSet()->setAttributeAndModes( mFog.get() );
    osg::StateSet* fogState = new osg::StateSet();
    fogState->setAttributeAndModes(mFog, osg::StateAttribute::FOG);
    fogState->setMode(GL_FOG, osg::StateAttribute::OVERRIDE |osg::StateAttribute::ON);
    map->getMapNode()->setStateSet( fogState );
    osgEarth::Registry::shaderGenerator().run(mFogEffect.get());
    map->getMapNode()->addChild(mFogEffect.get());









}

osg::Fog *Fog::getFog()
{
//    return mFog;
}

