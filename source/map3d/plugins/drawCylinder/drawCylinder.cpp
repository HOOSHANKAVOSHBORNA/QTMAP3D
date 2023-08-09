#include "drawCylinder.h"
#include "utility.h"
#include "compositeAnnotationLayer.h"

#include "plugininterface.h"
#include "mapItem.h"
#include <QQmlContext>
#include "cylinder.h"
#include "cylinderProperty.h"

int DrawCylinder::mCount{0};
DrawCylinder::DrawCylinder(QObject *parent): DrawShape(parent)
{
}

bool DrawCylinder::setup()
{
    auto toolboxItem =  new ToolboxItem{CYLINDER, CATEGORY, "qrc:/resources/cylinder.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCylinder::onCylinderItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/cylinder.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mCylinderLayer = new ParenticAnnotationLayer();
    mCylinderLayer->setName(CYLINDER);

    return true;
}

void DrawCylinder::onCylinderItemCheck(bool check)
{

    if (check) {
        setState(State::READY);
        mCylinderProperty = new CylinderProperty();
        createProperty("Cylinder", QVariant::fromValue<CylinderProperty*>(mCylinderProperty));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        setState(State::NONE);
        mCylinder = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }

}

void DrawCylinder::initDraw(const osgEarth::GeoPoint &geoPos)
{

    QString name = "Cylinder" + QString::number(mCount);
    mCylinder = new Cylinder();
    mCylinder->setName(name.toStdString());
    mCylinder->setRadius(mCylinderProperty->getRadius());
    mCylinder->setHeight(mCylinderProperty->getHeight());
    mCylinder->setPosition(geoPos);

    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mCylinderLayer)){
        mCylinderLayer->clear();
        shapeLayer->addLayer(mCylinderLayer);
    }

    mCylinderLayer->addChild(mCylinder);
    mCylinderProperty->setCylinder(mCylinder, mapItem()->getMapSRS());
    setState(State::DRAWING);
    mCount++;



}

void DrawCylinder::cancelDraw()
{
    if(state() == State::DRAWING){
        mCylinderLayer->getGroup()->removeChild(mCylinder);
        mCylinder = nullptr;
        mCylinderProperty->setCylinder(mCylinder, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
        if(!mCylinderLayer->hasNode())
            DrawShape::shapeLayer()->removeLayer(mCylinderLayer);

    }
}

void DrawCylinder::drawing(const osgEarth::GeoPoint &geoPos)
{
    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mCylinderLayer)){
        initDraw(geoPos);
    }
    mCylinder->setPosition(geoPos);
    mCylinderProperty->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}

