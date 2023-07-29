#include "drawSphere.h"


#include "mainwindow.h"
#include "utility.h"
#include "compositeAnnotationLayer.h"
int DrawSphere::mCount{0};
DrawSphere::DrawSphere(QObject *parent): DrawShape(parent)
{
}

bool DrawSphere::setup()
{
    auto toolboxItem =  new ToolboxItem{SPHERE, CATEGORY, "qrc:/resources/sphere.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawSphere::onSphereItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/sphere.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mCompositeSphereLayer = new CompositeAnnotationLayer();
    mCompositeSphereLayer->setName(SPHERE);

    return true;
}

void DrawSphere::onSphereItemCheck(bool check)
{
    qmlRegisterType<SphereProperties>("Crystal", 1, 0, "CProperty");

    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCompositeSphereLayer->getName()));
        if(!layer){
            mCompositeSphereLayer->clearLayers();
        }
        if(mCompositeSphereLayer->getNumLayers() <= 0){

            //            mapItem()->getMapObject()->addLayer(mBoxLayer, shapeLayer);
            shapeLayer->addLayer(mCompositeSphereLayer);
        }
        setState(State::READY);
        mapItem()->addNode(iconNode());
        //mSphereProperties = new SphereProperties(mSphere, qmlEngine(), uiHandle(), mapItem());
        //mSphereProperties->show();

        createProperty();




        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();
        if(mCompositeSphereLayer->getNumLayers() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mCompositeSphereLayer);
        }
        setState(State::NONE);
        mSphere = nullptr;
        mSphereProperties->setProperty("visible", false);
        mapItem()->removeNode(iconNode());
    }
}

void DrawSphere::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Sphere" + QString::number(mCount);
    mSphere = new SphereNode();
    mSphere->setName(name.toStdString());
    mSphere->setRadius(mSphereProperties->getRadius());
//    mSphere->setHeight(mSphereProperties->getHeight());
    mSphere->setPosition(geoPos);
    //    mapItem()->getMapObject()->addNodeToLayer(mSphere, mSphereLayer);
    //    mSphereProperties->setSphere(mSphere, );

    mSphereLayer = new ParenticAnnotationLayer();
    mSphereLayer->addChild(mSphere);
    mSphereLayer->setName(mSphere->getName());
    mCompositeSphereLayer->addLayer(mSphereLayer);
    mSphereProperties->setSphere(mSphere, mapItem()->getMapSRS());

    setState(State::DRAWING);
    mCount++;
}

void DrawSphere::cancelDraw()
{
    if(state() == State::DRAWING){
        //        mapItem()->getMapObject()->removeNodeFromLayer(mSphere, mSphereLayer);
        mCompositeSphereLayer->removeLayer(mSphereLayer);
        mSphere = nullptr;
        mSphereLayer = nullptr;
        //mSphereProperties->setSphere(mSphere);
        mSphereProperties->setSphere(mSphere, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}

void DrawSphere::drawing(const osgEarth::GeoPoint &geoPos)
{
    mSphere->setPosition(geoPos);
    mSphereProperties->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}

void DrawSphere::createProperty()
{
    QQmlComponent* comp = new QQmlComponent(qmlEngine());
    connect(comp, &QQmlComponent::statusChanged, [comp, this](){
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }
        QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
        mSphereProperties = static_cast<SphereProperties*>(item);

        mainWindow()->addDockItem(mSphereProperties, 0.3);
    });


    comp->loadUrl(QUrl("qrc:/Properties.qml"));
}

//bool DrawSphere::setup()
//{
//    auto toolboxItem =  new ToolboxItem{SPHERE, CATEGORY, "qrc:/resources/sphere.png", true};
//    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawSphere::onSphereItemCheck);
//    toolbox()->addItem(toolboxItem);

//    makeIconNode("../data/images/draw/sphere.png");
