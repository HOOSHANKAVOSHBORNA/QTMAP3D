#include "snow.h"
#include "autoScaler.h"
#include "qtimer.h"
#include <osgEarth/Registry>

#include <utility.h>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>

namespace osgParticle {
class PrecipitationEffect;
}

Snow::Snow(MapItem *map, double duration):
    osgEarth::Annotation::ModelNode(map->getMapNode(), Utility::getDefaultStyle())
{
    connect(map, &MapItem::modeChanged, this, &Snow::onModeChanged);
    mIs3D = map->getMode();
    //---------------------------------------------------
    mRoot = new osg::Group;
    mSwitchNode = new osg::Switch;
    mRoot->addChild(mSwitchNode);
    osg::ref_ptr<osg::Group> snow3D = new osg::Group;


    mPrecipitationRange = new osgEarth::Annotation::CircleNode;
    mPrecipitationRange->setRadius(osgEarth::Distance(20, osgEarth::Units::METERS));
    auto style = mPrecipitationRange->getStyle();
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osg::Vec4f(0,0,0,0));
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->outline() = true;
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color(osg::Vec4f(0,0.5,0.2,1));
    mPrecipitationRange->setStyle(style);

    //--Snow 3D Node---------------------------------------------------
    mSnow = new osgParticle::PrecipitationEffect;
    mSnow->rain(1.0);
    mSnow->setUseFarLineSegments(true);
    osgEarth::Registry::shaderGenerator().run(mSnow.get());
    mPrecipitationRange->addChild(mSnow);
    snow3D->addChild(mPrecipitationRange);

    //--2D node---------------------------------------------------------
    mImage = osgDB::readImageFile("../data/images/particle/snow.png");
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
        mSwitchNode->addChild(snow3D, true);
        mSwitchNode->addChild(m2DNode, false);
        mRoot->addChild(mSnow.get());
    }
    else{
        mSwitchNode->addChild(snow3D, false);
        mSwitchNode->addChild(m2DNode, true);
    }

    osgEarth::Symbology::Style  rootStyle ;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRoot);
    setStyle(rootStyle);
    //--duration---------------------------------------------------------------
    mTimerDuration = new QTimer();
    QObject::connect(mTimerDuration, &QTimer::timeout, [&](){
        mAutoScaler->setScaled(false);
        setCullCallback(nullptr);
        mRoot->setNodeMask(false);
        mTimerDuration->stop();
    });
    mTimerDuration->start(duration*1000);

}
Snow::~Snow(){
    delete mTimerDuration;
}

void Snow::setScaleRatio(double ratio){
    if(mAutoScaler)
        mAutoScaler->setDefaultScale(ratio);
}
void Snow::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
    if(mIs3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
        mRoot->addChild(mSnow.get());
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
        mRoot->removeChild(mSnow.get());
    }

}




