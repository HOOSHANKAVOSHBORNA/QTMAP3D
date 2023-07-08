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
        if(mEllipseLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->addLayer(mEllipseLayer, shapeLayer);
        }
        setState(State::READY);
        mEllipseProperties = new EllipseProperties(mEllipse, qmlEngine(), uiHandle(), mapItem());
        mEllipseProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mEllipseLayer->getGroup()->getNumChildren() <= 0){
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->removeLayer(mEllipseLayer, shapeLayer);
        }
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

    mapItem()->getMapObject()->addNodeToLayer(mEllipse, mEllipseLayer);
    mEllipseProperties->setEllipse(mEllipse);

    setState(State::DRAWING);
    mCount++;
}

void DrawEllipse::cancelDraw()
{
    if(state() == State::DRAWING){
        mapItem()->getMapObject()->removeNodeFromLayer(mEllipse, mEllipseLayer);
        mEllipse = nullptr;
        mEllipseProperties->setEllipse(mEllipse);
        setState(State::READY);
        mCount--;
    }
}
