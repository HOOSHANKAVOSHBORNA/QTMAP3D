#include "drawCone.h"
#include "utility.h"
#include "compositeAnnotationLayer.h"

#include "cone.h"
#include "coneProperty.h"

int DrawCone::mCount{0};
DrawCone::DrawCone(QObject *parent): DrawShape(parent)
{

}

bool DrawCone::setup()
{
    auto toolboxItem =  new ToolboxItem{CONE, CATEGORY, "qrc:/resources/cone.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCone::onConeItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/cone.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mConeLayer = new ParenticAnnotationLayer();
    mConeLayer->setName(CONE);
    return true;    
}

void DrawCone::onConeItemCheck(bool check)
{
    if (check) {
        setState(State::READY);
        mConeProperty = new ConeProperty();
        createProperty("Cone", QVariant::fromValue<ConeProperty*>(mConeProperty));
        mapItem()->addNode(iconNode());
    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        setState(State::NONE);
        mCone = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawCone::initDraw(const osgEarth::GeoPoint &geoPos)
{

    QString name = "Cone" + QString::number(mCount);
    mCone = new Cone();
    mCone->setName(name.toStdString());
    mCone->setPosition(geoPos);

    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mConeLayer)){
        mConeLayer->clear();
        shapeLayer->addLayer(mConeLayer);
    }
    mConeLayer->addChild(mCone);
    mConeProperty->setCone(mCone, mapItem()->getMapSRS());
    setState(State::DRAWING);
    mCount++;
}

void DrawCone::cancelDraw()
{
    if(state() == State::DRAWING){
        mConeLayer->getGroup()->removeChild(mCone);
        mCone = nullptr;
        mConeProperty->setCone(mCone, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
        if(!mConeLayer->hasNode())
            DrawShape::shapeLayer()->removeLayer(mConeLayer);

    }
}

void DrawCone::drawing(const osgEarth::GeoPoint &geoPos)
{
    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mConeLayer)){
        initDraw(geoPos);
    }
    mCone->setPosition(geoPos);
    mConeProperty->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}

