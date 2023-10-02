#include "drawCircle.h"
#include "utility.h"
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>
#include <QQmlContext>
#include "plugininterface.h"
#include "mapItem.h"
#include <osgEarth/GLUtils>
#include "circle.h"
#include "circleproperty.h"

int DrawCircle::mCount{0};
DrawCircle::DrawCircle(QObject *parent): DrawShape(parent)
{

}

bool DrawCircle::setup()
{
    connect(serviceManager(), &ServiceManager::circleDataReceived, this, &DrawCircle::addUpdateCircle);
    auto toolboxItem =  new ToolboxItem{CIRCLE, CATEGORY, "qrc:/resources/circle.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCircle::onCircleItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/circle.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());


    mCircleLayer =  new ParenticAnnotationLayer();
    mCircleLayer->setName(CIRCLE);


    return true;
}

void DrawCircle::onCircleItemCheck(bool check)
{

    if (check) {
        setState(State::READY);
        mCircleProperty = new CircleProperty();
        createProperty("Circle", QVariant::fromValue<CircleProperty*>(mCircleProperty));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        setState(State::NONE);
        mCircle = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawCircle::addUpdateCircle(CircleData *circleData)
{
    osgEarth::GeoPoint geoPoint(mapItem()->getMapObject()->getSRS(), circleData->longitude, circleData->latitude, circleData->altitude);
    Circle *circle;
    if(mCircleMap.contains(circleData->id)){
        circle = mCircleMap[circleData->id];
        circleData->layer->addChild(circle);
    }
    else {
        circle = new Circle;
        mCircleMap[circleData->id] = circle;
    }
    circle->setPosition(geoPoint);
    circle->setRadius(circleData->radius);
    circle->setFillColor(osgEarth::Color(circleData->color));
}

void DrawCircle::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Circle" + QString::number(mCount);
    mCircle = new Circle();
    mCircle->setName(name.toStdString());
    mCircle->setRadius(mCircleProperty->getRadius());
    mCircle->setHeight(mCircleProperty->getHeight());
    mCircle->setArcEnd(360);
    mCircle->setArcStart(0);
    mCircle->setStrokeColor(Utility::qColor2osgEarthColor(mCircleProperty->getStrokeColor()));
    mCircle->setStrokeWidth(mCircleProperty->getStrokeWidth());
    osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(mCircleProperty->getClamp());
    mCircle->setClamp(clampEnum);

    mCircle->setPosition(geoPos);

    if(!shapeLayer()->containsLayer(mCircleLayer)){
        mCircleLayer->clear();
        shapeLayer()->addLayer(mCircleLayer);
    }

    mCircleLayer->addChild(mCircle);
    mCircleProperty->setCircle(mCircle, mapItem()->getMapSRS());
    setState(State::DRAWING);
    mCount++;


}

void DrawCircle::cancelDraw()
{

    if(state() == State::DRAWING){
        mCircleLayer->getGroup()->removeChild(mCircle);
        mCircle = nullptr;
        mCircleProperty->setCircle(mCircle, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
        if(!mCircleLayer->hasNode())
            DrawShape::shapeLayer()->removeLayer(mCircleLayer);
    }
}

void DrawCircle::drawing(const osgEarth::GeoPoint &geoPos)
{
    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mCircleLayer)){
        initDraw(geoPos);
    }
    mCircle->setPosition(geoPos);
    mCircleProperty->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}


