#include "drawCone.h"
#include "mainwindow.h"
#include "utility.h"
#include "compositeAnnotationLayer.h"


int DrawCone::mCount{0};
DrawCone::DrawCone(QObject *parent): DrawShape(parent)
{

}

bool DrawCone::setup()
{
    auto toolboxItem =  new ToolboxItem{CONE, CATEGORY, "qrc:/resources/cone.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCone::onConeItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/cone.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());



    mCompositeConeLayer = new CompositeAnnotationLayer();
    mCompositeConeLayer->setName(CONE);

    return true;
}

void DrawCone::onConeItemCheck(bool check)
{
    qmlRegisterType<ConeProperties>("Crystal", 1, 0, "CProperty");

    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCompositeConeLayer->getName()));
        if(!layer){
            mCompositeConeLayer->clearLayers();
        }
        if(mCompositeConeLayer->getNumLayers() <= 0){

            //            mapItem()->getMapObject()->addLayer(mBoxLayer, shapeLayer);
            shapeLayer->addLayer(mCompositeConeLayer);
        }
        setState(State::READY);
        mapItem()->addNode(iconNode());


        createProperty();




        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();
        if(mCompositeConeLayer->getNumLayers() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mCompositeConeLayer);
        }
        setState(State::NONE);
        mCone = nullptr;
        mConeProperties->setProperty("visible", false);
        mapItem()->removeNode(iconNode());
    }
}

void DrawCone::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Cone" + QString::number(mCount);
    mCone = new Cone();
    mCone->setName(name.toStdString());
    mCone->setRadius(mConeProperties->getRadius());
    mCone->setHeight(mConeProperties->getHeight());
    mCone->setPosition(geoPos);


    mConeLayer = new ParenticAnnotationLayer();
    mConeLayer->addChild(mCone);
    mConeLayer->setName(mCone->getName());
    mCompositeConeLayer->addLayer(mConeLayer);
    mConeProperties->setCone(mCone, mapItem()->getMapSRS());

    setState(State::DRAWING);
    mCount++;
}

void DrawCone::cancelDraw()
{
    if(state() == State::DRAWING){
        //        mapItem()->getMapObject()->removeNodeFromLayer(mCone, mConeLayer);
        mCompositeConeLayer->removeLayer(mConeLayer);
        mCone = nullptr;
        mConeLayer = nullptr;
        //mConeProperties->setCone(mCone);
        mConeProperties->setCone(mCone, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}

void DrawCone::drawing(const osgEarth::GeoPoint &geoPos)
{
    mCone->setPosition(geoPos);
    mConeProperties->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}

void DrawCone::createProperty()
{
    QQmlComponent* comp = new QQmlComponent(qmlEngine());
    connect(comp, &QQmlComponent::statusChanged, [comp, this](){
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }
        //            QQmlContext *context = new QQmlContext(qmlEngine(), this);
        QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
        mConeProperties = static_cast<ConeProperties*>(item);
        //            mBoxProperties->setFillColorStatus(false);
        //            mBoxProperties->setFillColor(QColor());
        //            mBoxProperty->setStatuses();

        //        mBoxProperties = new BoxProperties();
        mainWindow()->addToRightContainer(mConeProperties, "Cone");
    });


    comp->loadUrl(QUrl("qrc:/Properties.qml"));
}
