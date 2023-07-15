#include "drawBox.h"

int DrawBox::mCount{0};

DrawBox::DrawBox(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<BoxPropertiesModel>("Crystal", 1, 0, "BoxProperties");
}

bool DrawBox::setup()
{
    auto toolboxItem =  new ToolboxItem{BOX, CATEGORY, "qrc:/resources/box.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawBox::onBoxItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/box.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

//    addLayer();
    mCompositeBoxLayer = new CompositeAnnotationLayer();
    mCompositeBoxLayer->setName(BOX);
    return true;
}

void DrawBox::onBoxItemCheck(bool check)
{
    if (check) {
        auto shapeLayer = DrawShape::shapeLayer();
        auto layer = shapeLayer->getLayerByName(QString::fromStdString(mCompositeBoxLayer->getName()));
        if(!layer){
            mCompositeBoxLayer->clearLayers();
        }
        if(mCompositeBoxLayer->getNumLayers() <= 0){

//            mapItem()->getMapObject()->addLayer(mBoxLayer, shapeLayer);
            shapeLayer->addLayer(mCompositeBoxLayer);
        }
        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mCompositeBoxLayer->getNumLayers() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
//            mapItem()->getMapObject()->removeLayer(mBoxLayer, shapeLayer);
            shapeLayer->removeLayer(mCompositeBoxLayer);
        }
        setState(State::NONE);
        mBox = nullptr;
        //mBoxProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawBox::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "box" + QString::number(mCount);
    mBox = new Box();
    mBox->setName(name.toStdString());
    mBox->setHeight(100000);
    mBox->setWidth(100000);
    mBox->setLength(100000);
    mBox->setPosition(geoPos);
    mBoxLayer = new ParenticAnnotationLayer();
    mBoxLayer->setNode(mBox);
    mBoxLayer->setName(mBox->getName());
//    mapItem()->getMapObject()->addNodeToLayer(mBox, mBoxLayer);
    mCompositeBoxLayer->addLayer(mBoxLayer);

//    mBoxProperties->setBox(mBox);

    setState(State::DRAWING);
    mCount++;
}

void DrawBox::drawing(const osgEarth::GeoPoint &geoPos)
{
    mBox->setPosition(geoPos);
}

void DrawBox::cancelDraw()
{
    if(state() == State::DRAWING){
//        mapItem()->getMapObject()->removeNodeFromLayer(mBox, mCompositeBoxLayer);
//        mapItem()->getMapObject()->removeLayer(mBoxLayer, mCompositeBoxLayer);
        mCompositeBoxLayer->removeLayer(mBoxLayer);
        mBox = nullptr;
        mBoxLayer = nullptr;
        //mBoxProperties->setBox(mBox);
        setState(State::READY);
        mCount--;
    }
}

