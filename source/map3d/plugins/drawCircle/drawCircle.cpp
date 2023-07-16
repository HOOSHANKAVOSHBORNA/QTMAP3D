#include "drawCircle.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

int DrawCircle::mCount{0};
DrawCircle::DrawCircle(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<CirclePropertiesModel>("Crystal", 1, 0, "CircleProperties");
}

bool DrawCircle::setup()
{
    auto toolboxItem =  new ToolboxItem{CIRCLE, CATEGORY, "qrc:/resources/circle.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCircle::onCircleItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/circle.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mCircleLayer = new osgEarth::Annotation::AnnotationLayer();
    mCircleLayer->setName(CIRCLE);

    return true;
}

void DrawCircle::onCircleItemCheck(bool check)
{
    if (check) {
        if(mCircleLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->addLayer(mCircleLayer, shapeLayer);
        }
        setState(State::READY);
//        mCircleProperties = new CircleProperties(mCircle, qmlEngine(), uiHandle(), mapItem());
//        mCircleProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mCircleLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->removeLayer(mCircleLayer, shapeLayer);
        }
        setState(State::NONE);
        mCircle = nullptr;
        //mCircleProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawCircle::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Circle" + QString::number(mCount);
    mCircle = new Circle();
    mCircle->setName(name.toStdString());
    mCircle->setRadius(200000);

    mCircle->setPosition(geoPos);

    mapItem()->getMapObject()->addNodeToLayer(mCircle, mCircleLayer);
//    mCircleProperties->setCircle(mCircle);

    setState(State::DRAWING);
    mCount++;
}

void DrawCircle::cancelDraw()
{
    if(state() == State::DRAWING){
        mapItem()->getMapObject()->removeNodeFromLayer(mCircle, mCircleLayer);
        mCircle = nullptr;
        //mCircleProperties->setCircle(mCircle);
        setState(State::READY);
        mCount--;
    }
}
