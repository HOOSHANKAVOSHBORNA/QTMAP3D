#include "drawRect.h"
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>
#include "utility.h"
#include <osgEarth/GLUtils>
#include "compositeAnnotationLayer.h"



int DrawRect::mCount{0};
DrawRect::DrawRect(QObject *parent): DrawShape(parent)
{
}

bool DrawRect::setup()
{
    auto toolboxItem =  new ToolboxItem{RECT, CATEGORY, "qrc:/resources/rectangle.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawRect::onRectItemCheck);
    toolbox()->addItem(toolboxItem);
    makeIconNode("../data/images/draw/rectangle.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    mRectLayer = new ParenticAnnotationLayer();
    mRectLayer->setName(RECT);
    return true;
}

void DrawRect::onRectItemCheck(bool check)
{
    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mRectLayer->getName()));
        if(!layer){
            mRectLayer->getGroup()->removeChildren(0, mRectLayer->getGroup()->getNumChildren());
        }
        if(mRectLayer->getGroup()->getNumChildren() <= 0){
            shapeLayer->addLayer(mRectLayer);
        }
        setState(State::READY);
        mRectProperties = new RectProperties();
        createProperty("Rect", QVariant::fromValue<RectProperties*>(mRectProperties));
        mapItem()->addNode(iconNode());


    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();
        if(mRectLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mRectLayer);
        }
        setState(State::NONE);
        mRect = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawRect::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Rect" + QString::number(mCount);
    mRect = new Rect();
    mRect->setName(name.toStdString());
    mRect->setHeight(mRectProperties->getHeight());
    mRect->setWidth(mRectProperties->getWidth());
    mRect->setPosition(geoPos);

    mRectLayer->addChild(mRect);
    mRectLayer->setName(mRect->getName());
    mRectProperties->setRect(mRect, mapItem()->getMapSRS());

    setState(State::DRAWING);
    mCount++;
}



void DrawRect::cancelDraw()
{
    if(state() == State::DRAWING){
        mRectLayer->getGroup()->removeChild(mRect);
        mRect = nullptr;
        mRectProperties->setRect(mRect, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}
void DrawRect::drawing(const osgEarth::GeoPoint &geoPos)
{
    mRect->setPosition(geoPos);
    mRectProperties->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}


