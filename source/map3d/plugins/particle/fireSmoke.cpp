#include "fireSmoke.h"
#include "autoScaler.h"
#include "qtimer.h"
#include <osg/Depth>
#include <osgEarth/Registry>
#include <utility.h>
#include <osgEarthAnnotation/AnnotationUtils>

FireSmoke::FireSmoke(MapItem *map, double duration):
    osgEarth::Annotation::ModelNode(map->getMapNode(), Utility::getDefaultStyle())
{
    connect(map, &MapItem::modeChanged, this, &FireSmoke::onModeChanged);
    mIs3D = map->getMode();
    mSwitchNode = new osg::Switch;
    //---------------------------------------------------
    mRoot = new osg::Group;
    mSwitchNode = new osg::Switch;
    mRoot->addChild(mSwitchNode);
    osg::ref_ptr<osg::Group> group3D = new osg::Group;

    //--fire 3D Node---------------------------------------------------
    mFire = new osgParticle::FireEffect(osg::Vec3(0,0,0), 9, 1);
    mSmoke = new osgParticle::SmokeEffect(osg::Vec3(0,0,500),10,0.1);
    mSmoke->setTextureFileName("../data/images/smoke_p1.png");
    mFire->setTextureFileName("../data/images/fire_p5.png");
    mFire->setUseLocalParticleSystem(false);
    mSmoke->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mSmoke);
    osgEarth::Registry::shaderGenerator().run(mFire);
    mFire->setEmitterDuration(duration/2);
    mFire->setParticleDuration(duration/10);
    mSmoke->setEmitterDuration(duration);
    mSmoke->setParticleDuration(duration/6);
    osgEarth::Registry::shaderGenerator().run(mSmoke->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(mFire->getParticleSystem());

    group3D->addChild(mSmoke);
    group3D->addChild(mFire);

    //--2D node---------------------------------------------------------
    mImage = osgDB::readImageFile("../data/images/particle/fire.png");
    if(mImage)
        mImage->scaleImage(16, 16, mImage->r());

    m2DNode = new osg::Geode();
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    osgEarth::ScreenSpaceLayoutOptions option;
    option.technique() = osgEarth::ScreenSpaceLayoutOptions::TECHNIQUE_LABELS;
    option.leaderLineMaxLength() = 64;
    option.leaderLineWidth() = 32;
    osgEarth::ScreenSpaceLayout::setOptions(option);
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);
    osg::ref_ptr<osg::Geometry> imgGeom = osgEarth::Annotation::AnnotationUtils::createImageGeometry(mImage, osg::Vec2s(0,0), 0, 0, 1);
    m2DNode->setStateSet(geodeStateSet);
    m2DNode->addDrawable(imgGeom);
    //--Auto scale----------------------------------------------------
    setCullingActive(false);
    mAutoScaler = new AutoScaler(1, 1, 1000);
    setCullCallback(mAutoScaler);

    //--setting--------------------------------------------------------
    if(mIs3D){
        mSwitchNode->insertChild(0, group3D, true);
        mSwitchNode->insertChild(1, m2DNode, false);
        mRoot->addChild(mFire->getParticleSystem());
        mRoot->addChild(mSmoke->getParticleSystem());
    }
    else{
        mSwitchNode->insertChild(0, group3D, false);
        mSwitchNode->insertChild(1, m2DNode, true);
    }

    osgEarth::Symbology::Style  rootStyle ;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRoot);
    setStyle(rootStyle);
    //--duration---------------------------------------------------------------
    mTimerDuration = new QTimer(this);
    QObject::connect(mTimerDuration, &QTimer::timeout, [&](){
        mAutoScaler->setScaled(false);
        setCullCallback(nullptr);//cancel auto scale
        mRoot->setNodeMask(false);
        mTimerDuration->stop();
    });
    mTimerDuration->start(duration*1000);

}

FireSmoke::~FireSmoke()
{
    delete mTimerDuration;
}

void FireSmoke::setScaleRatio(double ratio)
{
    if(mAutoScaler)
        mAutoScaler->setDefaultScale(ratio);
}

void FireSmoke::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    if(mIs3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
        mRoot->addChild(mFire->getParticleSystem());
        mRoot->addChild(mSmoke->getParticleSystem());
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
        mRoot->removeChild(mFire->getParticleSystem());
        mRoot->removeChild(mSmoke->getParticleSystem());
    }

}
