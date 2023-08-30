#include "fireSmoke.h"
#include <osgEarth/Registry>

FireSmoke::FireSmoke(MapItem *map)
{
    connect(map, &MapItem::modeChanged, this, &FireSmoke::onModeChanged);
    is3D = map->getMode();
    mSwitchNode = new osg::Switch;

    //--fire 3D Node---------------------------------------------------
    mFire = new osgParticle::FireEffect(osg::Vec3(0,0,0), 1000, 1);
    mSmoke = new osgParticle::SmokeEffect(osg::Vec3(0,0,500),2000,0.1);
    mSmoke->setTextureFileName("../data/images/smoke_p1.png");
    mFire->setTextureFileName("../data/images/fire_p5.png");
    mFire->setUseLocalParticleSystem(false);
    mSmoke->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mSmoke);
    osgEarth::Registry::shaderGenerator().run(mFire);
    mFire->setEmitterDuration(50);
    mSmoke->setEmitterDuration(80);
    mFire->setParticleDuration(7);
    mSmoke->setParticleDuration(10);
    osgEarth::Registry::shaderGenerator().run(mSmoke->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(mFire->getParticleSystem());


    //--2D node---------------------------------------------------------
    m2DNode = new osgEarth::Annotation::PlaceNode();
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("../data/images/particle/fire.png");
    m2DNode->setIconImage(image);

    //--setting--------------------------------------------------------
    if(is3D){
        mSwitchNode->addChild(mFire, true);
        mSwitchNode->addChild(mSmoke, true);
        mSwitchNode->addChild(m2DNode, false);
        getPositionAttitudeTransform()->addChild(mFire->getParticleSystem());
        getPositionAttitudeTransform()->addChild(mSmoke->getParticleSystem());
    }
    else{
        mSwitchNode->addChild(mFire, false);
        mSwitchNode->addChild(mSmoke, false);
        mSwitchNode->addChild(m2DNode, true);
    }

    getPositionAttitudeTransform()->addChild(mSwitchNode);

}

osg::PositionAttitudeTransform *FireSmoke::getFireSmoke()
{
    osg::PositionAttitudeTransform *mFireSmoke = new osg::PositionAttitudeTransform;
    mFireSmoke->addChild(mFire);
    mFireSmoke->addChild(mSmoke);
    return mFireSmoke;
}


void FireSmoke::onModeChanged(bool is3DView)
{
    is3D = is3DView;
    if(is3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1,true);
        mSwitchNode->setValue(2, false);
        getPositionAttitudeTransform()->addChild(mFire->getParticleSystem());
        getPositionAttitudeTransform()->addChild(mSmoke->getParticleSystem());
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1, false);
        mSwitchNode->setValue(2,true);
        getPositionAttitudeTransform()->removeChild(mFire->getParticleSystem());
        getPositionAttitudeTransform()->removeChild(mSmoke->getParticleSystem());
    }

}
