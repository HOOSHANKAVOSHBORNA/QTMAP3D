#include "drawCircle.h"
#include "utility.h"
#include "mainwindow.h"
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>
#include <QQmlContext>
#include "plugininterface.h"
#include "mapItem.h"
#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

int DrawCircle::mCount{0};
DrawCircle::DrawCircle(QObject *parent): DrawShape(parent)
{

}

bool DrawCircle::setup()
{
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
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCircleLayer->getName()));
        if(!layer){
            mCircleLayer->getGroup()->removeChildren(0, mCircleLayer->getGroup()->getNumChildren());
        }
        if(mCircleLayer->getGroup()->getNumChildren() <= 0){
            shapeLayer->addLayer(mCircleLayer);
        }
        setState(State::READY);
        mCircleProperties = new CircleProperties();
        createProperty("Circle", QVariant::fromValue<CircleProperties*>(mCircleProperties));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mCircleLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mCircleLayer);
        }
        setState(State::NONE);
        mCircle = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}






void DrawCircle::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Circle" + QString::number(mCount);
    mCircle = new Circle();
    mCircle->setName(name.toStdString());
    mCircle->setRadius(mCircleProperties->getRadius());
    mCircle->setCircleHeight(mCircleProperties->getHeight());
    mCircle->setArcEnd(360);
    mCircle->setArcStart(0);
    mCircle->setLineColor(Utility::qColor2osgEarthColor(mCircleProperties->getStrokeColor()));
    mCircle->setLineWidth(mCircleProperties->getStrokeWidth());
    osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(mCircleProperties->getClamp());
    mCircle->setClamp(clampEnum);

    mCircle->setPosition(geoPos);

    //    mapItem()->getMapObject()->addNodeToLayer(mCircle, mCircleLayer);
    //    mCircleProperties->setCircle(mCircle);


    //    mCompositeCircleLayer->addLayer(mCircleLayer);
    mCircleLayer->addChild(mCircle);
    mCircleLayer->setName(mCircle->getName());



    mCircleProperties->setCircle(mCircle, mapItem()->getMapSRS());

    setState(State::DRAWING);
    mCount++;


}

void DrawCircle::cancelDraw()
{

    if(state() == State::DRAWING){
        mCircleLayer->getGroup()->removeChild(mCircle);
        mCircle = nullptr;
        mCircleProperties->setCircle(mCircle, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}

void DrawCircle::drawing(const osgEarth::GeoPoint &geoPos)
{
    mCircle->setPosition(geoPos);
    mCircleProperties->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}


