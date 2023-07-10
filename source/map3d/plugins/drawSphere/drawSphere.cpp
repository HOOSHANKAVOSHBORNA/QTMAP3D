#include "drawSphere.h"

int DrawSphere::mCount{0};

DrawSphere::DrawSphere(QObject *parent)
    : DrawShape(parent)
{
    qmlRegisterType<SpherePropertiesModel>("Crystal", 1, 0, "SphereProperties");
}

bool DrawSphere::setup()
{
    auto toolboxItem =  new ToolboxItem{SPHERE, CATEGORY, "qrc:/resources/sphere.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawSphere::onSphereItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/sphere.png");

    mSphereLayer = new osgEarth::Annotation::AnnotationLayer();
    mSphereLayer->setName(SPHERE);

    return true;
}

void DrawSphere::onSphereItemCheck(bool check)
{
    if (check) {
        if(mSphereLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->addLayer(mSphereLayer, shapeLayer);
        }
        setState(State::READY);
        mSphereProperties = new SphereProperties(qmlEngine(), uiHandle(), mapItem());
        mSphereProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mSphereLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->removeLayer(mSphereLayer, shapeLayer);
        }
        setState(State::NONE);
        mSphere = nullptr;
        mSphereProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawSphere::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "sphere" + QString::number(mCount);
    mSphere = new SphereNode();
    mSphere->setName(name.toStdString());
    mSphere->setPosition(geoPos);

    mapItem()->getMapObject()->addNodeToLayer(mSphere, mSphereLayer);
    mSphereProperties->setSphere(mSphere);

    setState(State::DRAWING);
    mCount++;
}

void DrawSphere::cancelDraw()
{
    if(state() == State::DRAWING){
        mapItem()->getMapObject()->removeNodeFromLayer(mSphere, mSphereLayer);
        mSphere = nullptr;
        mSphereProperties->setSphere(mSphere);
        setState(State::READY);
        mCount--;
    }
}
