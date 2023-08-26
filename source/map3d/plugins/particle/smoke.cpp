#include "smoke.h"
#include <osgEarth/Registry>
#include <osgParticle/FireEffect>

Smoke::Smoke(MapItem *map)
{
    mSmoke = new osgParticle::SmokeEffect(osg::Vec3(0,0,0), 1000, 10000);
    mSmoke->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mSmoke);
    getPositionAttitudeTransform()->addChild(mSmoke);
    mSmoke->setEmitterDuration(1000);
    mSmoke->setParticleDuration(5);
    osgEarth::Registry::shaderGenerator().run(mSmoke->getParticleSystem());
    getPositionAttitudeTransform()->addChild(mSmoke->getParticleSystem());
}

osgParticle::SmokeEffect *Smoke::getSmoke()
{
    return mSmoke;
}


