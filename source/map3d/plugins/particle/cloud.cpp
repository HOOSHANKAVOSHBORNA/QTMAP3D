#include "cloud.h"
#include <osgEarth/Registry>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
#include <osgEarthAnnotation/PlaceNode>


Cloud::Cloud(MapItem *map)
{
    connect(map, &MapItem::modeChanged, this, &Cloud::onModeChanged);
    is3D = map->getMode();
    mSwitchNode = new osg::Switch;

    //--Cloud 3D Node---------------------------------------------------
    mCloud1 = new osgParticle::SmokeEffect(osg::Vec3(0,15000,15000),40000,0.4);
    mCloud1->setTextureFileName("../data/images/cloud_p3.png");
    mCloud1->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mCloud1);
    mCloud1->setEmitterDuration(100000);
    mCloud1->setParticleDuration(10);
    osgEarth::Registry::shaderGenerator().run(mCloud1->getParticleSystem());


    //--2D node---------------------------------------------------------
    m2DNode = new osgEarth::Annotation::PlaceNode();
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("../data/images/particle/cloud.png");
    m2DNode->setIconImage(image);

    //--setting--------------------------------------------------------
    if(is3D){
        mSwitchNode->addChild(mCloud1, true);
        mSwitchNode->addChild(m2DNode, false);
        getPositionAttitudeTransform()->addChild(mCloud1->getParticleSystem());
    }
    else{
        mSwitchNode->addChild(mCloud1, false);
        mSwitchNode->addChild(m2DNode, true);
    }

    getPositionAttitudeTransform()->addChild(mSwitchNode);

}

osg::ref_ptr<osgParticle::SmokeEffect> Cloud::getCloud()
{
    return mCloud1;
}

void Cloud::onModeChanged(bool is3DView)
{
    is3D = is3DView;
    if(is3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
        getPositionAttitudeTransform()->addChild(mCloud1->getParticleSystem());
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
        getPositionAttitudeTransform()->removeChild(mCloud1->getParticleSystem());
    }

}

