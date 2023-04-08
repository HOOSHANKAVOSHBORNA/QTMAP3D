#include "drawcircle.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString CIRCLE = "Circle";

DrawCircle::DrawCircle(QObject *parent)
    : PluginInterface(parent)
{

}

bool DrawCircle::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<CirclePropertiesModel>("Crystal", 1, 0, "CircleProperties");
    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{CIRCLE, CATEGORY, "qrc:/resources/circle.png", true,  false, ""});

    return true;
}

void DrawCircle::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == CIRCLE) {
            if (checked) {
                mEnterCircleZone = true;
                mCircleProperties = new CircleProperties(mCircle, mQmlEngine, mUiHandle, mMapcontroller);
                mCircleProperties->show();
                addNodeToLayer(mIconNode);
                mDrawingState = DrawingState::START;

            }
            else {
                mEnterCircleZone = false;
                mDrawingState = DrawingState::FINISH;
                mCircle = nullptr;
                removeNodeFromLayer(mIconNode);
                mCircleProperties->hide();
            }
        }
    }
}

bool DrawCircle::setup(MapController *mapController, UIHandle *uIHandle)
{
    mUiHandle = uIHandle;
    mMapcontroller = mapController;
    mIconNode = makeIconNode();
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
                finishDrawing(event);
                event->accept();
            }
        }
        else if (event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::START) {
            cancelDrawing(event);
        }
        else if (event->button() == Qt::MouseButton::MidButton && mDrawingState == DrawingState::DRAWING) {
            finishDrawing(event);
        }
    }
}

void DrawCircle::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterCircleZone) {
        osgEarth::GeoPoint geoPos = mMapcontroller->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
    }
}

osgEarth::Annotation::PlaceNode *DrawCircle::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/circle.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}

void DrawCircle::startDraw(QMouseEvent *event)
{
    mCircle = new Circle(mMapcontroller, true);
    mCircleProperties->setCircle(mCircle);
//    mCircleHdragger = new osgEarth::Annotation::SphereDragger(mMapcontroller->getMapNode());
//    mCircle->setArcStart(0);
//    mCircle->setArcEnd(360);

//    mCircle->setColor(osgEarth::Color(1, 0, 1, 0.7f));
//    mCircle->setRadius(500000);
    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);

    mCircle->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
//    mCircleHdragger->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
//    addNodeToLayer(mCircleHdragger);
//    mCircleHdragger->Dragger::setDefaultDragMode(osgEarth::Annotation::Dragger::DragMode::DRAGMODE_VERTICAL);

    mCircleProperties->setLocation(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    addNodeToLayer(mCircle);
    event->accept();
}

void DrawCircle::cancelDrawing(QMouseEvent *event)
{
    removeNodeFromLayer(mCircle);
    mCircle = nullptr;
    mCircleProperties->setCircle(mCircle);
    mDrawingState = DrawingState::START;
//    removeNodeFromLayer(mCircleHdragger);
//    mCircleHdragger = nullptr;
//    mCircleHdragger = new osgEarth::Annotation::SphereDragger(mMapcontroller->getMapNode());

    event->accept();
}

void DrawCircle::finishDrawing(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::DRAWING) {
//        mCircle = nullptr;
//        mCircleProperties->setCircle(mCircle);

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
