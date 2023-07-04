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
    auto toolboxItem =  new ToolboxItem{POLYGON, CATEGORY, "qrc:/resources/polygon.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawPolygon::onPolygonItemCheck);
    toolbox()->addItem(toolboxItem);

    auto toolboxItemArea =  new ToolboxItem{MEASUREAREA, M_CATEGORY, "qrc:/resources/polygon.png", true};
    QObject::connect(toolboxItemArea, &ToolboxItem::itemChecked, this, &DrawPolygon::onMeasureAreaItemCheck);
    toolbox()->addItem(toolboxItemArea);

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    //    addLayer();
    mPolygonLayer = new osgEarth::Annotation::AnnotationLayer();
    mPolygonLayer->setName(POLYGON);

    mAreaLayer = new osgEarth::Annotation::AnnotationLayer();
    mAreaLayer->setName(MEASUREAREA);
    return true;
}


void DrawPolygon::onPolygonItemCheck(bool check)
{
    if (check) {
        if(mPolygonLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mPolygonLayer);
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->setParentLayer(mPolygonLayer, shapeLayer);
        }
        mType = Type::POLYGONN;
        setState(State::READY);


        mPolygonProperties = new PolygonProperties(qmlEngine(), uiHandle(), mapItem());
        mPolygonProperties->show();
        mapItem()->addNode(iconNode());
    }
    else {
        if(mPolygonLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->setParentLayer(mPolygonLayer, nullptr);
            mapItem()->getMapObject()->removeLayer(mPolygonLayer);
        }
        if(state() == State::DRAWING)
            cancelDraw();
        setState(State::NONE);
        mPolygonProperties->deleteLater();
        mType = Type::NONE;
        mPolygon = nullptr;
        mPolygonProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawPolygon::onMeasureAreaItemCheck(bool check)
{
    if(check)
    {
        onPolygonItemCheck(true);
        mType = Type::AREA;
        if(mAreaLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mAreaLayer);
            auto measureLayer = DrawShape::measureLayer();
            mapItem()->getMapObject()->setParentLayer(mAreaLayer, measureLayer);
        }
        makeIconNode("../data/images/draw/polygon.png");

    }
    else
    {
        if(mAreaLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->setParentLayer(mAreaLayer, nullptr);
            mapItem()->getMapObject()->removeLayer(mAreaLayer);
        }
        mType = Type::NONE;
        mPolygonProperties->deleteLater();
        mPolygon = nullptr;
        mPolygonProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawPolygon::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name;
    mPolygon = new Polygon(mapItem());
    if (mType == Type::AREA){
        name = MEASUREAREA + QString::number(mCount);
        mapItem()->getMapObject()->addNodeToLayer(mPolygon, mAreaLayer);
        mPolygonProperties->setPolygon(mPolygon);
        mPolygon->setName(name.toStdString());
        mPolygon->setShowArea(true);
        mPolygon->clearPoints();
    }
    else{
        name = POLYGON + QString::number(mCount);
        mapItem()->getMapObject()->addNodeToLayer(mPolygon, mPolygonLayer);
        mPolygonProperties->setPolygon(mPolygon);
        mPolygon->setName(name.toStdString());
    }
    setState(State::DRAWING);
    mCount++;
}

void DrawPolygon::tempDrawing(const osgEarth::GeoPoint &geoPos)
{
    if (mPolygon->getSize() >= 2)
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
        mPolygonProperties->setPolygon(mPolygon);
        setState(State::READY);
        mCount--;
    }

}


