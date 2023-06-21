#include "drawBox.h"

int DrawBox::mCount{0};

DrawBox::DrawBox(QObject *parent): DrawShape(parent)
{
//    Q_INIT_RESOURCE(drawBox);
    qmlRegisterType<BoxPropertiesModel>("Crystal", 1, 0, "BoxProperties");
}

//bool DrawBox::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
//{
////    qmlRegisterType<BoxPropertiesModel>("Crystal", 1, 0, "BoxProperties");

////    desc->toolboxItemsList.push_back(new ItemDesc{BOX, CATEGORY, "qrc:/resources/box.png", true,  false, ""});

//    return true;
//}

bool DrawBox::setup()
{
//    DrawShape::setup();

    auto toolboxItem =  new ToolboxItem{BOX, CATEGORY, "qrc:/resources/box.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawBox::onBoxItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/box.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

//    addLayer();
    mBoxLayer = new osgEarth::Annotation::AnnotationLayer();
    mBoxLayer->setName(BOX);
//    mBoxLayer->getGroup()->setName(BOX);
//    shapeLayer()->getGroup()->addChild(mBoxLayer->getGroup());
//    emit mapItem()->layerChanged();
//    mapItem()->addLayer(boxLayer);

    return true;
}

void DrawBox::onBoxItemCheck(bool check)
{
    if (check) {
        mapItem()->addLayerToLayer(mBoxLayer, CATEGORY);
        setState(State::READY);
        mBoxProperties = new BoxProperties(mBox, qmlEngine(), uiHandle(), mapItem());
        mBoxProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(mBoxLayer->getGroup()->getNumChildren() <= 0)
            mapItem()->removeLayerFromLayer(mBoxLayer, CATEGORY);
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
    mBox->setHeight(100000);
    mBox->setWidth(100000);
    mBox->setLength(100000);
    mBox->setPosition(geoPos);

    mapItem()->addNodeToLayer(mBox, BOX);
//    mBoxLayer->addChild(mBox);
//    emit mapItem()->layerChanged();

    mBoxProperties->setBox(mBox);

    setState(State::EDIT);
    mCount++;
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

