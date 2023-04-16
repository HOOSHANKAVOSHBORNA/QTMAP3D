#include "drawpolygon.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString POLYGON = "Polygon";

DrawPolygon::DrawPolygon(QObject *parent)
    : PluginInterface(parent)
{

}

bool DrawPolygon::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    desc->toolboxItemsList.push_back(new ItemDesc{POLYGON, CATEGORY, "qrc:/resources/polygon.png", true,  false, ""});
    return true;
}

void DrawPolygon::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == POLYGON) {
            if (checked) {
                mEnterPolygonZone = true;
                mDrawingState = DrawingState::START;
                addNodeToLayer(mIconNode);

            }
            else {
                mEnterPolygonZone = false;
                mDrawingState = DrawingState::FINISH;
                mPolygon = nullptr;
                removeNodeFromLayer(mIconNode);
            }
        }
    }
}

bool DrawPolygon::setup(MapController *mapController, UIHandle *uIHandle)
{
    mMapController = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());
    mIconNode = makeIconNode();
    osgEarth::ModelLayer *polygonLayer = new osgEarth::ModelLayer();
    polygonLayer->setName(DRAW_LAYER_NAME);
    mMapController->addLayer(polygonLayer);
    return true;
}

void DrawPolygon::mousePressEvent(QMouseEvent *event)
{
    if (mEnterPolygonZone){
        if(event->button() == Qt::MouseButton::LeftButton)
        {
            if(mDrawingState == DrawingState::START)
            {
                startDraw(event);
                event->accept();
            }

            if(mDrawingState == DrawingState::DRAWING)
            {
                drawing(event);
                event->accept();
            }
        }
        //cancel
        if(event->button() == Qt::MouseButton::RightButton)
        {
            if(mDrawingState == DrawingState::DRAWING)
            {
                cancelDraw();
                event->accept();
            }
        }
        //finish
        if(event->button() == Qt::MouseButton::MidButton)
        {
            if(mDrawingState == DrawingState::DRAWING)
            {
                finishDraw(event);
                event->accept();
            }
        }
    }
}

void DrawPolygon::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterPolygonZone){
        osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
        if (mDrawingState == DrawingState::DRAWING){
            mouseMoveDrawing(event);
        }
    }
}

osgEarth::Annotation::PlaceNode *DrawPolygon::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/polygon.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}

void DrawPolygon::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void DrawPolygon::startDraw(QMouseEvent *event)
{
    mPolygon = new Polygon(mMapController, true);
    addNodeToLayer(mPolygon);
    mDrawingState = DrawingState::DRAWING;
}
void DrawPolygon::drawing(QMouseEvent *event)
{
    osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());
    mPolygon->addPoints(geoPos);
}

void DrawPolygon::cancelDraw()
{
    removeNodeFromLayer(mPolygon);
    mDrawingState = DrawingState::START;
}

void DrawPolygon::finishDraw(QMouseEvent *event)
{
    if(mDrawingState == DrawingState::DRAWING)
    {
        mDrawingState = DrawingState::START;
    }
}
void DrawPolygon::mouseMoveDrawing(QMouseEvent *event)
{
    if (mPolygon->getSize() >= 2)
    {
        mPolygon->removePoint();

    }
    osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());
    mPolygon->addPoints(geoPos);
}
bool DrawPolygon::addNodeToLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
}
void DrawPolygon::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}

