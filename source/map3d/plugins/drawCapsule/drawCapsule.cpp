#include "drawCapsule.h"
#include "mainwindow.h"
#include "utility.h"
int DrawCapsule::mCount{0};
DrawCapsule::DrawCapsule(QObject *parent): DrawShape(parent)
{
}

bool DrawCapsule::setup()
{
    auto toolboxItem =  new ToolboxItem{CAPSULE, CATEGORY, "qrc:/resources/capsule.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCapsule::onCapsuleItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/capsule.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mCapsuleLayer = new osgEarth::Annotation::AnnotationLayer();
    mCapsuleLayer->setName(CAPSULE);

    return true;
}

void DrawCapsule::onCapsuleItemCheck(bool check)
{
    qmlRegisterType<CapsuleProperties>("Crystal", 1, 0, "CProperty");

    if (check) {
        if(mCapsuleLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->addLayer(mCapsuleLayer, shapeLayer);
        }
        setState(State::READY);
        mapItem()->addNode(iconNode());
        //mCapsuleProperties = new CapsuleProperties(mCapsule, qmlEngine(), uiHandle(), mapItem());
        //mCapsuleProperties->show();

        createProperty();




        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();
        if(mCapsuleLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->removeLayer(mCapsuleLayer, shapeLayer);
        }
        setState(State::NONE);
        mCapsule = nullptr;
        //mCapsuleProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawCapsule::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Capsule" + QString::number(mCount);
    mCapsule = new Capsule();
    mCapsule->setName(name.toStdString());
    mCapsule->setRadius(mCapsuleProperties->getRadius());
    mCapsule->setHeight(mCapsuleProperties->getHeight());
    mCapsule->setPosition(geoPos);
    mapItem()->getMapObject()->addNodeToLayer(mCapsule, mCapsuleLayer);
//    mCapsuleProperties->setCapsule(mCapsule, );

    mCapsuleProperties->setCapsule(mCapsule, mapItem()->getMapSRS());
    mCapsule->setColor(Utility::qColor2osgEarthColor(mCapsuleProperties->getFillColor()));


    setState(State::DRAWING);
    mCount++;
}

void DrawCapsule::cancelDraw()
{
    if(state() == State::DRAWING){
        mapItem()->getMapObject()->removeNodeFromLayer(mCapsule, mCapsuleLayer);
        mCapsule = nullptr;
        //mCapsuleProperties->setCapsule(mCapsule);
        mCapsuleProperties->setCapsule(mCapsule, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;
    }
}

void DrawCapsule::drawing(const osgEarth::GeoPoint &geoPos)
{
    mCapsule->setPosition(geoPos);
    mCapsuleProperties->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}

void DrawCapsule::createProperty()
{
    QQmlComponent* comp = new QQmlComponent(qmlEngine());
    connect(comp, &QQmlComponent::statusChanged, [comp, this](){
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }
        //            QQmlContext *context = new QQmlContext(qmlEngine(), this);
        QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
        mCapsuleProperties = static_cast<CapsuleProperties*>(item);
        //            mBoxProperties->setFillColorStatus(false);
        //            mBoxProperties->setFillColor(QColor());
        //            mBoxProperty->setStatuses();

        //        mBoxProperties = new BoxProperties();
        mainWindow()->addDockItem(mCapsuleProperties, 0.3);
    });


    comp->loadUrl(QUrl("qrc:/Properties.qml"));
}

