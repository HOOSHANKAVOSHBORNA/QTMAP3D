#include "rain.h"
#include <osgEarth/Registry>

namespace osgParticle {
class PrecipitationEffect;
}

Rain::Rain(osgEarth::Annotation::CircleNode *rangeLayer)
{

    mRain = new osgParticle::PrecipitationEffect;
    mRain->rain(1.0);
    mRain->setUseFarLineSegments(true);
    osgEarth::Registry::shaderGenerator().run(mRain.get());
    rangeLayer->addChild(mRain);

}

osgParticle::PrecipitationEffect *Rain::getRain()
{
    return mRain;
}



