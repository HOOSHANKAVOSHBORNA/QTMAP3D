#include "drawCylinder.h"

#include "mainwindow.h"
#include "utility.h"
#include "compositeAnnotationLayer.h"
int DrawCylinder::mCount{0};
DrawCylinder::DrawCylinder(QObject *parent): DrawShape(parent)
{
}

bool DrawCylinder::setup()
{
    auto toolboxItem =  new ToolboxItem{CYLINDER, CATEGORY, "qrc:/resources/cylinder.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCylinder::onCylinderItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/cylinder.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mCompositeCylinderLayer = new CompositeAnnotationLayer();
    mCompositeCylinderLayer->setName(CYLINDER);

    return true;
}

void DrawCylinder::onCylinderItemCheck(bool check)
{
    qmlRegisterType<CylinderProperties>("Crystal", 1, 0, "CProperty");

    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCompositeCylinderLayer->getName()));
        if(!layer){
            mCompositeCylinderLayer->clearLayers();
        }
        if(mCompositeCylinderLayer->getNumLayers() <= 0){

            //            mapItem()->getMapObject()->addLayer(mBoxLayer, shapeLayer);
            shapeLayer->addLayer(mCompositeCylinderLayer);
        }
        setState(State::READY);
        mapItem()->addNode(iconNode());
        //mCylinderProperties = new CylinderProperties(mCylinder, qmlEngine(), uiHandle(), mapItem());
        //mCylinderProperties->show();

        createProperty();




        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();
        if(mCompositeCylinderLayer->getNumLayers() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mCompositeCylinderLayer);
        }
        setState(State::NONE);
        mCylinder = nullptr;
        mCylinderProperties->setProperty("visible", false);
        mapItem()->removeNode(iconNode());
    }
}

void DrawCylinder::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Cylinder" + QString::number(mCount);
    mCylinder = new Cylinder();
    mCylinder->setName(name.toStdString());
    mCylinder->setRadius(mCylinderProperties->getRadius());
    mCylinder->setHeight(mCylinderProperties->getHeight());
    mCylinder->setPosition(geoPos);
    //    mapItem()->getMapObject()->addNodeToLayer(mCylinder, mCylinderLayer);
    //    mCylinderProperties->setCylinder(mCylinder, );

    mCylinderLayer = new ParenticAnnotationLayer();
    mCylinderLayer->addChild(mCylinder);
    mCylinderLayer->setName(mCylinder->getName());
    mCompositeCylinderLayer->addLayer(mCylinderLayer);
    mCylinderProperties->setCylinder(mCylinder, mapItem()->getMapSRS());

    setState(State::DRAWING);
    mCount++;
}

void DrawCylinder::cancelDraw()
{
    if(state() == State::DRAWING){
        //        mapItem()->getMapObject()->removeNodeFromLayer(mCylinder, mCylinderLayer);
        mCompositeCylinderLayer->removeLayer(mCylinderLayer);
        mCylinder = nullptr;
        mCylinderLayer = nullptr;
        //mCylinderProperties->setCylinder(mCylinder);
        mCylinderProperties->setCylinder(mCylinder, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}

void DrawCylinder::drawing(const osgEarth::GeoPoint &geoPos)
{
    mCylinder->setPosition(geoPos);
    mCylinderProperties->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}

void DrawCylinder::createProperty()
{
    QQmlComponent* comp = new QQmlComponent(qmlEngine());
    connect(comp, &QQmlComponent::statusChanged, [comp, this](){
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }
        //            QQmlContext *context = new QQmlContext(qmlEngine(), this);
        QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
        mCylinderProperties = static_cast<CylinderProperties*>(item);
        //            mBoxProperties->setFillColorStatus(false);
        //            mBoxProperties->setFillColor(QColor());
        //            mBoxProperty->setStatuses();

        //        mBoxProperties = new BoxProperties();
        mainWindow()->addDockItem(mCylinderProperties, 0.3);
    });


    comp->loadUrl(QUrl("qrc:/Properties.qml"));
}
