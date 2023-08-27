#include "rain.h"
#include <osgEarth/Registry>

namespace osgParticle {
class PrecipitationEffect;
}

Rain::Rain(MapItem *map)
{
    mRain = new osgParticle::PrecipitationEffect;
    mRain->rain(1.0);
    mRain->setUseFarLineSegments(true);
    mRain->getFog()->setDensity(0.012f);
    mRain->getFog()->setMode(osg::Fog::EXP);
    mRain->getFog()->setFogCoordinateSource(osg::Fog::FOG_COORDINATE);

        //    mSnow->setWind(osg::Vec3(1,1,0));
        osgEarth::Registry::shaderGenerator().run(mRain.get());
    map->getMapNode()->addChild(mRain.get());
    map->getMapNode()->getOrCreateStateSet()->setAttributeAndModes(mRain->getFog());

}

osgParticle::PrecipitationEffect *Rain::getRain()
{
    return mRain;
}

