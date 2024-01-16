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

   connect(serviceManager(),&ServiceManager::explosionDataReceived,this,&Particle::onExplosionDataReceived);

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

bool Particle::mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::None)
        return false;

    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        if (mState == State::Ready || mState == State::Init) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            init(geoPos);
            return true;
        }
//        if (mState == State::Moving) {
//            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
//            moving(geoPos);
//            return true;
//        }
    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::Init)) {
        cancel();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::Init)) {
        confirm();
        return false;
    }
    return false;
}

void Particle::onExplodeClicked(bool check)
{
    if (check) {
        mState = (State::Ready);
        mType = Type::Explosion;
    }
    else {
        if(mState == State::Init)
            cancel();
        mState =State::None;
        mType = Type::None;
    }
}

void Particle::onFireClicked(bool check)
{
    if (check) {

        mState = (State::Ready);
        mType = Type::Fire;
    }
    else {
        if(mState == State::Init)
            cancel();
        mState =State::None;
        mType = Type::None;
    }
}

void Particle::onSnowClicked(bool check)
{
    if (check) {

        mState = (State::Ready);
        mType = Type::Snow;
    }
    else {
        if(mState == State::Init)
            cancel();
        mState =State::None;
        mType = Type::None;
    }
}

void Particle::onRainClicked(bool check)
{
    if (check) {

        mState = (State::Ready);
        mType = Type::Rain;
    }
    else {
        if(mState == State::Init)
            cancel();
        mState =State::None;
        mType = Type::None;
    }
}

void Particle::onCloudClicked(bool check)
{
    if (check) {

        mState = (State::Ready);
        mType = Type::Cloud;
    }
    else {
        if(mState == State::Init)
            cancel();
        mState =State::None;
        mType = Type::None;
    }
}

void Particle::onWindClicked(bool check)
{
    if (check) {

        mState = (State::Ready);
        mType = Type::Wind;
    }
    else {
        if(mState == State::Init)
            cancel();
        mState =State::None;
        mType = Type::None;
    }
}

void Particle::onFogClicked(bool check)
{
    if (check) {
        mState = (State::Ready);
        mType = Type::Fog;
    }
    else {
        if(mState == State::Init)
            cancel();
        mState =State::None;
        mType = Type::None;
    }
}

void Particle::onExplosionDataReceived(const ExplosionData &explosionData)
{
    mExplosion = new Explosion(mapItem(),explosionData.duration);
    mExplosion->setPosition(osgEarth::GeoPoint(mapItem()->getMapSRS(),explosionData.longitude, explosionData.latitude));
    mExplosion->setScaleRatio(explosionData.scale);
    mParticleLayer->addChild(mExplosion);
}

void Particle::init(const osgEarth::GeoPoint &geoPos)
{
    switch (mType) {
    case Type::Fire:
        mFire = new FireSmoke(mapItem());
        mFire->setPosition(geoPos);
        mParticleLayer->addChild(mFire);
        break;
    case Type::Explosion:
        mExplosion = new Explosion(mapItem());
        mExplosion->setScaleRatio(5);
        mExplosion->setPosition(geoPos);
        mParticleLayer->addChild(mExplosion);
        break;
    case Type::Snow:
        mSnow = new Snow(mapItem());
        mSnow->setPosition(geoPos);
        mParticleLayer->addChild(mSnow);
        break;
    case Type::Rain:
        mRain = new Rain(mapItem());
        mRain->setPosition(geoPos);
        mParticleLayer->addChild(mRain);
        break;
    case Type::Cloud:
        mCloud = new Cloud(mapItem());
        mCloud->setPosition(geoPos);
        mParticleLayer->addChild(mCloud);
        break;
    case Type::Wind:
        mWind = new WindEffect(mapItem());
        mWind->setPosition(geoPos);
        mParticleLayer->addChild(mWind);
        break;
    case Type::Fog:
        mFog = new Fog(mapItem());
        mFog->setPosition(geoPos);
        mParticleLayer->addChild(mFog);
        break;
    default:
        break;
    }
    mState = State::Init;
}

//void Particle::moving(const osgEarth::GeoPoint &geoPos)
//{
////    switch (mType) {
////    case Type::Fire:
////        mFire->setPosition(geoPos);
////        break;
////    case Type::Explosion:
////        mExplosion->setPosition(geoPos);
////        break;
////    case Type::Cloud:
////        mCloud->setPosition(geoPos);
////        break;
////    case Type::Wind:
////        mWind->setPosition(geoPos);
////        break;
////    case Type::Snow:
////        mSnow->setPosition(geoPos);
////        break;
////    case Type::Rain:
////        mRain->setPosition(geoPos);
////        break;
////    case Type::Fog:
////        mFog->setPosition(geoPos);
////        break;
////    default:
////        break;
////    }

//}

void Particle::confirm()
{
    if (mState == State::Init) {
        mState = State::Ready;
    }
}

void Particle::cancel(){

    if(mState == State::Init){
        switch (mType) {
        case Type::Fire:
            mParticleLayer->removeChild(mFire);
            break;
        case Type::Explosion:
            mParticleLayer->removeChild(mExplosion);
            break;
        case Type::Cloud:
            mParticleLayer->removeChild(mCloud);
            break;
        case Type::Snow:
            mParticleLayer->removeChild(mSnow);
            break;
        case Type::Rain:
            mParticleLayer->removeChild(mRain);
            break;
        case Type::Wind:
            mParticleLayer->removeChild(mWind);
            break;
        case Type::Fog:
            mParticleLayer->removeChild(mFog);
            break;
        default:
            break;
        }
        mState = State::Ready;
    }
}
