#include "drawPolygon.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>

#include "utility.h"
#include "mainwindow.h"

int DrawPolygon::mCount{0};
DrawPolygon::DrawPolygon(QObject *parent): DrawShape(parent)
{

}

bool DrawPolygon::setup()
{
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
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mPolygonLayer->getName()));
        if(!layer){
            mPolygonLayer->getGroup()->removeChildren(0, mPolygonLayer->getGroup()->getNumChildren());
        }
        if(mPolygonLayer->getGroup()->getNumChildren() <= 0){
            shapeLayer->addLayer(mPolygonLayer);
        }
        setState(State::READY);
        mPolygonProperties = new PolygonProperties();
        createProperty("Polygon", QVariant::fromValue<PolygonProperties*>(mPolygonProperties));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mPolygonLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mPolygonLayer);
        }
        setState(State::NONE);
        mPolygon = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawPolygon::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Polygon" + QString::number(mCount);
    mPolygon = new Polygon(mapItem());
    mPolygon->setName(name.toStdString());
    //    mPolygon->setRadius(mPolygonProperties->getRadius());
    mPolygon->setHeight(mPolygonProperties->getHeight());
    mPolygon->setLineColor(Utility::qColor2osgEarthColor(mPolygonProperties->getStrokeColor()));
    mPolygon->setLineWidth(mPolygonProperties->getStrokeWidth());
    osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(mPolygonProperties->getClamp());
    mPolygon->setClamp(clampEnum);

    mPolygonLayer->addChild(mPolygon);
    mPolygonProperties->setPolygon(mPolygon, mapItem()->getMapSRS());
    mPolygonLayer->setName(mPolygon->getName());
    setState(State::DRAWING);

    mCount++;


}

void DrawPolygon::cancelDraw()
{




    if(state() == State::DRAWING){
        mPolygonLayer->getGroup()->removeChild(mPolygon);
        mPolygon = nullptr;
        mPolygonProperties->setPolygon(mPolygon, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}




