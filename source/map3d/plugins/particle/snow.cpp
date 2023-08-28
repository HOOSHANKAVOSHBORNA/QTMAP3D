#include "snow.h"
#include <osgEarth/Registry>

namespace osgParticle {
class PrecipitationEffect;
}

Snow::Snow(osgEarth::Annotation::CircleNode *map)
{
    mSnow = new osgParticle::PrecipitationEffect;
    mSnow->snow(1.0);
    mSnow->setUseFarLineSegments(true);
    osgEarth::Registry::shaderGenerator().run(mSnow.get());



    map->addChild(mSnow);


}

osgParticle::PrecipitationEffect *Snow::getSnow()
{
    return mSnow;
}

void Snow::removeSnow(MapItem *map)
{
    map->getMapNode()->removeChild(mSnow.get());
}
