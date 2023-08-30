#include "wind.h"


#include <osgEarth/Registry>

WindEffect::WindEffect(MapItem *map)
{
    connect(map, &MapItem::modeChanged, this, &WindEffect::onModeChanged);
    is3D = map->getMode();
    mSwitchNode = new osg::Switch;

    //--Wind 3D Node---------------------------------------------------
    mWind = new osgParticle::SmokeEffect(osg::Vec3(0,0,0),100,1);
    mWind->setTextureFileName("../data/images/wind_p1.png");
    mWind->setUseLocalParticleSystem(false);
    mWind->setWind(osg::Vec3(30,30,0));
    osgEarth::Registry::shaderGenerator().run(mWind);
    mWind->setEmitterDuration(10000);
    mWind->setParticleDuration(5);
    osgEarth::Registry::shaderGenerator().run(mWind->getParticleSystem());



    //--2D node---------------------------------------------------------
    m2DNode = new osgEarth::Annotation::PlaceNode();
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("../data/images/particle/wind.png");
    m2DNode->setIconImage(image);

    //--setting--------------------------------------------------------
    if(is3D){
        mSwitchNode->addChild(mWind, true);
        mSwitchNode->addChild(m2DNode, false);
        getPositionAttitudeTransform()->addChild(mWind->getParticleSystem());
    }
    else{
        mSwitchNode->addChild(mWind, false);
        mSwitchNode->addChild(m2DNode, true);
    }

    getPositionAttitudeTransform()->addChild(mSwitchNode);

}

osg::ref_ptr<osgParticle::SmokeEffect> WindEffect::getWind()
{

    return mWind;
}


void WindEffect::onModeChanged(bool is3DView)
{
    is3D = is3DView;
    if(is3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
        getPositionAttitudeTransform()->addChild(mWind->getParticleSystem());
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
        getPositionAttitudeTransform()->removeChild(mWind->getParticleSystem());
    }

}

