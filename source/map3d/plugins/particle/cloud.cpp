#include "cloud.h"

#include <osgEarth/Registry>

Cloud::Cloud(MapItem *map)
{
    mCloud1 = new osgParticle::SmokeEffect(osg::Vec3(0,15000,15000),40000,0.4);
    mCloud1->setTextureFileName("../data/images/cloud_p3.png");
    mCloud1->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mCloud1);
    getPositionAttitudeTransform()->addChild(mCloud1);
    mCloud1->setEmitterDuration(10000);
    mCloud1->setParticleDuration(10);
    osgEarth::Registry::shaderGenerator().run(mCloud1->getParticleSystem());
    getPositionAttitudeTransform()->addChild(mCloud1->getParticleSystem());


    mCloud2 = new osgParticle::SmokeEffect(osg::Vec3(40000,20000,30000),40000,0.4);
    mCloud2->setTextureFileName("../data/images/cloud_p2.png");
    mCloud2->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mCloud2);
    getPositionAttitudeTransform()->addChild(mCloud2);
    mCloud2->setEmitterDuration(10000);
    mCloud2->setParticleDuration(10);
    osgEarth::Registry::shaderGenerator().run(mCloud2->getParticleSystem());
    getPositionAttitudeTransform()->addChild(mCloud2->getParticleSystem());


    mCloud3 = new osgParticle::SmokeEffect(osg::Vec3(-30000,15000,50000),40000,0.4);
    mCloud3->setTextureFileName("../data/images/cloud_p1.png");
    mCloud3->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mCloud3);
    getPositionAttitudeTransform()->addChild(mCloud3);
    mCloud3->setEmitterDuration(10000);
    mCloud3->setParticleDuration(10);
    osgEarth::Registry::shaderGenerator().run(mCloud3->getParticleSystem());
    getPositionAttitudeTransform()->addChild(mCloud3->getParticleSystem());
}

osg::ref_ptr<osgParticle::SmokeEffect> Cloud::getCloud()
{

    return mCloud1;
}

