#include "drawCylinder.h"
#include "mainwindow.h"
#include "utility.h"
#include "compositeAnnotationLayer.h"

#include "plugininterface.h"
#include "mapItem.h"
#include <QQmlContext>

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
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCylinderLayer->getName()));
        if(!layer){
            mCylinderLayer->getGroup()->removeChildren(0, mCylinderLayer->getGroup()->getNumChildren());
        }
        if(mCylinderLayer->getGroup()->getNumChildren() <= 0){
            shapeLayer->addLayer(mCylinderLayer);
        }
        setState(State::READY);
        mCylinderProperties = new CylinderProperties();
        createProperty("Cylinder", QVariant::fromValue<CylinderProperties*>(mCylinderProperties));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mCylinderLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mCylinderLayer);
        }
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
    mCylinder->setRadius(mCylinderProperties->getRadius());
    mCylinder->setHeight(mCylinderProperties->getHeight());
    mCylinder->setPosition(geoPos);
    mCylinderLayer->addChild(mCylinder);

    mCylinderLayer->setName(mCylinder->getName());
    mCylinderProperties->setCylinder(mCylinder, mapItem()->getMapSRS());
    setState(State::DRAWING);
    mCount++;



}

void DrawCylinder::cancelDraw()
{
    if(state() == State::DRAWING){
        mCylinderLayer->getGroup()->removeChild(mCylinder);
        mCylinder = nullptr;
        mCylinderProperties->setCylinder(mCylinder, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}

void DrawCylinder::drawing(const osgEarth::GeoPoint &geoPos)
{
    mCylinder->setPosition(geoPos);
    mCylinderProperties->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}

