#include "drawRect.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

int DrawRect::mCount{0};
DrawRect::DrawRect(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<RectPropertiesModel>("Crystal", 1, 0, "RectProperties");
}

bool DrawRect::setup()
{
    auto toolboxItem =  new ToolboxItem{RECT, CATEGORY, "qrc:/resources/rectangle.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawRect::onRectItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/rectangle.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mRectLayer = new osgEarth::Annotation::AnnotationLayer();
    mRectLayer->setName(RECT);

    return true;
}

void DrawRect::onRectItemCheck(bool check)
{
    if (check) {
        if(mRectLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->addLayer(mRectLayer, shapeLayer);
        }
        setState(State::READY);
//        /*mRectProperties*/ = new RectProperties(mRect, qmlEngine(), uiHandle(), mapItem());
//        mRectProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mRectLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->removeLayer(mRectLayer, shapeLayer);
        }
        setState(State::NONE);
        mRect = nullptr;
//        mRectProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawRect::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Rect" + QString::number(mCount);
    mRect = new Rect();
    mRect->setName(name.toStdString());

    mRect->setPosition(geoPos);

    mapItem()->getMapObject()->addNodeToLayer(mRect, mRectLayer);
    mRectProperties->setRect(mRect);

    setState(State::DRAWING);
    mCount++;
}

void DrawRect::cancelDraw()
{
    if(state() == State::DRAWING){
        mapItem()->getMapObject()->removeNodeFromLayer(mRect, mRectLayer);
        mRect = nullptr;
        mRectProperties->setRect(mRect);
        setState(State::READY);
        mCount--;
    }
}
