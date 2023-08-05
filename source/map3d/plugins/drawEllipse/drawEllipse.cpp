#include "drawEllipse.h"
#include "utility.h"
#include "mainwindow.h"

int DrawEllipse::mCount{0};
DrawEllipse::DrawEllipse(QObject *parent): DrawShape(parent)
{   }

bool DrawEllipse::setup()
{
    auto toolboxItem =  new ToolboxItem{ELLIPSE, CATEGORY, "qrc:/resources/ellipse.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawEllipse::onEllipseItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/ellipse.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    //    mEllipseLayer = new CompositeAnnotationLayer();
    //    mEllipseLayer->setName(Ellipse);
    mEllipseLayer = new ParenticAnnotationLayer();
    mEllipseLayer->setName(ELLIPSE);

    return true;
}

void DrawEllipse::onEllipseItemCheck(bool check)
{


    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mEllipseLayer->getName()));
        if(!layer){
            mEllipseLayer->getGroup()->removeChildren(0, mEllipseLayer->getGroup()->getNumChildren());
        }
        if(mEllipseLayer->getGroup()->getNumChildren() <= 0){
            shapeLayer->addLayer(mEllipseLayer);
        }
        setState(State::READY);
        mEllipseProperties = new EllipseProperties();
        createProperty("Ellipse", QVariant::fromValue<EllipseProperties*>(mEllipseProperties));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mEllipseLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mEllipseLayer);
        }
        setState(State::NONE);
        mEllipse = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawEllipse::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Ellipse" + QString::number(mCount);
    mEllipse = new Ellipse();
    mEllipse->setName(name.toStdString());

//    mEllipse->setRadii(mEllipseProperties->getRadius());
    mEllipse->setHeight(mEllipseProperties->getHeight());
    mEllipse->setStrokeColor(Utility::qColor2osgEarthColor(mEllipseProperties->getStroke()));
    mEllipse->setStrokeWidth(mEllipseProperties->getStrokeWidth());
    osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(mEllipseProperties->getClamp());
    mEllipse->setClamp(clampEnum);

    mEllipse->setPosition(geoPos);



    mEllipseLayer->addChild(mEllipse);
    mEllipseLayer->setName(mEllipse->getName());


    mEllipseProperties->setEllipse(mEllipse, mapItem()->getMapSRS());

    setState(State::DRAWING);
    mCount++;




}

void DrawEllipse::cancelDraw()
{



    if(state() == State::DRAWING){
        mEllipseLayer->getGroup()->removeChild(mEllipse);
        mEllipse = nullptr;
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
