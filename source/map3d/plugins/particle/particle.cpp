#include "particle.h"
#include <osgEarth/GLUtils>
#include <QLabel>
#include <osgEarth/Registry>
#include <osgEarthAnnotation/CircleNode>


const QString CATEGORY  = "Particle" ;
const QString EXPLOSION = "Explosion";
const QString FIRE      = "Fire";
const QString SNOW      = "Snow";
const QString RAIN      = "Rain";
const QString CLOUD     = "Cloud";




using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;


Particle::Particle(QObject *parent): PluginInterface(parent)
{

}


bool Particle::setup()
{

    mParticleLayer = new ParenticAnnotationLayer;
    mParticleLayer->setName(CATEGORY.toStdString());
    mapItem()->getMapObject()->addLayer(mParticleLayer);
    ///////////////////////////add explosion/////////////////////////////////
    auto toolboxItemExplode =  new ToolboxItem{EXPLOSION, CATEGORY, "qrc:/resources/explosion.png", true};
    QObject::connect(toolboxItemExplode, &ToolboxItem::itemChecked, this, &Particle::onExplodeClicked);
    toolbox()->addItem(toolboxItemExplode);

    ///////////////////////////add fire/////////////////////////////////////
    auto toolboxItemFire =  new ToolboxItem{FIRE, CATEGORY, "qrc:/resources/fire.png", true};
    QObject::connect(toolboxItemFire, &ToolboxItem::itemChecked, this, &Particle::onFireClicked);
    toolbox()->addItem(toolboxItemFire);
    ///////////////////////////add snow/////////////////////////////////////
    auto toolboxItemSnow =  new ToolboxItem{SNOW, CATEGORY, "qrc:/resources/snow.png", true};
    QObject::connect(toolboxItemSnow, &ToolboxItem::itemChecked, this, &Particle::onSnowClicked);
    toolbox()->addItem(toolboxItemSnow);
    ///////////////////////////add rain/////////////////////////////////////
    auto toolboxItemRain =  new ToolboxItem{RAIN, CATEGORY, "qrc:/resources/rain.png", true};
    QObject::connect(toolboxItemRain, &ToolboxItem::itemChecked, this, &Particle::onRainClicked);
    toolbox()->addItem(toolboxItemRain);
    ///////////////////////////add cloud/////////////////////////////////////
    auto toolboxItemCloud =  new ToolboxItem{CLOUD, CATEGORY, "qrc:/resources/cloud.png", true};
    QObject::connect(toolboxItemCloud, &ToolboxItem::itemChecked, this, &Particle::onCloudClicked);
    toolbox()->addItem(toolboxItemCloud);

    return true;
}



bool Particle::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;

    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        if (mState == State::READY) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            add(geoPos);
            return true;
        }
        if (mState == State::MOVING) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            moving(geoPos);
            return true;
        }

    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::MOVING)) {
        cancelAdd();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::MOVING)) {
        confirm();
        return false;
    }
    return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////
void Particle::onExplodeClicked(bool check)
{
    if (check) {
        mState = (State::READY);
        mMode = Mode::EXPLOSION;
    }
    else {
        if(mState == State::MOVING)
            cancelAdd();
        mState =State::NONE;
        mMode = Mode::NONE;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////
void Particle::onFireClicked(bool check)
{
    if (check) {

        mState = (State::READY);
        mMode = Mode::FIRE;
    }
    else {
        if(mState == State::MOVING)
            cancelAdd();
        mState =State::NONE;
        mMode = Mode::NONE;
    }
}
/////////////////////////////////////////////////////////////////////////
void Particle::onSnowClicked(bool check)
{
    if (check) {

        mState = (State::READY);
        mMode = Mode::SNOW;
    }
    else {
        if(mState == State::MOVING)
            cancelAdd();
        mState =State::NONE;
        mMode = Mode::NONE;
    }
}
///////////////////////////////////////////////////////////////////////////
void Particle::onRainClicked(bool check)
{
    if (check) {

        mState = (State::READY);
        mMode = Mode::RAIN;
    }
    else {
        if(mState == State::MOVING)
            cancelAdd();
        mState =State::NONE;
        mMode = Mode::NONE;
    }
}
///////////////////////////////////////////////////////////////////////////
void Particle::onCloudClicked(bool check)
{
    if (check) {

        mState = (State::READY);
        mMode = Mode::CLOUD;
    }
    else {
        if(mState == State::MOVING)
            cancelAdd();
        mState =State::NONE;
        mMode = Mode::NONE;
    }
}
///////////////////////////////////////////////////////////////////////////


void Particle::add(const osgEarth::GeoPoint &geoPos)
{
    circle = new osgEarth::Annotation::CircleNode;
    circle->setRadius(osgEarth::Distance(10, osgEarth::Units::KILOMETERS));
    auto style = circle->getStyle();
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osg::Vec4f(0,0,0,0));
    style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->outline() = true;
    style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color(osg::Vec4f(0,1,0,1));
    circle->setStyle(style);

    switch (mMode) {
    case Mode::FIRE:
        mFire = new FireSmoke(mapItem());
        mFire->setPosition(geoPos);
        mParticleLayer->addChild(mFire);
        break;
    case Mode::EXPLOSION:
        mExplosion = new Explosion(mapItem());
        mExplosion->setPosition(geoPos);
        mParticleLayer->addChild(mExplosion);
        break;
    case Mode::SNOW:
        mSnow = new Snow(circle);
        mapItem()->getMapNode()->addChild(circle);
        circle->setPosition(geoPos);
        mParticleLayer->addChild(mSnow);
        break;
    case Mode::RAIN:
        mRain = new Rain(mapItem());
        mParticleLayer->addChild(mRain);
        break;
    case Mode::CLOUD:
        mCloud = new Cloud(mapItem());
        mCloud->setPosition(geoPos);
        mParticleLayer->addChild(mCloud);
        break;
    default:
        break;
    }
    mState = State::MOVING;
}

void Particle::moving(const osgEarth::GeoPoint &geoPos)
{
    switch (mMode) {
    case Mode::FIRE:
        mFire->setPosition(geoPos);
        break;
    case Mode::EXPLOSION:
        mExplosion->setPosition(geoPos);
        break;
    case Mode::CLOUD:
        mCloud->setPosition(geoPos);
        break;
    case Mode::SNOW:
        circle->setPosition(geoPos);
        break;
    default:
        break;
    }

}


void Particle::confirm()
{
    if (mState == State::MOVING) {
        mState = State::READY;
    }
}

void Particle::cancelAdd(){

    if(mState == State::MOVING){
        switch (mMode) {
        case Mode::FIRE:
            mParticleLayer->removeChild(mFire);
            break;
        case Mode::EXPLOSION:
            mParticleLayer->removeChild(mExplosion);
            break;
        case Mode::CLOUD:
            mParticleLayer->removeChild(mCloud);
            break;
        case Mode::SNOW:
            mSnow->removeSnow(mapItem());
            mParticleLayer->removeChild(mSnow);
            break;
        case Mode::RAIN:
            mRain->removeRain(mapItem());
            mParticleLayer->removeChild(mRain);
            break;
        default:
            break;
        }
//

        mState = State::READY;
    }
}
