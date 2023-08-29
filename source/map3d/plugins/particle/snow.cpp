#include "snow.h"
#include <osgEarth/Registry>


namespace osgParticle {
class PrecipitationEffect;
}

Snow::Snow(osgEarth::Annotation::CircleNode *rangeLayer)
{
    mSnow = new osgParticle::PrecipitationEffect;
    mSnow->snow(1.0);
    mSnow->setUseFarLineSegments(true);
    osgEarth::Registry::shaderGenerator().run(mSnow.get());
    rangeLayer->addChild(mSnow);
}

osgParticle::PrecipitationEffect *Snow::getSnow()
{
    return mSnow;
}

