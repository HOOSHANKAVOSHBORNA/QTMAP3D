#include "drawCapsule.h"
int DrawCapsule::mCount{0};
DrawCapsule::DrawCapsule(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<CapsuleProperties>("Crystal", 1, 0, "CapsuleProperties");
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
    if (check) {
        if(mCapsuleLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->addLayer(mCapsuleLayer, shapeLayer);
        }
        setState(State::READY);
        mapItem()->addNode(iconNode());
        //mCapsuleProperties = new CapsuleProperties(mCapsule, qmlEngine(), uiHandle(), mapItem());
        //mCapsuleProperties->show();
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
    mCapsule->setRadius(20000);
    mCapsule->setPosition(geoPos);
    mapItem()->getMapObject()->addNodeToLayer(mCapsule, mCapsuleLayer);
    //mCapsuleProperties->setCapsule(mCapsule);

    setState(State::DRAWING);
    mCount++;
}

void DrawCapsule::cancelDraw()
{
    if(state() == State::DRAWING){
        mapItem()->getMapObject()->removeNodeFromLayer(mCapsule, mCapsuleLayer);
        mCapsule = nullptr;
        //mCapsuleProperties->setCapsule(mCapsule);
        setState(State::READY);
        mCount--;
    }
}
