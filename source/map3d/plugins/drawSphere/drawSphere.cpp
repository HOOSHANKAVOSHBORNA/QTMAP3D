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
        mapItem()->addLayerToLayer(mSphereLayer, CATEGORY);
        setState(State::READY);
        mSphereProperties = new SphereProperties(qmlEngine(), uiHandle(), mapItem());
        mSphereProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(mSphereLayer->getGroup()->getNumChildren() <= 0)
            mapItem()->removeLayerFromLayer(mSphereLayer, CATEGORY);
        if(state() == State::EDIT)
            cancelDraw();
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

    mapItem()->addNodeToLayer(mSphere, SPHERE);
    mSphereProperties->setSphere(mSphere);

    setState(State::EDIT);
    mCount++;
}

void DrawSphere::cancelDraw()
{
    if(state() == State::EDIT){
        mapItem()->removeNodeFromLayer(mSphere, SPHERE);
        mSphere = nullptr;
        mSphereProperties->setSphere(mSphere);
        setState(State::READY);
        mCount--;
    }
}
