#include "drawEllipse.h"
#include "utility.h"
#include "mainwindow.h"

int DrawEllipse::mCount{0};
DrawEllipse::DrawEllipse(QObject *parent): DrawShape(parent)
{
    //    qmlRegisterType<EllipsePropertiesModel>("Crystal", 1, 0, "EllipseProperties");
    //    qmlRegisterType<EllipseProperties>("Crystal", 1, 0, "CProperty");
}

bool DrawEllipse::setup()
{
    auto toolboxItem =  new ToolboxItem{ELLIPSE, CATEGORY, "qrc:/resources/ellipse.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawEllipse::onEllipseItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/ellipse.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    //    mEllipseLayer = new CompositeAnnotationLayer();
    //    mEllipseLayer->setName(Ellipse);
    mCompositeEllipseLayer = new CompositeAnnotationLayer();
    mCompositeEllipseLayer->setName(ELLIPSE);

    return true;
}

void DrawEllipse::onEllipseItemCheck(bool check)
{

    qmlRegisterType<EllipseProperties>("Crystal", 1, 0, "CProperty");
    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCompositeEllipseLayer->getName()));
        if(!layer){
            mCompositeEllipseLayer->clearLayers();
        }
        if(mCompositeEllipseLayer->getNumLayers() <= 0){

            //            mapItem()->getMapObject()->addLayer(mEllipseLayer, shapeLayer);
            shapeLayer->addLayer(mCompositeEllipseLayer);
        }
        setState(State::READY);

        createProperty();
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mCompositeEllipseLayer->getNumLayers() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            //            mapItem()->getMapObject()->removeLayer(mEllipseLayer, shapeLayer);
            shapeLayer->removeLayer(mCompositeEllipseLayer);
        }
        setState(State::NONE);
        mEllipse = nullptr;
        mEllipseProperties->setProperty("visible", false);
        mapItem()->removeNode(iconNode());
    }}

void DrawEllipse::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Ellipse" + QString::number(mCount);
    mEllipse = new Ellipse();
    mEllipse->setName(name.toStdString());
    //    mEllipse->setRadius(mEllipseProperties->getRadius());
    mEllipse->setHeight(mEllipseProperties->getHeight());
    mEllipse->setStrokeColor(Utility::qColor2osgEarthColor(mEllipseProperties->getStroke()));
    mEllipse->setStrokeWidth(mEllipseProperties->getStrokeWidth());
    osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(mEllipseProperties->getClamp());
    mEllipse->setClamp(clampEnum);

    mEllipse->setPosition(geoPos);

    //    mapItem()->getMapObject()->addNodeToLayer(mEllipse, mEllipseLayer);
    //    mEllipseProperties->setEllipse(mEllipse);

    setState(State::DRAWING);
    mCount++;
    //    mCompositeEllipseLayer->addLayer(mEllipseLayer);
    mEllipseLayer = new ParenticAnnotationLayer();
    mEllipseLayer->addChild(mEllipse);
    mEllipseLayer->setName(mEllipse->getName());
    mCompositeEllipseLayer->addLayer(mEllipseLayer);

    mEllipseProperties->setEllipse(mEllipse, mapItem()->getMapSRS());

    setState(State::DRAWING);
    mCount++;




}

void DrawEllipse::cancelDraw()
{



    if(state() == State::DRAWING){
        //        mapItem()->getMapObject()->removeNodeFromLayer(mEllipse, mEllipseLayer);
        mCompositeEllipseLayer->removeLayer(mEllipseLayer);
        mEllipse = nullptr;
        mEllipseLayer = nullptr;
        //mEllipseProperties->setEllipse(mEllipse);
        mEllipseProperties->setEllipse(mEllipse, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}

void DrawEllipse::drawing(const osgEarth::GeoPoint &geoPos)
{
    mEllipse->setPosition(geoPos);
    mEllipseProperties->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}
void DrawEllipse::createProperty()
{
    QQmlComponent* comp = new QQmlComponent(qmlEngine());
    connect(comp, &QQmlComponent::statusChanged, [comp, this](){
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }
        QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
        mEllipseProperties = static_cast<EllipseProperties*>(item);


        mainWindow()->addDockItem(mEllipseProperties, 0.3);
    });


    comp->loadUrl(QUrl("qrc:/Properties.qml"));
}
