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
    osgEarth::Registry::shaderGenerator().run(mRain.get());
    map->getMapNode()->addChild(mRain.get());


}

osgParticle::PrecipitationEffect *Rain::getRain()
{
    return mRain;
}

void Rain::removeRain(MapItem *map)
{
    map->getMapNode()->removeChild(mRain.get());
}



