#include "rain.h"
#include <osgEarth/Registry>

namespace osgParticle {
class PrecipitationEffect;
}

Rain::Rain( MapItem *map)
{
    connect(map, &MapItem::modeChanged, this, &Rain::onModeChanged);
    is3D = map->getMode();
    mSwitchNode = new osg::Switch;

    precipitationRange = new osgEarth::Annotation::CircleNode;
    precipitationRange->setRadius(osgEarth::Distance(10, osgEarth::Units::KILOMETERS));
    auto style = precipitationRange->getStyle();
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osg::Vec4f(0,0,0,0));
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->outline() = true;
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color(osg::Vec4f(0,0.5,0.2,1));
    precipitationRange->setStyle(style);

    //--Rain 3D Node---------------------------------------------------
    mRain = new osgParticle::PrecipitationEffect;
    mRain->rain(1.0);
    mRain->setUseFarLineSegments(true);
    osgEarth::Registry::shaderGenerator().run(mRain.get());
    precipitationRange->addChild(mRain);
//    rangeLayer->addChild(mRain);

    //--2D node---------------------------------------------------------
    m2DNode = new osgEarth::Annotation::PlaceNode();
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("../data/images/particle/rain.png");
    m2DNode->setIconImage(image);

    //--setting--------------------------------------------------------
    if(is3D){
        mSwitchNode->addChild(precipitationRange, true);
        mSwitchNode->addChild(m2DNode, false);
    }
    else{
        mSwitchNode->addChild(precipitationRange, false);
        mSwitchNode->addChild(m2DNode, true);
    }

    getPositionAttitudeTransform()->addChild(mSwitchNode);


}

osgParticle::PrecipitationEffect *Rain::getRain()
{
    return mRain;
}

void Rain::onModeChanged(bool is3DView)
{
    is3D = is3DView;
    if(is3D){
        mSwitchNode->setValue(0,true);
        mSwitchNode->setValue(1, false);
    }
    else{
        mSwitchNode->setValue(0, false);
        mSwitchNode->setValue(1,true);
    }

}




