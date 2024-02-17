#include "cloud.h"
#include "autoScaler.h"
#include "qtimer.h"
#include <osgEarth/Registry>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
#include <osgEarthAnnotation/PlaceNode>


#include <osgEarth/ScreenSpaceLayout>
#include <utility.h>
#include <osgEarthAnnotation/AnnotationUtils>

Cloud::Cloud(MapItem *map, double duration):
    osgEarth::Annotation::ModelNode(map->getMapNode(), Utility::getDefaultStyle())
{
    connect(map, &MapItem::modeChanged, this, &Cloud::onModeChanged);
    mIs3D = map->getMode();
    //---------------------------------------------------
    mRoot = new osg::Group;
    mSwitchNode = new osg::Switch;
    mRoot->addChild(mSwitchNode);
    osg::ref_ptr<osg::Group> fog3D = new osg::Group;

    //--Fog 3D Node---------------------------------------------------
    mCloud1 = new osgParticle::SmokeEffect(osg::Vec3(0,100,100),10,0.08);
    mCloud1->setTextureFileName("../data/images/cloud_p1.png");
    mCloud1->setUseLocalParticleSystem(false);
    osgEarth::Registry::shaderGenerator().run(mCloud1);
    mCloud1->setEmitterDuration(duration);
    mCloud1->setParticleDuration(duration/6);
    osgEarth::Registry::shaderGenerator().run(mCloud1->getParticleSystem());
    fog3D->addChild(mCloud1);


    //--2D node---------------------------------------------------------
    mImage = osgDB::readImageFile("../data/images/particle/cloud.png");
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
        mSwitchNode->insertChild(0, fog3D, true);
        mSwitchNode->insertChild(1, m2DNode, false);
        mRoot->addChild(mCloud1->getParticleSystem());
    }
    else{
        mSwitchNode->insertChild(0, fog3D, false);
        mSwitchNode->insertChild(1, m2DNode, true);
        mRoot->removeChild(mCloud1->getParticleSystem());
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
Cloud::~Cloud(){
    delete mTimerDuration;
}

void Cloud::setScaleRatio(double ratio){
    if(mAutoScaler)
        mAutoScaler->setDefaultScale(ratio);
}
void Cloud::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    if(mIs3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
        mRoot->addChild(mCloud1->getParticleSystem());
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
        mRoot->removeChild(mCloud1->getParticleSystem());
    }

}
