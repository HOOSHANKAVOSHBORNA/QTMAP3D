#include "explosion.h"
#include <osgEarth/Registry>

Explosion::Explosion(MapItem *map)
{

    float scale = 100;
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion1 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 8.0f * scale, 0.128f);
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion2 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 4.0f * scale, 0.25f);
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion3 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 2.0f * scale, 0.5f);
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion4 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 1.0f * scale, 1.0f);

    explosion1->setParticleDuration(2.0);
    explosion2->setParticleDuration(2.2);
    explosion3->setParticleDuration(2.4);
    explosion4->setParticleDuration(2.8);

    osg::ref_ptr<osgParticle::ExplosionDebrisEffect> debris1 = new osgParticle::ExplosionDebrisEffect(osg::Vec3(0,0,0), 10.0f * scale, 1.0f);
    osg::ref_ptr<osgParticle::SmokeEffect> smoke = new osgParticle::SmokeEffect(osg::Vec3(0,0,0), 10.0f * scale ,2.0);
    osg::ref_ptr<osgParticle::FireEffect> fire = new osgParticle::FireEffect(osg::Vec3(0,0,0),10.0f * scale , 2.0);

    explosion1->setUseLocalParticleSystem(false);
    explosion2->setUseLocalParticleSystem(false);
    explosion3->setUseLocalParticleSystem(false);
    explosion4->setUseLocalParticleSystem(false);
    debris1->setUseLocalParticleSystem(false);
    smoke->setUseLocalParticleSystem(false);
    fire->setUseLocalParticleSystem(false);



    smoke->setEmitterDuration(30.6);
    smoke->setParticleDuration(10.6);
    fire->setEmitterDuration(15.9);
    fire->setParticleDuration(7.5);

    explosion1->setTextureFileName("../data/images/fire_p1.png");
    explosion2->setTextureFileName("../data/images/fire_p2.png");
    explosion3->setTextureFileName("../data/images/fire_p3.png");
    explosion4->setTextureFileName("../data/images/fire_p4.png");

    debris1->setTextureFileName("../data/images/debris_p1.png");
    smoke->setTextureFileName("../data/images/smoke_p1.png");
    fire->setTextureFileName("../data/images/fire_p1.png");

    pSphereGroup->addChild(explosion1);
    pSphereGroup->addChild(explosion2);
    pSphereGroup->addChild(explosion3);
    pSphereGroup->addChild(explosion4);
    pSphereGroup->addChild(debris1);
    pSphereGroup->addChild(fire);
    pSphereGroup->addChild(smoke);


    osgEarth::Registry::shaderGenerator().run(explosion1);
    osgEarth::Registry::shaderGenerator().run(explosion2);
    osgEarth::Registry::shaderGenerator().run(explosion3);
    osgEarth::Registry::shaderGenerator().run(explosion4);
    osgEarth::Registry::shaderGenerator().run(debris1);
    osgEarth::Registry::shaderGenerator().run(smoke);
    osgEarth::Registry::shaderGenerator().run(fire);

    getPositionAttitudeTransform()->addChild(explosion1);
    getPositionAttitudeTransform()->addChild(explosion2);
    getPositionAttitudeTransform()->addChild(explosion3);
    getPositionAttitudeTransform()->addChild(explosion4);
    getPositionAttitudeTransform()->addChild(debris1);
    getPositionAttitudeTransform()->addChild(smoke);
    getPositionAttitudeTransform()->addChild(fire);

    osgEarth::Registry::shaderGenerator().run(explosion1->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(explosion2->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(explosion3->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(explosion4->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(debris1->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(smoke->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(fire->getParticleSystem());


    getPositionAttitudeTransform()->addChild(explosion1->getParticleSystem());
    getPositionAttitudeTransform()->addChild(explosion2->getParticleSystem());
    getPositionAttitudeTransform()->addChild(explosion3->getParticleSystem());
    getPositionAttitudeTransform()->addChild(explosion4->getParticleSystem());
    getPositionAttitudeTransform()->addChild(debris1->getParticleSystem());
    getPositionAttitudeTransform()->addChild(smoke->getParticleSystem());
    getPositionAttitudeTransform()->addChild(fire->getParticleSystem());

}

osg::PositionAttitudeTransform *Explosion::getExplosion()
{
    return pSphereGroup;
}
