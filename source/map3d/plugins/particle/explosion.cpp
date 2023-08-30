#include "explosion.h"
#include <osgEarth/Registry>

Explosion::Explosion(MapItem *map)
{
    connect(map, &MapItem::modeChanged, this, &Explosion::onModeChanged);
    is3D = map->getMode();
    mSwitchNode = new osg::Switch;

    //--explosion 3D Node---------------------------------------------------
    pSphereGroup = new osg::PositionAttitudeTransform;
    float scale = 100;
    explosion1 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 6.0f * scale, 1.128f);
    explosion2 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 4.0f * scale, 1.25f);
    explosion3 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 2.0f * scale, 1.5f);
    explosion4 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 1.0f * scale, 2.0f);

    explosion1->setParticleDuration(1.0);
    explosion2->setParticleDuration(1.2);
    explosion3->setParticleDuration(1.4);
    explosion4->setParticleDuration(1.8);

    debris1 = new osgParticle::ExplosionDebrisEffect(osg::Vec3(0,0,0), 10.0f * scale, 1.0f);
    smoke = new osgParticle::SmokeEffect(osg::Vec3(0,0,scale), 18.0f * scale ,0.2);
    fire = new osgParticle::FireEffect(osg::Vec3(0,0,0),9.0f * scale , 2.0);

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
    fire->setTextureFileName("../data/images/fire_p5.png");

//    pSphereGroup->addChild(explosion1);
//    pSphereGroup->addChild(explosion2);
//    pSphereGroup->addChild(explosion3);
//    pSphereGroup->addChild(explosion4);
//    pSphereGroup->addChild(debris1);
//    pSphereGroup->addChild(fire);
//    pSphereGroup->addChild(smoke);


    osgEarth::Registry::shaderGenerator().run(explosion1);
    osgEarth::Registry::shaderGenerator().run(explosion2);
    osgEarth::Registry::shaderGenerator().run(explosion3);
    osgEarth::Registry::shaderGenerator().run(explosion4);
    osgEarth::Registry::shaderGenerator().run(debris1);
    osgEarth::Registry::shaderGenerator().run(smoke);
    osgEarth::Registry::shaderGenerator().run(fire);

//    getPositionAttitudeTransform()->addChild(explosion1);
//    getPositionAttitudeTransform()->addChild(explosion2);
//    getPositionAttitudeTransform()->addChild(explosion3);
//    getPositionAttitudeTransform()->addChild(explosion4);
//    getPositionAttitudeTransform()->addChild(debris1);
//    getPositionAttitudeTransform()->addChild(smoke);
//    getPositionAttitudeTransform()->addChild(fire);

    osgEarth::Registry::shaderGenerator().run(explosion1->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(explosion2->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(explosion3->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(explosion4->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(debris1->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(smoke->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(fire->getParticleSystem());




    //--2D node---------------------------------------------------------
    m2DNode = new osgEarth::Annotation::PlaceNode();
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("../data/images/particle/explosion.png");
    m2DNode->setIconImage(image);

    //--setting--------------------------------------------------------
    if(is3D){
        mSwitchNode->addChild(explosion1, true);
        mSwitchNode->addChild(explosion2, true);
        mSwitchNode->addChild(explosion3, true);
        mSwitchNode->addChild(explosion4, true);
        mSwitchNode->addChild(debris1, true);
        mSwitchNode->addChild(smoke, true);
        mSwitchNode->addChild(fire, true);
        mSwitchNode->addChild(m2DNode, false);
        getPositionAttitudeTransform()->addChild(explosion1->getParticleSystem());
        getPositionAttitudeTransform()->addChild(explosion2->getParticleSystem());
        getPositionAttitudeTransform()->addChild(explosion3->getParticleSystem());
        getPositionAttitudeTransform()->addChild(explosion4->getParticleSystem());
        getPositionAttitudeTransform()->addChild(debris1->getParticleSystem());
        getPositionAttitudeTransform()->addChild(smoke->getParticleSystem());
        getPositionAttitudeTransform()->addChild(fire->getParticleSystem());
    }
    else{
        mSwitchNode->addChild(explosion1, false);
        mSwitchNode->addChild(explosion2, false);
        mSwitchNode->addChild(explosion3, false);
        mSwitchNode->addChild(explosion4, false);
        mSwitchNode->addChild(debris1, false);
        mSwitchNode->addChild(smoke, false);
        mSwitchNode->addChild(fire, false);
        mSwitchNode->addChild(m2DNode, true);
    }

    getPositionAttitudeTransform()->addChild(mSwitchNode);
}

osg::PositionAttitudeTransform *Explosion::getExplosion()
{
    return pSphereGroup;
}

void Explosion::onModeChanged(bool is3DView)
{
    is3D = is3DView;
    if(is3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1,true);
        mSwitchNode->setValue(2,true);
        mSwitchNode->setValue(3,true);
        mSwitchNode->setValue(4,true);
        mSwitchNode->setValue(5,true);
        mSwitchNode->setValue(6,true);
        mSwitchNode->setValue(7, false);
        getPositionAttitudeTransform()->addChild(explosion1->getParticleSystem());
        getPositionAttitudeTransform()->addChild(explosion2->getParticleSystem());
        getPositionAttitudeTransform()->addChild(explosion3->getParticleSystem());
        getPositionAttitudeTransform()->addChild(explosion4->getParticleSystem());
        getPositionAttitudeTransform()->addChild(debris1->getParticleSystem());
        getPositionAttitudeTransform()->addChild(smoke->getParticleSystem());
        getPositionAttitudeTransform()->addChild(fire->getParticleSystem());
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1, false);
        mSwitchNode->setValue(2, false);
        mSwitchNode->setValue(3, false);
        mSwitchNode->setValue(4, false);
        mSwitchNode->setValue(5, false);
        mSwitchNode->setValue(6, false);
        mSwitchNode->setValue(7,true);
        getPositionAttitudeTransform()->removeChild(explosion1->getParticleSystem());
        getPositionAttitudeTransform()->removeChild(explosion2->getParticleSystem());
        getPositionAttitudeTransform()->removeChild(explosion3->getParticleSystem());
        getPositionAttitudeTransform()->removeChild(explosion4->getParticleSystem());
        getPositionAttitudeTransform()->removeChild(debris1->getParticleSystem());
        getPositionAttitudeTransform()->removeChild(smoke->getParticleSystem());
        getPositionAttitudeTransform()->removeChild(fire->getParticleSystem());
    }

}
