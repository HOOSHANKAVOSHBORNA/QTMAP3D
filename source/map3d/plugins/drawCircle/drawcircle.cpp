#include "drawcircle.h"

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString CIRCLE = "Circletest";

DrawCircle::DrawCircle(QObject *parent)
    : PluginInterface(parent)
{
    //    Q_INIT_RESOURCE(DrawCircle);
}

bool DrawCircle::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{CIRCLE, CATEGORY, "qrc:/resources/circle.png", true});
    return true;
}

void DrawCircle::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == CIRCLE) {
            if (checked) {
                mEnterCircleZone = true;
                mDrawingState = DrawingState::START;
            }
            else {
                mEnterCircleZone = false;
                mDrawingState = DrawingState::FINISH;
            }
        }
    }
}

bool DrawCircle::setup(MapController *mapController, UIHandle *uIHandle)
{
    mUiHandle = uIHandle;
    mMapcontroller = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapcontroller->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *circleLayer = new osgEarth::ModelLayer();
    circleLayer->setName(DRAW_LAYER_NAME);
    mMapcontroller->addLayer(circleLayer);

    return true;
}

void DrawCircle::mousePressEvent(QMouseEvent *event)
{
    if (mEnterCircleZone) {
        if (event->button() == Qt::MouseButton::LeftButton) {
            if (mDrawingState == DrawingState::START) {
                startDraw(event);
                event->accept();
            }
        }
        else if (event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::START) {
            cancelDrawing(event);
        }
        else if (event->button() == Qt::MouseButton::MidButton && mDrawingState == DrawingState::START) {
            finishDrawing(event);
            event->accept();
        }
    }
}

void DrawCircle::startDraw(QMouseEvent *event)
{
    mCircle = new CircleNode(mMapcontroller, true);
    mCircle->setArcStart(0);
    mCircle->setArcEnd(360);

    mCircle->setColor(osgEarth::Color("red"));
    mCircle->setRadius(100000);
    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);

    mCircle->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    addNodeToLayer(mCircle);
    event->accept();
}

void DrawCircle::cancelDrawing(QMouseEvent *event)
{
    removeNodeFromLayer(mCircle);
    event->accept();
}

void DrawCircle::finishDrawing(QMouseEvent *event, osg::Node *nodeEditor)
{
    if (mDrawingState == DrawingState::START) {
        event->accept();
    }
}

bool DrawCircle::addNodeToLayer(osg::Node *node)
{
    auto layer = mMapcontroller->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
}

void DrawCircle::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapcontroller->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}
