#include "explosion.h"
#include <osgEarth/Registry>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osgDB/WriteFile>
#include <utility.h>
#include "autoScaler.h"
#include "qtimer.h"
#include <osg/Depth>

Explosion::Explosion(MapItem *map, double duration):
    osgEarth::Annotation::ModelNode(map->getMapNode(), Utility::getDefaultStyle())
{
    connect(map, &MapItem::modeChanged, this, &Explosion::onModeChanged);
    mIs3D = map->getMode();
    //---------------------------------------------------
    mRoot = new osg::Group;
    mSwitchNode = new osg::Switch;
    mRoot->addChild(mSwitchNode);
    osg::ref_ptr<osg::Group> group3D = new osg::Group;

    //--explosion 3D Node---------------------------------------------------
    float scale = 1;
    mExplosion1 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 6.0f * scale, 1.128f);
    mExplosion2 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 4.0f * scale, 1.25f);
    mExplosion3 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 2.0f * scale, 1.5f);
    mExplosion4 = new osgParticle::ExplosionEffect(osg::Vec3(0,0,0), 1.0f * scale, 2.0f);

    mExplosion1->setParticleDuration(1.0);
    mExplosion2->setParticleDuration(1.2);
    mExplosion3->setParticleDuration(1.4);
    mExplosion4->setParticleDuration(1.8);

    mDebris = new osgParticle::ExplosionDebrisEffect(osg::Vec3(0,0,0), 3*scale, 1.0f);
    mSmoke = new osgParticle::SmokeEffect(osg::Vec3(0,0,scale), 10.0f * scale ,0.7);
    mFire = new osgParticle::FireEffect(osg::Vec3(0,0,0),9.0f * scale , 2.0);

    mExplosion1->setUseLocalParticleSystem(false);
    mExplosion2->setUseLocalParticleSystem(false);
    mExplosion3->setUseLocalParticleSystem(false);
    mExplosion4->setUseLocalParticleSystem(false);
    mDebris->setUseLocalParticleSystem(false);
    mSmoke->setUseLocalParticleSystem(false);
    mFire->setUseLocalParticleSystem(false);

    mSmoke->setEmitterDuration(duration);
    mSmoke->setParticleDuration(duration/6);
    mFire->setEmitterDuration(duration/2);
    mFire->setParticleDuration(duration/10);

    mExplosion1->setTextureFileName("../data/images/fire_p1.png");
    mExplosion2->setTextureFileName("../data/images/fire_p2.png");
    mExplosion3->setTextureFileName("../data/images/fire_p3.png");
    mExplosion4->setTextureFileName("../data/images/fire_p4.png");

    mDebris->setTextureFileName("../data/images/debris_p1.png");
    mSmoke->setTextureFileName("../data/images/smoke_p1.png");
    mFire->setTextureFileName("../data/images/fire_p5.png");

    osgEarth::Registry::shaderGenerator().run(mExplosion1);
    osgEarth::Registry::shaderGenerator().run(mExplosion2);
    osgEarth::Registry::shaderGenerator().run(mExplosion3);
    osgEarth::Registry::shaderGenerator().run(mExplosion4);
    osgEarth::Registry::shaderGenerator().run(mDebris);
    osgEarth::Registry::shaderGenerator().run(mSmoke);
    osgEarth::Registry::shaderGenerator().run(mFire);

    osgEarth::Registry::shaderGenerator().run(mExplosion1->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(mExplosion2->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(mExplosion3->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(mExplosion4->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(mDebris->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(mSmoke->getParticleSystem());
    osgEarth::Registry::shaderGenerator().run(mFire->getParticleSystem());


    group3D->addChild(mExplosion1);
    group3D->addChild(mExplosion2);
    group3D->addChild(mExplosion3);
    group3D->addChild(mExplosion3);
    group3D->addChild(mDebris);
    group3D->addChild(mSmoke);
    group3D->addChild(mFire);

    //--2D node---------------------------------------------------------
//    m2DNode = new osgEarth::Annotation::PlaceNode();
    mImage = osgDB::readImageFile("../data/images/particle/explosion.png");
    if(mImage)
        mImage->scaleImage(16, 16, mImage->r());
//    m2DNode->setIconImage(image);

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
        mRoot->addChild(mExplosion1->getParticleSystem());
        mRoot->addChild(mExplosion2->getParticleSystem());
        mRoot->addChild(mExplosion3->getParticleSystem());
        mRoot->addChild(mExplosion4->getParticleSystem());
        mRoot->addChild(mDebris->getParticleSystem());
        mRoot->addChild(mSmoke->getParticleSystem());
        mRoot->addChild(mFire->getParticleSystem());
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

Explosion::~Explosion()
{
    delete mTimerDuration;
}

void Explosion::setScaleRatio(double ratio)
{
    if(mAutoScaler)
        mAutoScaler->setDefaultScale(ratio);
}

void Explosion::onModeChanged(bool mIs3DView)
{
    mIs3D = mIs3DView;
    if(mIs3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
        mRoot->addChild(mExplosion1->getParticleSystem());
        mRoot->addChild(mExplosion2->getParticleSystem());
        mRoot->addChild(mExplosion3->getParticleSystem());
        mRoot->addChild(mExplosion4->getParticleSystem());
        mRoot->addChild(mDebris->getParticleSystem());
        mRoot->addChild(mSmoke->getParticleSystem());
        mRoot->addChild(mFire->getParticleSystem());
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
        mRoot->removeChild(mExplosion1->getParticleSystem());
        mRoot->removeChild(mExplosion2->getParticleSystem());
        mRoot->removeChild(mExplosion3->getParticleSystem());
        mRoot->removeChild(mExplosion4->getParticleSystem());
        mRoot->removeChild(mDebris->getParticleSystem());
        mRoot->removeChild(mSmoke->getParticleSystem());
        mRoot->removeChild(mFire->getParticleSystem());
    }

}
