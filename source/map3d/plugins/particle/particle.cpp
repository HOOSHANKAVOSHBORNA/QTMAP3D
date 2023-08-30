#include "particle.h"
#include <osgEarth/GLUtils>
#include <QLabel>
#include <osgEarth/Registry>
#include <osgEarthAnnotation/CircleNode>


using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;


Particle::Particle(QObject *parent): PluginInterface(parent)
{
}

bool Particle::setup()
{

    mParticleLayer = new ParenticAnnotationLayer;
    mParticleLayer->setName(CATEGORY);
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
    ///////////////////////////add wind/////////////////////////////////////
    auto toolboxItemWind =  new ToolboxItem{WIND, CATEGORY, "qrc:/resources/wind.png", true};
    QObject::connect(toolboxItemWind, &ToolboxItem::itemChecked, this, &Particle::onWindClicked);
    toolbox()->addItem(toolboxItemWind);
    ///////////////////////////add fog/////////////////////////////////////
    auto toolboxItemFog =  new ToolboxItem{FOG, CATEGORY, "qrc:/resources/fog.png", true};
    QObject::connect(toolboxItemFog, &ToolboxItem::itemChecked, this, &Particle::onFogClicked);
    toolbox()->addItem(toolboxItemFog);


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
        mMode = Mode::Explosion;
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
        mMode = Mode::Fire;
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
        mMode = Mode::Snow;
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
        mMode = Mode::Rain;
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
        mMode = Mode::Cloud;
    }
    else {
        if(mState == State::MOVING)
            cancelAdd();
        mState =State::NONE;
        mMode = Mode::NONE;
    }
}
///////////////////////////////////////////////////////////////////////////
void Particle::onWindClicked(bool check)
{
    if (check) {

        mState = (State::READY);
        mMode = Mode::Wind;
    }
    else {
        if(mState == State::MOVING)
            cancelAdd();
        mState =State::NONE;
        mMode = Mode::NONE;
    }
}
///////////////////////////////////////////////////////////////////////////
void Particle::onFogClicked(bool check)
{
    if (check) {
        mState = (State::READY);
        mMode = Mode::Fog;
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


    switch (mMode) {
    case Mode::Fire:
        mFire = new FireSmoke(mapItem());
        mFire->setPosition(geoPos);
        mParticleLayer->addChild(mFire);
        break;
    case Mode::Explosion:
        mExplosion = new Explosion(mapItem());
        mExplosion->setPosition(geoPos);
        mParticleLayer->addChild(mExplosion);
        break;
    case Mode::Snow:
        mSnow = new Snow(mapItem());
        mSnow->setPosition(geoPos);
        mParticleLayer->addChild(mSnow);
        break;
    case Mode::Rain:
        mRain = new Rain(mapItem());
        mRain->setPosition(geoPos);
        mParticleLayer->addChild(mRain);
        break;
    case Mode::Cloud:
        mCloud = new Cloud(mapItem());
        mCloud->setPosition(geoPos);
        mParticleLayer->addChild(mCloud);
        break;
    case Mode::Wind:
        mWind = new WindEffect(mapItem());
        mWind->setPosition(geoPos);
        mParticleLayer->addChild(mWind);
        break;
    case Mode::Fog:
        mFog = new Fog(mapItem());
        mFog->setPosition(geoPos);
        mParticleLayer->addChild(mFog);
        break;
    default:
        break;
    }
    mState = State::MOVING;
}

void Particle::moving(const osgEarth::GeoPoint &geoPos)
{
    switch (mMode) {
    case Mode::Fire:
        mFire->setPosition(geoPos);
        break;
    case Mode::Explosion:
        mExplosion->setPosition(geoPos);
        break;
    case Mode::Cloud:
        mCloud->setPosition(geoPos);
        break;
    case Mode::Wind:
        mWind->setPosition(geoPos);
        break;
    case Mode::Snow:
        mSnow->setPosition(geoPos);
        break;
    case Mode::Rain:
        mRain->setPosition(geoPos);
        break;
    case Mode::Fog:
        mFog->setPosition(geoPos);
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
        case Mode::Fire:
            mParticleLayer->removeChild(mFire);
            break;
        case Mode::Explosion:
            mParticleLayer->removeChild(mExplosion);
            break;
        case Mode::Cloud:
            mParticleLayer->removeChild(mCloud);
            break;
        case Mode::Snow:
            mParticleLayer->removeChild(mSnow);
            break;
        case Mode::Rain:
            mParticleLayer->removeChild(mRain);
            break;
        case Mode::Wind:
            mParticleLayer->removeChild(mWind);
            break;
        case Mode::Fog:
            mParticleLayer->removeChild(mFog);
            break;
        default:
            break;
        }
//

        mState = State::READY;
    }
}
