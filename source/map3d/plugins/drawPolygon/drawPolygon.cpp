#include "drawPolygon.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>


int DrawPolygon::mCount{0};
DrawPolygon::DrawPolygon(QObject *parent)
    : DrawShape(parent)
{
    qmlRegisterType<PolygonPropertiesModel>("Crystal", 1, 0, "PolygonProperties");
}

bool DrawPolygon::setup()
{
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    auto toolboxItem =  new ToolboxItem{POLYGON, CATEGORY, "qrc:/resources/polygon.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawPolygon::onPolygonItemCheck);
    toolbox()->addItem(toolboxItem);

    auto toolboxItemArea =  new ToolboxItem{MEASUREAREA, M_CATEGORY, "qrc:/resources/measure_area.png", true};
    QObject::connect(toolboxItemArea, &ToolboxItem::itemChecked, this, &DrawPolygon::onMeasureAreaItemCheck);
    toolbox()->addItem(toolboxItemArea);

    mPolygonLayer = new osgEarth::Annotation::AnnotationLayer();
    mPolygonLayer->setName(POLYGON);

    mAreaLayer = new osgEarth::Annotation::AnnotationLayer();
    mAreaLayer->setName(MEASUREAREA);
    return true;
}

void DrawPolygon::onPolygonItemCheck(bool check)
{
    auto shapeLayer = DrawShape::shapeLayer();
    if (check) {
        if(mPolygonLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mPolygonLayer, shapeLayer);
        }
        makeIconNode("../data/images/draw/polygon.png");
        mType = Type::POLYGONN;
        setState(State::READY);

//        mPolygonProperties = new PolygonProperties(qmlEngine(), uiHandle(), mapItem());
//        mPolygonProperties->show();
        mapItem()->addNode(iconNode());
    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mPolygonLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->removeLayer(mPolygonLayer, shapeLayer);
        }

        setState(State::NONE);
//        mPolygonProperties->deleteLater();
        mType = Type::NONE;
        mPolygon = nullptr;
//        mPolygonProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawPolygon::onMeasureAreaItemCheck(bool check)
{
    auto measureLayer = DrawShape::measureLayer();
    if(check)
    {
        mType = Type::AREA;
        if(mAreaLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mAreaLayer, measureLayer);
        }
        makeIconNode("../data/images/draw/polygon.png");

        setState(State::READY);

//        mPolygonProperties = new PolygonProperties(qmlEngine(), uiHandle(), mapItem());
//        mPolygonProperties->show();
        mapItem()->addNode(iconNode());
    }
    else
    {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mAreaLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->removeLayer(mAreaLayer, measureLayer);
        }
        mType = Type::NONE;
//        mPolygonProperties->deleteLater();
        mPolygon = nullptr;
//        mPolygonProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawPolygon::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name;
    mPolygon = new Polygon(mapItem());
    if (mType == Type::AREA){
        name = MEASUREAREA + QString::number(mCount);
        mPolygon->setClamp(osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN);
        mPolygon->setName(name.toStdString());
        mPolygon->setShowArea(true);
//        mPolygon->clearPoints();
        mapItem()->getMapObject()->addNodeToLayer(mPolygon, mAreaLayer);
    }
    else{
        name = POLYGON + QString::number(mCount);
        mPolygon->setName(name.toStdString());
        mapItem()->getMapObject()->addNodeToLayer(mPolygon, mPolygonLayer);
    }
//    mPolygonProperties->setPolygon(mPolygon);
    setState(State::DRAWING);
    mCount++;
}

void DrawPolygon::tempDrawing(const osgEarth::GeoPoint &geoPos)
{
    if (mPolygon->getSize() >1)
    {
        mPolygon->removePoint();

    }
    mPolygon->addPoints(geoPos);

}

void DrawPolygon::drawing(const osgEarth::GeoPoint &geoPos)
{
    mPolygon->addPoints(geoPos);
}


void DrawPolygon::cancelDraw()
{
    if(state() == State::DRAWING){
        mapItem()->getMapObject()->removeNodeFromLayer(mPolygon, mPolygonLayer);
        mapItem()->getMapObject()->removeNodeFromLayer(mPolygon, mAreaLayer);
        mPolygon = nullptr;
//        mPolygonProperties->setPolygon(mPolygon);
        setState(State::READY);
        mCount--;
    }

}


