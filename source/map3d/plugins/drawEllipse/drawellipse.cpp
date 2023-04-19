#include "drawellipse.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString ELLIPSE = "Ellipse";

DrawEllipse::DrawEllipse(QObject *parent)
    : PluginInterface(parent)
{

}

bool DrawEllipse::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{ELLIPSE, CATEGORY, "qrc:/resources/ellipse.png", true,  false, ""});
    return true;
}

void DrawEllipse::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == ELLIPSE) {
            if (checked) {
                mEnterEllipseZone = true;
                mDrawingState = DrawingState::START;
                addNodeToLayer(mIconNode);



            }
            else {
                mEnterEllipseZone = false;
                mDrawingState = DrawingState::FINISH;
                mEllipse = nullptr;
                removeNodeFromLayer(mIconNode);
            }
        }
    }
}

bool DrawEllipse::setup(MapController *mapController, UIHandle *uIHandle)
{
    mUiHandle = uIHandle;
    mMapController = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());
    mIconNode = makeIconNode();
    osgEarth::ModelLayer *ellipseLayer = new osgEarth::ModelLayer();
    ellipseLayer->setName(DRAW_LAYER_NAME);
    mMapController->addLayer(ellipseLayer);
    return true;
}
void DrawEllipse::mousePressEvent(QMouseEvent *event)
{
    if (mEnterEllipseZone){
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

void DrawEllipse::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterEllipseZone){
        osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
    }
}

osgEarth::Annotation::PlaceNode *DrawEllipse::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/ellipse.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}

void DrawEllipse::startDraw(QMouseEvent *event)
{
    mEllipse = new Ellipse(mMapController, true);
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    mEllipse->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y()));

    addNodeToLayer(mEllipse);
}

void DrawEllipse::cancelDraw()
{
    removeNodeFromLayer(mEllipse);
    mDrawingState = DrawingState::START;
}

void DrawEllipse::finishDraw()
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
    }
}

bool DrawEllipse::addNodeToLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
}
void DrawEllipse::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}
