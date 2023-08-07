#include "drawBox.h"
#include "plugininterface.h"
#include "mapItem.h"
#include "utility.h"
#include <QQmlContext>

int DrawBox::mCount{0};

DrawBox::DrawBox(QObject *parent): DrawShape(parent)
{
}

bool DrawBox::setup()
{
    auto toolboxItem =  new ToolboxItem{BOX, CATEGORY, "qrc:/resources/box.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawBox::onBoxItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/box.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mBoxLayer =  new ParenticAnnotationLayer();
    mBoxLayer->setName(BOX);
    return true;
}

void DrawBox::onBoxItemCheck(bool check)
{
    if (check) {
        setState(State::READY);
        mBoxProperty = new BoxProperty();
        createProperty("Box", QVariant::fromValue<BoxProperty*>(mBoxProperty));
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        setState(State::NONE);
        mBox = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawBox::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "box" + QString::number(mCount);
    mBox = new Box();
    mBox->setName(name.toStdString());
    mBox->setPosition(geoPos);

    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mBoxLayer)){
        mBoxLayer->clear();
        shapeLayer->addLayer(mBoxLayer);
    }

    mBoxLayer->addChild(mBox);
//    qDebug()<<"number of nods:"<<mBoxLayer->getNumberOfNodes();
    mBoxProperty->setBox(mBox, mapItem()->getMapSRS());
    setState(State::DRAWING);
    mCount++;
}

void DrawBox::drawing(const osgEarth::GeoPoint &geoPos)
{
    auto shapeLayer = DrawShape::shapeLayer();
    if(!shapeLayer->containsLayer(mBoxLayer)){
        initDraw(geoPos);
    }

    mBox->setPosition(geoPos);
    mBoxProperty->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
}

void DrawBox::cancelDraw()
{
        if(state() == State::DRAWING){
            mBoxLayer->removeChild(mBox);
            mBox = nullptr;
            mBoxProperty->setBox(mBox, mapItem()->getMapSRS());
            setState(State::READY);
            mCount--;

            if(!mBoxLayer->hasNode())
                DrawShape::shapeLayer()->removeLayer(mBoxLayer);
        }
}

