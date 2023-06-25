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
        mapItem()->addLayerToLayer(mRectLayer, CATEGORY);
        setState(State::READY);
        mRectProperties = new RectProperties(mRect, qmlEngine(), uiHandle(), mapItem());
        mRectProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(mRectLayer->getGroup()->getNumChildren() <= 0)
            mapItem()->removeLayerFromLayer(mRectLayer, CATEGORY);
        if(state() == State::EDIT)
            cancelDraw();
        setState(State::NONE);
        mRect = nullptr;
        mRectProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawRect::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Rect" + QString::number(mCount);
    mRect = new Rect();
    mRect->setName(name.toStdString());

    mRect->setPosition(geoPos);

    mapItem()->addNodeToLayer(mRect, RECT);
    mRectProperties->setRect(mRect);

    setState(State::EDIT);
    mCount++;
}

void DrawRect::cancelDraw()
{
    if(state() == State::EDIT){
        mapItem()->removeNodeFromLayer(mRect, RECT);
        mRect = nullptr;
        mRectProperties->setRect(mRect);
        setState(State::READY);
        mCount--;
    }
}
