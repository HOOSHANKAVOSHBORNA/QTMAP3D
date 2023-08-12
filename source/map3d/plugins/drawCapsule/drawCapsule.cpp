#include "drawCapsule.h"
#include "utility.h"
#include "plugininterface.h"
#include "compositeAnnotationLayer.h"
#include "utility.h"
#include <QQmlContext>
#include "capsuleProperty.h"

int DrawCapsule::mCount{0};

DrawCapsule::DrawCapsule(QObject *parent): DrawShape(parent)
{
}

bool DrawCapsule::setup()
{
    auto toolboxItem =  new ToolboxItem{CAPSULE, CATEGORY, "qrc:/resources/capsule.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCapsule::onCapsuleItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/capsule.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mCapsuleLayer  = new ParenticAnnotationLayer();
    mCapsuleLayer->setName(CAPSULE);
    return true;
}

void DrawCapsule::onCapsuleItemCheck(bool check)
{
    if (check) {
        setState(State::READY);
        mCapsuleProperty = new CapsuleProperty();
        createProperty("Capsule", QVariant::fromValue<CapsuleProperty*>(mCapsuleProperty));
        mapItem()->addNode(iconNode());


    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mCapsuleLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mCapsuleLayer);
        }
        setState(State::NONE);
        mCapsule = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawCapsule::initDraw(const osgEarth::GeoPoint &geoPos)
{

    QString name = "Capsule" + QString::number(mCount);
    mCapsule = new Capsule();
    mCapsule->setName(name.toStdString());
    mCapsule->setRadius(mCapsuleProperty->getRadius());
    mCapsule->setHeight(mCapsuleProperty->getHeight());
    mCapsule->setPosition(geoPos);

    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mCapsuleLayer)){
        mCapsuleLayer->clear();
        shapeLayer->addLayer(mCapsuleLayer);
    }

    mCapsuleLayer->addChild(mCapsule);
    mCapsuleProperty->setCapsule(mCapsule, mapItem()->getMapSRS());

    setState(State::DRAWING);
    mCount++;
}

void DrawCapsule::cancelDraw()
{
    if(state() == State::DRAWING){
        mCapsuleLayer->getGroup()->removeChild(mCapsule);
        mCapsule = nullptr;
        mCapsuleProperty->setCapsule(mCapsule, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;

        if(!mCapsuleLayer->hasNode())
            DrawShape::shapeLayer()->removeLayer(mCapsuleLayer);
    }
}

void DrawCapsule::drawing(const osgEarth::GeoPoint &geoPos)
{
    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mCapsuleLayer)){
        initDraw(geoPos);
    }

    mCapsule->setPosition(geoPos);
    mCapsuleProperty->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}
