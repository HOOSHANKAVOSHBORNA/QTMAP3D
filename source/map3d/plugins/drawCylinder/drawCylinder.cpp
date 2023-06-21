#include "drawCylinder.h"


int DrawCylinder::mCount{0};
DrawCylinder::DrawCylinder(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<CylinderPropertiesModel>("Crystal", 1, 0, "CylinderProperties");
}

bool DrawCylinder::setup()
{
    auto toolboxItem =  new ToolboxItem{CYLINDER, CATEGORY, "qrc:/resources/cylinder.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCylinder::onConeItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/cylinder.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mCylinderLayer = new osgEarth::Annotation::AnnotationLayer();
    mCylinderLayer->setName(CYLINDER);

    return true;
}

void DrawCylinder::onCylinderItemCheck(bool check)
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

void DrawCylinder::initDraw(const osgEarth::GeoPoint &geoPos)
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

void DrawCylinder::cancelDraw()
{
    if(state() == State::EDIT){
        mapItem()->removeNodeFromLayer(mCone, CONE);
        mCone = nullptr;
        mConeProperties->setCone(mCone);
        setState(State::READY);
        mCount--;
    }
}
