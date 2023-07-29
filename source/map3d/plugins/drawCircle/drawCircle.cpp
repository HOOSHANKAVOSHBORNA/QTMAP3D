#include "drawCircle.h"
#include "utility.h"
#include "mainwindow.h"
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

int DrawCircle::mCount{0};
DrawCircle::DrawCircle(QObject *parent): DrawShape(parent)
{
    //    qmlRegisterType<CirclePropertiesModel>("Crystal", 1, 0, "CircleProperties");
    //    qmlRegisterType<CircleProperties>("Crystal", 1, 0, "CProperty");
}

bool DrawCircle::setup()
{
    auto toolboxItem =  new ToolboxItem{CIRCLE, CATEGORY, "qrc:/resources/circle.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCircle::onCircleItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/circle.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    //    mCircleLayer = new CompositeAnnotationLayer();
    //    mCircleLayer->setName(CIRCLE);
    mCompositeCircleLayer = new CompositeAnnotationLayer();
    mCompositeCircleLayer->setName(CIRCLE);

    return true;
}

void DrawCircle::onCircleItemCheck(bool check)
{

        qmlRegisterType<CircleProperties>("Crystal", 1, 0, "CProperty");
        if (check) {
            auto shapeLayer = DrawShape::shapeLayer();
            auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCompositeCircleLayer->getName()));
            if(!layer){
                mCompositeCircleLayer->clearLayers();
            }
            if(mCompositeCircleLayer->getNumLayers() <= 0){

                //            mapItem()->getMapObject()->addLayer(mCircleLayer, shapeLayer);
                shapeLayer->addLayer(mCompositeCircleLayer);
            }
            setState(State::READY);

            createProperty();
            mapItem()->addNode(iconNode());

        }
        else {
            if(state() == State::DRAWING)
                cancelDraw();

            if(mCompositeCircleLayer->getNumLayers() <= 0){
                auto shapeLayer = DrawShape::shapeLayer();
                //            mapItem()->getMapObject()->removeLayer(mCircleLayer, shapeLayer);
                shapeLayer->removeLayer(mCompositeCircleLayer);
            }
            setState(State::NONE);
            mCircle = nullptr;
            mCircleProperties->setProperty("visible", false);
            mapItem()->removeNode(iconNode());
        }}

    void DrawCircle::initDraw(const osgEarth::GeoPoint &geoPos)
    {
        QString name = "Circle" + QString::number(mCount);
        mCircle = new Circle();
        mCircle->setName(name.toStdString());
        mCircle->setRadius(mCircleProperties->getRadius());
        mCircle->setCircleHeight(mCircleProperties->getHeight());
        mCircle->setArcEnd(mCircleProperties->getArc().x());
        mCircle->setArcStart(mCircleProperties->getArc().y());
        mCircle->setLineColor(Utility::qColor2osgEarthColor(mCircleProperties->getStroke()));
        mCircle->setLineWidth(mCircleProperties->getStrokeWidth());
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(mCircleProperties->getClamp());
        mCircle->setClamp(clampEnum);

        mCircle->setPosition(geoPos);

        //    mapItem()->getMapObject()->addNodeToLayer(mCircle, mCircleLayer);
        //    mCircleProperties->setCircle(mCircle);

        setState(State::DRAWING);
        mCount++;
        //    mCompositeCircleLayer->addLayer(mCircleLayer);
        mCircleLayer = new ParenticAnnotationLayer();
        mCircleLayer->addChild(mCircle);
        mCircleLayer->setName(mCircle->getName());
        mCompositeCircleLayer->addLayer(mCircleLayer);

        mCircleProperties->setCircle(mCircle, mapItem()->getMapSRS());

        setState(State::DRAWING);
        mCount++;




    }

    void DrawCircle::cancelDraw()
    {


        if(state() == State::DRAWING){
            //        mapItem()->getMapObject()->removeNodeFromLayer(mCircle, mCircleLayer);
            mCompositeCircleLayer->removeLayer(mCircleLayer);
            mCircle = nullptr;
            mCircleLayer = nullptr;
            //mCircleProperties->setCircle(mCircle);
            mCircleProperties->setCircle(mCircle, mapItem()->getMapSRS());
            setState(State::READY);
            mCount--;
        }
    }

    void DrawCircle::drawing(const osgEarth::GeoPoint &geoPos)
    {
        mCircle->setPosition(geoPos);
        mCircleProperties->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
    }
    void DrawCircle::createProperty()
    {
        QQmlComponent* comp = new QQmlComponent(qmlEngine());
        connect(comp, &QQmlComponent::statusChanged, [comp, this](){
            if (comp->status() == QQmlComponent::Status::Error) {
                qDebug() << comp->errorString();
            }
            QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
            mCircleProperties = static_cast<CircleProperties*>(item);


            mainWindow()->addToRightContainer(mCircleProperties, "Circle");
        });


        comp->loadUrl(QUrl("qrc:/Properties.qml"));
    }


