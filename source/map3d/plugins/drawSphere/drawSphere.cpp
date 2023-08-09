#include "drawSphere.h"
#include "plugininterface.h"
#include "mapItem.h"
#include "utility.h"
#include <QQmlContext>

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

    mSphereLayer = new ParenticAnnotationLayer();
    mSphereLayer->setName(SPHERE);
    return true;
}

void DrawSphere::onSphereItemCheck(bool check)
{
    if (check) {
        setState(State::READY);
        mSphereProperty = new SphereProperty;
        createProperty("Sphere", QVariant::fromValue<SphereProperty*>(mSphereProperty));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        setState(State::NONE);
        mSphere = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawSphere::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Sphere" + QString::number(mCount);
    mSphere = new SphereNode();
    mSphere->setName(name.toStdString());
    mSphere->setPosition(geoPos);

    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mSphereLayer)){
        mSphereLayer->clear();
        shapeLayer->addLayer(mSphereLayer);
    }

    mSphereProperty->setSphere(mSphere, mapItem()->getMapSRS());
    setState(State::DRAWING);
    mCount++;
}

void DrawSphere::cancelDraw()
{
    if(state() == State::DRAWING){
        mSphereLayer->removeChild(mSphere);
        mSphere = nullptr;
        mSphereProperty->setSphere(mSphere, mapItem()->getMapSRS());
        setState(State::READY);
        mCount--;

        if(!mSphereLayer->hasNode()){
            DrawShape::shapeLayer()->removeLayer(mSphereLayer);
        }
    }
}

void DrawSphere::drawing(const osgEarth::GeoPoint &geoPos)
{
    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mSphereLayer)){
        initDraw(geoPos);
    }

    mSphere->setPosition(geoPos);
    mSphereProperty->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}

