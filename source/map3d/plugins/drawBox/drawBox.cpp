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
    mBoxLayer = new osgEarth::Annotation::AnnotationLayer();
    mBoxLayer->setName(BOX);
    return true;
}

void DrawBox::onBoxItemCheck(bool check)
{
    if (check) {
        if(mBoxLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mBoxLayer);
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->setParentLayer(mBoxLayer, shapeLayer);
        }
        setState(State::READY);
        mBoxProperties = new BoxProperties(mBox, qmlEngine(), uiHandle(), mapItem());
        mBoxProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(mBoxLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->setParentLayer(mBoxLayer, nullptr);
            mapItem()->getMapObject()->removeLayer(mBoxLayer);
        }
        if(state() == State::EDIT)
            cancelDraw();
        setState(State::NONE);
        mBox = nullptr;
        mBoxProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawBox::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "box" + QString::number(mCount);
    mBox = new Box();
    mBox->setName(name.toStdString());
//    mBox->setHeight(100000);
//    mBox->setWidth(100000);
//    mBox->setLength(100000);
    mBox->setPosition(geoPos);

    mapItem()->addNodeToLayer(mBox, BOX);
    mBoxProperties->setBox(mBox);

    setState(State::EDIT);
    mCount++;
}

void DrawBox::drawing(const osgEarth::GeoPoint &geoPos)
{
    mBox->setPosition(geoPos);
}

void DrawBox::cancelDraw()
{
    if(state() == State::EDIT){
        mapItem()->removeNodeFromLayer(mBox, BOX);
        mBox = nullptr;
        mBoxProperties->setBox(mBox);
        setState(State::READY);
        mCount--;
    }
}

