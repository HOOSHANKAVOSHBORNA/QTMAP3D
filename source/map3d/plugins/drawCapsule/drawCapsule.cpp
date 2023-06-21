#include "drawCapsule.h"
int DrawCapsule::mCount{0};
DrawCapsule::DrawCapsule(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<CapsulePropertiesModel>("Crystal", 1, 0, "CapsuleProperties");
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
        mapItem()->addLayerToLayer(mCapsuleLayer, CATEGORY);
        setState(State::READY);
        mCapsuleProperties = new CapsuleProperties(mCapsule, qmlEngine(), uiHandle(), mapItem());
        mCapsuleProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(mCapsuleLayer->getGroup()->getNumChildren() <= 0)
            mapItem()->removeLayerFromLayer(mCapsuleLayer, CATEGORY);
        if(state() == State::EDIT)
            cancelDraw();
        setState(State::NONE);
        mCapsule = nullptr;
        mCapsuleProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawCapsule::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "box" + QString::number(mCount);
    mCapsule = new Capsule();
    mCapsule->setName(name.toStdString());

    mCapsule->setPosition(geoPos);

    mapItem()->addNodeToLayer(mCapsule, CAPSULE);
    mCapsuleProperties->setCapsule(mCapsule);

    setState(State::EDIT);
    mCount++;
}

void DrawCapsule::cancelDraw()
{
    if(state() == State::EDIT){
        mapItem()->removeNodeFromLayer(mCapsule, CAPSULE);
        mCapsule = nullptr;
        mCapsuleProperties->setCapsule(mCapsule);
        setState(State::READY);
        mCount--;
    }
}
