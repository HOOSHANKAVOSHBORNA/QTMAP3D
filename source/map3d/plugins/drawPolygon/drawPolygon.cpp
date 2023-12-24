#include "drawPolygon.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>

#include <utility.h>

int DrawPolygon::mCount{0};
DrawPolygon::DrawPolygon(QObject *parent): DrawShape(parent)
{

}

bool DrawPolygon::setup()
{
    connect(serviceManager(), &ServiceManager::polygonDataReceived, this, &DrawPolygon::polygonDataReceived);

    auto toolboxItem =  new ToolboxItem{POLYGON, CATEGORY, "qrc:/resources/polygon.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawPolygon::onPolygonItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/polygon.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mPolygonLayer = new ParenticAnnotationLayer();
    mPolygonLayer->setName(POLYGON);

    return true;


}

void DrawPolygon::onPolygonItemCheck(bool check)
{
    if (check) {
        setState(State::READY);
        mPolygonProperty = new PolygonProperty();
        createProperty("Polygon", QVariant::fromValue<PolygonProperty*>(mPolygonProperty));
        mapItem()->addNode(iconNode());
    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        setState(State::NONE);
        mPolygon = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawPolygon::polygonDataReceived(PolygonData *polygonData)
{
    if (polygonData->command == "REMOVE"){
        if (mPolygonMap.contains(polygonData->id)){
            mPolygonMap[polygonData->id]->polygonData()->layer->removeChild(mPolygonMap[polygonData->id]);
            mPolygonMap[polygonData->id].release();
            mPolygonMap.remove(polygonData->id);
        }
    } else if (polygonData->command == "UPDATE") {
        addUpdatePolygon(polygonData);
    }
    else {
        addUpdatePolygon(polygonData);
    }
}

void DrawPolygon::addUpdatePolygon(PolygonData *polygonData)
{
    Polygon *polygon;
    if (!mPolygonMap.contains(polygonData->id)) {
        polygon = new Polygon(mapItem());
        polygonData->layer->addChild(polygon);
        mPolygonMap[polygonData->id] = polygon;
    }
    else {
        polygon = mPolygonMap[polygonData->id];
        polygon->clearPoints();
    }
    polygon->setName(polygonData->name);
    polygon->create(&polygonData->points);
    polygon->setPolygonData(polygonData);
//    for (auto point: polygonData->points){
//        osgEarth::GeoPoint geopos(mapItem()->getMapSRS(), point.x(), point.y(), point.z());
//        polygon->addPoint(geopos);
//    }
//    polygon->setHeight(0);
    polygon->setStrokeWidth(polygonData->width);
    QColor color(QString::fromStdString(polygonData->color));
    polygon->setStrokeColor(Utility::qColor2osgEarthColor(color));
    QColor fillColor(QString::fromStdString(polygonData->fillColor));
    polygon->setFillColor(Utility::qColor2osgEarthColor(fillColor));
//    polygon->setClamp(osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN);
}

void DrawPolygon::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Polygon" + QString::number(mCount);
    mPolygon = new Polygon(mapItem());
    mPolygon->setName(name.toStdString());
    mPolygonProperty->setPolygon(mPolygon, mapItem()->getMapSRS());

    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mPolygonLayer)){
        mPolygonLayer->clear();
        shapeLayer->addLayer(mPolygonLayer);
    }
    mPolygonLayer->addChild(mPolygon);

    setState(State::DRAWING);
    mCount++;
}

void DrawPolygon::tempDrawing(const osgEarth::GeoPoint &geoPos)
{
    if (mPolygon->getSize() > 1)
    {
        mPolygon->removePoint();
    }
    mPolygon->addPoint(geoPos);
}

void DrawPolygon::drawing(const osgEarth::GeoPoint &geoPos)
{
    mPolygon->addPoint(geoPos);
}

void DrawPolygon::cancelDraw()
{
    if(state() == State::DRAWING){
        mPolygonLayer->removeChild(mPolygon);
        mPolygon = nullptr;
        mPolygonProperty->setPolygon(mPolygon, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;

        if(!mPolygonLayer->hasNode())
            DrawShape::shapeLayer()->removeLayer(mPolygonLayer);
    }
}




