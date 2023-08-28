#include "cloud.h"

#include <osgEarth/Registry>

Cloud::Cloud(MapItem *map)
{

    mCloud = new osgParticle::SmokeEffect(osg::Vec3(0,20000,20000),50000,0.4);
    mCloud->setTextureFileName("../data/images/cloud_p3.png");
//    mCloud->setWind(osg::Vec3(100,0,100));
    mCloud->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mCloud);
    getPositionAttitudeTransform()->addChild(mCloud);
    mCloud->setEmitterDuration(10000);
    mCloud->setParticleDuration(10);
    osgEarth::Registry::shaderGenerator().run(mCloud->getParticleSystem());
    getPositionAttitudeTransform()->addChild(mCloud->getParticleSystem());
}

osg::ref_ptr<osgParticle::SmokeEffect> Cloud::getCloud()
{

    return mCloud;
}

