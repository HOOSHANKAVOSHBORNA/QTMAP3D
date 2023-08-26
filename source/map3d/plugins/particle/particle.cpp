#include "particle.h"
#include <osgEarth/GLUtils>
#include <QLabel>
#include <osgEarth/Registry>


const QString CATEGORY = "Particle";
const QString EXPLOSION = "Explosion";
const QString FIRE = "Fire";


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

        if (mState == State::ADDING) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            moving(geoPos);
            return true;
        }

    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::ADDING)) {
        cancelAdd();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::ADDING)) {
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
        if(mState == State::ADDING)
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
        if(mState == State::ADDING)
            cancelAdd();
        mState =State::NONE;
        mMode = Mode::NONE;
    }
}


void Particle::add(const osgEarth::GeoPoint &geoPos)
{
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
    default:
        break;
    }
    mState = State::ADDING;
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
    default:
        break;
    }

}


void Particle::confirm()
{
    if (mState == State::ADDING) {
        mState = State::READY;
    }
}

void Particle::cancelAdd(){

    if(mState == State::ADDING){
        switch (mMode) {
        case Mode::FIRE:
            mParticleLayer->removeChild(mFire);
            break;
        case Mode::EXPLOSION:
            mParticleLayer->removeChild(mExplosion);
        break;
        default:
            break;
        }
//

        mState = State::READY;
    }
}
