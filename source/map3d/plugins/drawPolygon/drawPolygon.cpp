#include "drawPolygon.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>

#include "utility.h"
#include "mainwindow.h"

int DrawPolygon::mCount{0};
DrawPolygon::DrawPolygon(QObject *parent): DrawShape(parent)
{
    //    qmlRegisterType<PolygonPropertiesModel>("Crystal", 1, 0, "PolygonProperties");
    //    qmlRegisterType<PolygonProperties>("Crystal", 1, 0, "CProperty");
}

bool DrawPolygon::setup()
{
    auto toolboxItem =  new ToolboxItem{POLYGON, CATEGORY, "qrc:/resources/polygon.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawPolygon::onPolygonItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/polygon.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    //    mPolygonLayer = new CompositeAnnotationLayer();
    //    mPolygonLayer->setName(Polygon);
    mCompositePolygonLayer = new CompositeAnnotationLayer();
    mCompositePolygonLayer->setName(POLYGON);

    return true;
}

void DrawPolygon::onPolygonItemCheck(bool check)
{

    qmlRegisterType<PolygonProperties>("Crystal", 1, 0, "CProperty");
    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCompositePolygonLayer->getName()));
        if(!layer){
            mCompositePolygonLayer->clearLayers();
        }
        if(mCompositePolygonLayer->getNumLayers() <= 0){

            //            mapItem()->getMapObject()->addLayer(mPolygonLayer, shapeLayer);
            shapeLayer->addLayer(mCompositePolygonLayer);
        }
        setState(State::READY);

        createProperty();
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mCompositePolygonLayer->getNumLayers() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            //            mapItem()->getMapObject()->removeLayer(mPolygonLayer, shapeLayer);
            shapeLayer->removeLayer(mCompositePolygonLayer);
        }
        setState(State::NONE);
        mPolygon = nullptr;
        mPolygonProperties->setProperty("visible", false);
        mapItem()->removeNode(iconNode());
    }}

void DrawPolygon::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Polygon" + QString::number(mCount);
    mPolygon = new Polygon(mapItem());
    mPolygon->setName(name.toStdString());
    //    mPolygon->setRadius(mPolygonProperties->getRadius());
    mPolygon->setHeight(mPolygonProperties->getHeight());
    mPolygon->setLineColor(Utility::qColor2osgEarthColor(mPolygonProperties->getStroke()));
    mPolygon->setLineWidth(mPolygonProperties->getStrokeWidth());
    osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(mPolygonProperties->getClamp());
    mPolygon->setClamp(clampEnum);



    setState(State::DRAWING);


    mCount++;
    mPolygonLayer = new ParenticAnnotationLayer();
    mPolygonLayer->addChild(mPolygon);
    mPolygonLayer->setName(mPolygon->getName());
    mCompositePolygonLayer->addLayer(mPolygonLayer);

    mPolygonProperties->setPolygon(mPolygon, mapItem()->getMapSRS());
    setState(State::DRAWING);



    mCount++;





}

void DrawPolygon::cancelDraw()
{



    if(state() == State::DRAWING){
        //        mapItem()->getMapObject()->removeNodeFromLayer(mPolygon, mPolygonLayer);
        mCompositePolygonLayer->removeLayer(mPolygonLayer);
        mPolygon = nullptr;
        mPolygonLayer = nullptr;
        //mPolygonProperties->setPolygon(mPolygon);
        mPolygonProperties->setPolygon(mPolygon, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}


void DrawPolygon::createProperty()
{
    QQmlComponent* comp = new QQmlComponent(qmlEngine());
    connect(comp, &QQmlComponent::statusChanged, [comp, this](){
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }
        QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
        mPolygonProperties = static_cast<PolygonProperties*>(item);


        mainWindow()->addDockItem(mPolygonProperties, 0.3);
    });


    comp->loadUrl(QUrl("qrc:/Properties.qml"));
}

