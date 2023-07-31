#include "drawRect.h"
#include "mainwindow.h"
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>
#include "utility.h"
#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"
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

    mCompositeRectLayer = new CompositeAnnotationLayer();
    mCompositeRectLayer->setName(RECT);

    return true;
}

void DrawRect::onRectItemCheck(bool check)
{
    qmlRegisterType<RectProperties>("Crystal", 1, 0, "CProperty");

    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCompositeRectLayer->getName()));
        if(!layer){
            mCompositeRectLayer->clearLayers();
        }
        if(mCompositeRectLayer->getNumLayers() <= 0){

            //            mapItem()->getMapObject()->addLayer(mBoxLayer, shapeLayer);
            shapeLayer->addLayer(mCompositeRectLayer);
        }
        setState(State::READY);

        //mRectProperties = new RectProperties(mRect, qmlEngine(), uiHandle(), mapItem());
        //mRectProperties->show();

        createProperty();




        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();
        if(mCompositeRectLayer->getNumLayers() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mCompositeRectLayer);
        }
        setState(State::NONE);
        mRect = nullptr;
        mRectProperties->setProperty("visible", false);
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
    //    mapItem()->getMapObject()->addNodeToLayer(mRect, mRectLayer);
    //    mRectProperties->setRect(mRect, );

    mRectLayer = new ParenticAnnotationLayer();
    mRectLayer->addChild(mRect);
    mRectLayer->setName(mRect->getName());
    mCompositeRectLayer->addLayer(mRectLayer);
    mRectProperties->setRect(mRect, mapItem()->getMapSRS());

    setState(State::DRAWING);
    mCount++;
}

void DrawRect::cancelDraw()
{
    if(state() == State::DRAWING){
        //        mapItem()->getMapObject()->removeNodeFromLayer(mRect, mRectLayer);
        mCompositeRectLayer->removeLayer(mRectLayer);
        mRect = nullptr;
        mRectLayer = nullptr;
        //mRectProperties->setRect(mRect);
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


void DrawRect::createProperty()
{
    QQmlComponent* comp = new QQmlComponent(qmlEngine());
    connect(comp, &QQmlComponent::statusChanged, [comp, this](){
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }
        QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
        mRectProperties = static_cast<RectProperties*>(item);

        mainWindow()->addToRightContainer(mRectProperties, "Rect");
    });


    comp->loadUrl(QUrl("qrc:/Properties.qml"));
}
