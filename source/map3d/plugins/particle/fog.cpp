#include "fog.h"
#include <osgEarth/Registry>


Fog::Fog(MapItem *map)
{
    connect(map, &MapItem::modeChanged, this, &Fog::onModeChanged);
    is3D = map->getMode();
    mSwitchNode = new osg::Switch;

    //--Fog 3D Node---------------------------------------------------
    mFog = new osgParticle::SmokeEffect(osg::Vec3(0,100,100),40000,0.08);
    mFog->setTextureFileName("../data/images/cloud_p1.png");
    mFog->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mFog);
    mFog->setEmitterDuration(100000);
    mFog->setParticleDuration(10);
    osgEarth::Registry::shaderGenerator().run(mFog->getParticleSystem());


    //--2D node---------------------------------------------------------
    m2DNode = new osgEarth::Annotation::PlaceNode();
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("../data/images/particle/fog.png");
    m2DNode->setIconImage(image);

    //--setting--------------------------------------------------------
    if(is3D){
        mSwitchNode->addChild(mFog, true);
        mSwitchNode->addChild(m2DNode, false);
        getPositionAttitudeTransform()->addChild(mFog->getParticleSystem());
    }
    else{
        mSwitchNode->addChild(mFog, false);
        mSwitchNode->addChild(m2DNode, true);
    }

    getPositionAttitudeTransform()->addChild(mSwitchNode);

}

osg::ref_ptr<osgParticle::SmokeEffect> Fog::getFog()
{
    return mFog;
}

void Fog::onModeChanged(bool is3DView)
{
    is3D = is3DView;
    if(is3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
        getPositionAttitudeTransform()->addChild(mFog->getParticleSystem());
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
        getPositionAttitudeTransform()->removeChild(mFog->getParticleSystem());
    }

}

