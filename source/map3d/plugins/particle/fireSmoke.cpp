#include "fireSmoke.h"
#include <osgEarth/Registry>

FireSmoke::FireSmoke(MapItem *map)
{
    mFire = new osgParticle::FireEffect(osg::Vec3(0,0,0), 1000, 1);
    mSmoke = new osgParticle::SmokeEffect(osg::Vec3(0,0,500),2000,0.2);
    mSmoke->setTextureFileName("../data/images/smoke_p1.png");
    mFire->setTextureFileName("../data/images/fire_p5.png");
    mFire->setUseLocalParticleSystem(false);
    mSmoke->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mSmoke);
    osgEarth::Registry::shaderGenerator().run(mFire);
    getPositionAttitudeTransform()->addChild(mSmoke);
    getPositionAttitudeTransform()->addChild(mFire);

    mFire->setEmitterDuration(50);
    mSmoke->setEmitterDuration(80);
    mFire->setParticleDuration(5);
    mSmoke->setParticleDuration(20);
    osgEarth::Registry::shaderGenerator().run(mSmoke->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(mFire->getParticleSystem());
    getPositionAttitudeTransform()->addChild(mSmoke->getParticleSystem());
    getPositionAttitudeTransform()->addChild(mFire->getParticleSystem());
}

osg::PositionAttitudeTransform *FireSmoke::getFireSmoke()
{
    osg::PositionAttitudeTransform *mFireSmoke = new osg::PositionAttitudeTransform;
    mFireSmoke->addChild(mFire);
    mFireSmoke->addChild(mSmoke);
    return mFireSmoke;
}

