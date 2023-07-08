#include "drawCylinder.h"


int DrawCylinder::mCount{0};
DrawCylinder::DrawCylinder(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<CylinderPropertiesModel>("Crystal", 1, 0, "CylinderProperties");
}

bool DrawCylinder::setup()
{
    auto toolboxItem =  new ToolboxItem{CYLINDER, CATEGORY, "qrc:/resources/cylinder.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawCylinder::onCylinderItemCheck);
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
        if(mCylinderLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->addLayer(mCylinderLayer, shapeLayer);
        }
        setState(State::READY);
        mCylinderProperties = new CylinderProperties(mCylinder, qmlEngine(), uiHandle(), mapItem());
        mCylinderProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mCylinderLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->removeLayer(mCylinderLayer, shapeLayer);
        }
        setState(State::NONE);
        mCylinder = nullptr;
        mCylinderProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawCylinder::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Cylinder" + QString::number(mCount);
    mCylinder = new Cylinder();
    mCylinder->setName(name.toStdString());

    mCylinder->setPosition(geoPos);

    mapItem()->getMapObject()->addNodeToLayer(mCylinder, mCylinderLayer);
    mCylinderProperties->setCylinder(mCylinder);

    setState(State::DRAWING);
    mCount++;
}

void DrawCylinder::cancelDraw()
{
    if(state() == State::DRAWING){
        mapItem()->getMapObject()->removeNodeFromLayer(mCylinder, mCylinderLayer);
        mCylinder = nullptr;
        mCylinderProperties->setCylinder(mCylinder);
        setState(State::READY);
        mCount--;
    }
}
