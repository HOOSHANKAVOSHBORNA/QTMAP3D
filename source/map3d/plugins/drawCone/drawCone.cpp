#include "drawCone.h"

int DrawCone::mCount{0};
DrawCone::DrawCone(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<ConePropertiesModel>("Crystal", 1, 0, "ConeProperties");
}

bool DrawCone::setup()
{
    auto toolboxItem =  new ToolboxItem{CONE, CATEGORY, "qrc:/resources/cone.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCone::onConeItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/cone.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mConeLayer = new osgEarth::Annotation::AnnotationLayer();
    mConeLayer->setName(CONE);

    return true;
}

void DrawCone::onConeItemCheck(bool check)
{
    if (check) {
        mapItem()->addLayerToLayer(mConeLayer, CATEGORY);
        setState(State::READY);
        mConeProperties = new ConeProperties(mCone, qmlEngine(), uiHandle(), mapItem());
        mConeProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(mConeLayer->getGroup()->getNumChildren() <= 0)
            mapItem()->removeLayerFromLayer(mConeLayer, CATEGORY);
        if(state() == State::EDIT)
            cancelDraw();
        setState(State::NONE);
        mCone = nullptr;
        mConeProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawCone::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Cone" + QString::number(mCount);
    mCone = new Cone();
    mCone->setName(name.toStdString());

    mCone->setPosition(geoPos);

    mapItem()->addNodeToLayer(mCone, CONE);
    mConeProperties->setCone(mCone);

    setState(State::EDIT);
    mCount++;
}

void DrawCone::cancelDraw()
{
    if(state() == State::EDIT){
        mapItem()->removeNodeFromLayer(mCone, CONE);
        mCone = nullptr;
        mConeProperties->setCone(mCone);
        setState(State::READY);
        mCount--;
    }
}
