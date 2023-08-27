#include "snow.h"
#include <osgEarth/Registry>

namespace osgParticle {
class PrecipitationEffect;
}

Snow::Snow(MapItem *map)
{
    mSnow = new osgParticle::PrecipitationEffect;
    mSnow->snow(1.0);
    mSnow->setUseFarLineSegments(true);
    mSnow->getFog()->setDensity(0.00001f);
    mSnow->getFog()->setMode(osg::Fog::EXP2);
    mSnow->getFog()->setFogCoordinateSource(osg::Fog::FOG_COORDINATE);
//    mSnow->setWind(osg::Vec3(1,1,0));
    osgEarth::Registry::shaderGenerator().run(mSnow.get());
    map->getMapNode()->addChild(mSnow.get());
    map->getMapNode()->getOrCreateStateSet()->setAttributeAndModes(mSnow->getFog());

}

osgParticle::PrecipitationEffect *Snow::getSnow()
{
    return mSnow;
}
