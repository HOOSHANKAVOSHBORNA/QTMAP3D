#include "drawBox.h"
#include "mainwindow.h"
#include "plugininterface.h"
#include "mapItem.h"
#include "utility.h"
#include <QQmlContext>
int DrawBox::mCount{0};

DrawBox::DrawBox(QObject *parent): DrawShape(parent)
{
//    qmlRegisterType<BoxPropertiesModel>("Crystal", 1, 0, "BoxProperties");


}

bool DrawBox::setup()
{
    auto toolboxItem =  new ToolboxItem{BOX, CATEGORY, "qrc:/resources/box.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawBox::onBoxItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/box.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

//    addLayer();
    mCompositeBoxLayer = new CompositeAnnotationLayer();
    mCompositeBoxLayer->setName(BOX);
    return true;
}

void DrawBox::onBoxItemCheck(bool check)
{

    qmlRegisterType<BoxProperty>("Crystal", 1, 0, "CProperty");
    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCompositeBoxLayer->getName()));
        if(!layer){
            mCompositeBoxLayer->clearLayers();
        }
        if(mCompositeBoxLayer->getNumLayers() <= 0){

//            mapItem()->getMapObject()->addLayer(mBoxLayer, shapeLayer);
            shapeLayer->addLayer(mCompositeBoxLayer);
        }
        setState(State::READY);

       createProperty();
       mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mCompositeBoxLayer->getNumLayers() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
//            mapItem()->getMapObject()->removeLayer(mBoxLayer, shapeLayer);
            shapeLayer->removeLayer(mCompositeBoxLayer);
        }
        setState(State::NONE);
        mBox = nullptr;
        mBoxProperty->setProperty("visible", false);
        mapItem()->removeNode(iconNode());
    }
}

void DrawBox::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "box" + QString::number(mCount);
    mBox = new Box();
    mBox->setName(name.toStdString());

    mBox->setPosition(geoPos);
//    mBox->setCenter(geoPos);
    mBoxLayer = new ParenticAnnotationLayer();
    mBoxLayer->addChild(mBox);
    mBoxLayer->setName(mBox->getName());
//    mapItem()->getMapObject()->addNodeToLayer(mBox, mBoxLayer);
    mCompositeBoxLayer->addLayer(mBoxLayer);
    mBoxProperty->setBox(mBox, mapItem()->getMapSRS());




    setState(State::DRAWING);
    mCount++;
}

void DrawBox::drawing(const osgEarth::GeoPoint &geoPos)
{
    mBox->setPosition(geoPos);
//    mBox->setCenter(geoPos);
    mBoxProperty->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
//    mBoxProperty->setCenter(Utility::osgEarthGeoPointToQvector3D(geoPos));
}

void DrawBox::cancelDraw()
{
    if(state() == State::DRAWING){
//        mapItem()->getMapObject()->removeNodeFromLayer(mBox, mCompositeBoxLayer);
//        mapItem()->getMapObject()->removeLayer(mBoxLayer, mCompositeBoxLayer);
        mCompositeBoxLayer->removeLayer(mBoxLayer);
        mBox = nullptr;
        mBoxLayer = nullptr;
        mBoxProperty->setBox(mBox, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}

void DrawBox::createProperty()
{
    QQmlComponent* comp = new QQmlComponent(qmlEngine());
    connect(comp, &QQmlComponent::statusChanged, [comp, this](){
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }
        //            QQmlContext *context = new QQmlContext(qmlEngine(), this);
        QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
        mBoxProperty = static_cast<BoxProperty*>(item);

        //        mBoxProperties = new BoxProperties();
        mainWindow()->addToRightContainer(mBoxProperty, "Box");

    });


    comp->loadUrl(QUrl("qrc:/Properties.qml"));
}

