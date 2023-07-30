#include "drawCapsule.h"
#include "utility.h"
#include "compositeAnnotationLayer.h"
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

    mCompositeCapsuleLayer = new CompositeAnnotationLayer();
    mCompositeCapsuleLayer->setName(CAPSULE);

    return true;
}

void DrawCapsule::onCapsuleItemCheck(bool check)
{
    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCompositeCapsuleLayer->getName()));
        if(!layer){
            mCompositeCapsuleLayer->clearLayers();
        }
        if(mCompositeCapsuleLayer->getNumLayers() <= 0){
            shapeLayer->addLayer(mCompositeCapsuleLayer);
        }
        setState(State::READY);
        mapItem()->addNode(iconNode());
        mCapsuleProperties = new CapsuleProperties();
        createProperty("Capsule", QVariant::fromValue<CapsuleProperties*>(mCapsuleProperties));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();
        if(mCompositeCapsuleLayer->getNumLayers() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mCompositeCapsuleLayer);
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
    mCapsule->setRadius(mCapsuleProperties->getRadius());
    mCapsule->setHeight(mCapsuleProperties->getHeight());
    mCapsule->setPosition(geoPos);

    mCapsuleLayer = new ParenticAnnotationLayer();
    mCapsuleLayer->addChild(mCapsule);
    mCapsuleLayer->setName(mCapsule->getName());
    mCompositeCapsuleLayer->addLayer(mCapsuleLayer);
    mCapsuleProperties->setCapsule(mCapsule, mapItem()->getMapSRS());

    setState(State::DRAWING);
    mCount++;
}

void DrawCapsule::cancelDraw()
{
    if(state() == State::DRAWING){
        mCompositeCapsuleLayer->removeLayer(mCapsuleLayer);
        mCapsule = nullptr;
        mCapsuleLayer = nullptr;
        mCapsuleProperties->setCapsule(mCapsule, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}

void DrawCapsule::drawing(const osgEarth::GeoPoint &geoPos)
{
    mCapsule->setPosition(geoPos);
    mCapsuleProperties->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}
