#include "wind.h"
#include "autoScaler.h"
#include "qtimer.h"
#include <osgEarth/Registry>
#include <utility.h>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>

WindEffect::WindEffect(MapItem *map, double duration):
    osgEarth::Annotation::ModelNode(map->getMapNode(), Utility::getDefaultStyle())
{
    connect(map, &MapItem::modeChanged, this, &WindEffect::onModeChanged);
    mIs3D = map->getMode();
    //---------------------------------------------------
    mRoot = new osg::Group;
    mSwitchNode = new osg::Switch;
    mRoot->addChild(mSwitchNode);
    osg::ref_ptr<osg::Group> wind3D = new osg::Group;

    //--Wind 3D Node---------------------------------------------------
    mWind = new osgParticle::SmokeEffect(osg::Vec3(0,0,0),10,1);

    mWind->setEmitterDuration(duration);
    mWind->setParticleDuration(duration/6);
    osgEarth::Registry::shaderGenerator().run(mWind->getParticleSystem());
    wind3D->addChild(mWind);
    mWind->setTextureFileName("../data/images/wind_p1.png");
    mWind->setUseLocalParticleSystem(false);
    mWind->setWind(osg::Vec3(30,30,0));
    mWind->setEmitterDuration(10000);
    mWind->setParticleDuration(5);




    //--2D node---------------------------------------------------------
    mImage = osgDB::readImageFile("../data/images/particle/wind.png");
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
        mSwitchNode->addChild(wind3D, true);
        mSwitchNode->addChild(m2DNode, false);
        mRoot->addChild(mWind->getParticleSystem());
    }
    else{
        mSwitchNode->addChild(wind3D, false);
        mSwitchNode->addChild(m2DNode, true);
    }
    osgEarth::Symbology::Style  rootStyle ;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRoot);
    setStyle(rootStyle);
    //--duration---------------------------------------------------------------
    mTimerDuration = new QTimer();
    QObject::connect(mTimerDuration, &QTimer::timeout, [&](){
        mAutoScaler->setScaled(false);
        setCullCallback(nullptr);//cancel auto scale
        mRoot->setNodeMask(false);
        mTimerDuration->stop();
    });
    mTimerDuration->start(duration*1000);
}

WindEffect::~WindEffect()
{
    delete mTimerDuration;
}

void WindEffect::setScaleRatio(double ratio)
{
    if(mAutoScaler)
        mAutoScaler->setDefaultScale(ratio);
}


void WindEffect::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    if(mIs3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
        mRoot->addChild(mWind->getParticleSystem());
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
        mRoot->removeChild(mWind->getParticleSystem());
    }

}

