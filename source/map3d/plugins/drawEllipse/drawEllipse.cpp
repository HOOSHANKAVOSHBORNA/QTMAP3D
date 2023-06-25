#include "drawEllipse.h"

int DrawEllipse::mCount{0};
DrawEllipse::DrawEllipse(QObject *parent): DrawShape(parent)
{
    qmlRegisterType<EllipsePropertiesModel>("Crystal", 1, 0, "EllipseProperties");
}

bool DrawEllipse::setup()
{
    auto toolboxItem =  new ToolboxItem{ELLIPSE, CATEGORY, "qrc:/resources/ellipse.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawEllipse::onEllipseItemCheck);
    toolbox()->addItem(toolboxItem);

    makeIconNode("../data/images/draw/ellipse.png");
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mEllipseLayer = new osgEarth::Annotation::AnnotationLayer();
    mEllipseLayer->setName(ELLIPSE);

    return true;
}

void DrawEllipse::onEllipseItemCheck(bool check)
{
    if (check) {
        mapItem()->addLayerToLayer(mEllipseLayer, CATEGORY);
        setState(State::READY);
        mEllipseProperties = new EllipseProperties(mEllipse, qmlEngine(), uiHandle(), mapItem());
        mEllipseProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(mEllipseLayer->getGroup()->getNumChildren() <= 0)
            mapItem()->removeLayerFromLayer(mEllipseLayer, CATEGORY);
        if(state() == State::EDIT)
            cancelDraw();
        setState(State::NONE);
        mEllipse = nullptr;
        mEllipseProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawEllipse::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name = "Ellipse" + QString::number(mCount);
    mEllipse = new Ellipse();
    mEllipse->setName(name.toStdString());

    mEllipse->setPosition(geoPos);

    mapItem()->addNodeToLayer(mEllipse, ELLIPSE);
    mEllipseProperties->setEllipse(mEllipse);

    setState(State::EDIT);
    mCount++;
}

void DrawEllipse::cancelDraw()
{
    if(state() == State::EDIT){
        mapItem()->removeNodeFromLayer(mEllipse, ELLIPSE);
        mEllipse = nullptr;
        mEllipseProperties->setEllipse(mEllipse);
        setState(State::READY);
        mCount--;
    }
}
