#include "drawrect.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString RECT = "Rect";

DrawRect::DrawRect(QObject *parent)
    : PluginInterface(parent)
{

}
bool DrawRect::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{RECT, CATEGORY, "qrc:/resources/rectangle.png", true,  false, ""});
    return true;
}

void DrawRect::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == RECT) {
            if (checked) {
                mEnterRectZone = true;
                mDrawingState = DrawingState::START;
                addNodeToLayer(mIconNode);
            }
            else {
                mEnterRectZone = false;
                mDrawingState = DrawingState::FINISH;
                mRect = nullptr;
                removeNodeFromLayer(mIconNode);
            }
        }
    }
}

bool DrawRect::setup(MapController *mapController, UIHandle *uIHandle)
{
    mUiHandle = uIHandle;
    mMapController = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());
    mIconNode = makeIconNode();
    osgEarth::ModelLayer *rectLayer = new osgEarth::ModelLayer();
    rectLayer->setName(DRAW_LAYER_NAME);
    mMapController->addLayer(rectLayer);
    return true;
}
void DrawRect::mousePressEvent(QMouseEvent *event)
{
    if (mEnterRectZone){
        if(event->button() == Qt::MouseButton::LeftButton)
        {
            if (mDrawingState == DrawingState::START) {
                mDrawingState = DrawingState::DRAWING;
                startDraw(event);
                finishDraw();
                event->accept();
            }
        }
        //cancel
        if(event->button() == Qt::MouseButton::RightButton)
        {
            if(mDrawingState == DrawingState::START)
            {
                cancelDraw();
                event->accept();
            }
        }
        //finish
        if(event->button() == Qt::MouseButton::MidButton)
        {
            if(mDrawingState == DrawingState::START)
            {
                finishDraw();
            }
        }
    }
}

void DrawRect::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterRectZone){
        osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
    }
}

osgEarth::Annotation::PlaceNode *DrawRect::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/rectangle.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    if (icon){
        model->setIconImage(icon);
    }
    return model.release();
}

void DrawRect::startDraw(QMouseEvent *event)
{
    mRect = new Rect(mMapController, true);
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    mRect->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y()));

    addNodeToLayer(mRect);
}

void DrawRect::cancelDraw()
{
    removeNodeFromLayer(mRect);
    mDrawingState = DrawingState::START;
}

void DrawRect::finishDraw()
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
    }
}

bool DrawRect::addNodeToLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
}
void DrawRect::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}
