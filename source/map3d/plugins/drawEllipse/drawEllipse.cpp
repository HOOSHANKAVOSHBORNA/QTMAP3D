#include "drawEllipse.h"
#include "utility.h"
#include <QQmlContext>
#include "plugininterface.h"
#include "mapItem.h"

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

    mEllipseLayer = new ParenticAnnotationLayer();
    mEllipseLayer->setName(ELLIPSE);
    return true;
}

void DrawEllipse::onEllipseItemCheck(bool check)
{
    if (check) {
        setState(State::READY);
        mEllipseProperty = new EllipseProperty();
        createProperty("Ellipse", QVariant::fromValue<EllipseProperty*>(mEllipseProperty));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

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
    mEllipse->setPosition(geoPos);
    osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(mEllipseProperty->getClamp());
    mEllipse->setClamp(clampEnum);
    mEllipse->setHeight(mEllipseProperty->getHeight());
    mEllipse->setStrokeWidth(mEllipseProperty->getStrokeWidth());

    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mEllipseLayer)){
        mEllipseLayer->clear();
        shapeLayer->addLayer(mEllipseLayer);
    }

    mEllipseLayer->addChild(mEllipse);
    mEllipseLayer->setName(mEllipse->getName());
    mEllipseProperty->setEllipse(mEllipse, mapItem()->getMapSRS());
    setState(State::DRAWING);
    mCount++;

}

void DrawEllipse::cancelDraw()
{



    if(state() == State::DRAWING){
        mEllipseLayer->getGroup()->removeChild(mEllipse);
        mEllipse = nullptr;
        mEllipseProperty->setEllipse(mEllipse, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;

        if(!mEllipseLayer->hasNode())
            DrawShape::shapeLayer()->removeLayer(mEllipseLayer);
    }

}

void DrawEllipse::drawing(const osgEarth::GeoPoint &geoPos)
{
    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mEllipseLayer)){
        initDraw(geoPos);
    }

    mEllipse->setPosition(geoPos);
    mEllipseProperty->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}
