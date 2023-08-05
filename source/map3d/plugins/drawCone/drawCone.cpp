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



    mConeLayer = new ParenticAnnotationLayer();
    mConeLayer->setName(CONE);
    return true;    
}

void DrawCone::onConeItemCheck(bool check)
{



    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mConeLayer->getName()));
        if(!layer){
            mConeLayer->getGroup()->removeChildren(0, mConeLayer->getGroup()->getNumChildren());
        }
        if(mConeLayer->getGroup()->getNumChildren() <= 0){
            shapeLayer->addLayer(mConeLayer);
        }
        setState(State::READY);
        mConeProperties = new ConeProperties();
        createProperty("Cone", QVariant::fromValue<ConeProperties*>(mConeProperties));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mConeLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            shapeLayer->removeLayer(mConeLayer);
        }
        setState(State::NONE);
        mCone = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawCone::initDraw(const osgEarth::GeoPoint &geoPos)
{

    QString name = "Cone" + QString::number(mCount);
    mCone = new Cone();
    mCone->setName(name.toStdString());
    mCone->setPosition(geoPos);
    mConeLayer->addChild(mCone);
    mConeProperties->setCone(mCone, mapItem()->getMapSRS());
    mConeLayer->setName(mCone->getName());
    setState(State::DRAWING);
    mCount++;
}

void DrawCone::cancelDraw()
{
    if(state() == State::DRAWING){
        mConeLayer->getGroup()->removeChild(mCone);
        mCone = nullptr;
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

