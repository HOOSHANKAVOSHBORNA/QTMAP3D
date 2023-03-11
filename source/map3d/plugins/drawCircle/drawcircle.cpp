#include "drawcircle.h"

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString CIRCLE = "Circletest";

DrawCircle::DrawCircle(QObject *parent)
    : PluginInterface(parent)
{

}

bool DrawCircle::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<CirclePropertiesModel>("Crystal", 1, 0, "CircleProperties");
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
                mCircleProperties = new CircleProperties(mQmlEngine, mUiHandle);
                mCircleProperties->show();

            }
            else {
                mEnterCircleZone = false;
                mDrawingState = DrawingState::FINISH;
                mCircleProperties->hide();
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
                mDrawingState = DrawingState::DRAWING;
                startDraw(event);
                event->accept();
            }
        }
        else if (event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::DRAWING) {
            cancelDrawing(event);
        }
        else if (event->button() == Qt::MouseButton::MidButton && mDrawingState == DrawingState::DRAWING) {
            finishDrawing(event);
        }
    }
}

//void DrawCircle::mouseMoveEvent(QMouseEvent */*event*/)
//{
//    if (mCircle)
//        mCircle->setCircleHeight(static_cast<float>(mCircleHdragger->Dragger::getPosition().z()));
//}

void DrawCircle::startDraw(QMouseEvent *event)
{
    mCircle = new Circle(mMapcontroller, true);
//    mCircleHdragger = new osgEarth::Annotation::SphereDragger(mMapcontroller->getMapNode());
    mCircle->setArcStart(0);
    mCircle->setArcEnd(360);

    mCircle->setColor(osgEarth::Color(1, 0, 1, 0.7f));
    mCircle->setRadius(500000);
    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);

    mCircle->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
//    mCircleHdragger->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
//    addNodeToLayer(mCircleHdragger);
//    mCircleHdragger->Dragger::setDefaultDragMode(osgEarth::Annotation::Dragger::DragMode::DRAGMODE_VERTICAL);
    
    addNodeToLayer(mCircle);
    event->accept();
}

void DrawCircle::cancelDrawing(QMouseEvent *event)
{
    removeNodeFromLayer(mCircle);
    mCircle = nullptr;
    mDrawingState = DrawingState::START;
//    removeNodeFromLayer(mCircleHdragger);
//    mCircleHdragger = nullptr;
//    mCircleHdragger = new osgEarth::Annotation::SphereDragger(mMapcontroller->getMapNode());

    event->accept();
}

void DrawCircle::finishDrawing(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mCircle = nullptr;
//        removeNodeFromLayer(mCircleHdragger);
//        mCircleHdragger = nullptr;
//        mCircleHdragger = new osgEarth::Annotation::SphereDragger(mMapcontroller->getMapNode());
        mDrawingState = DrawingState::START;
        event->accept();
    }
}

void DrawCircle::onCircleMouseMove(QMouseEvent */*event*/)
{

    if (mCircle){
        mCircle->setCircleHeight(static_cast<float>(mCircleHdragger->Dragger::getPosition().z()));
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
