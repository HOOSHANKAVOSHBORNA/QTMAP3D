#include "drawline.h"
#include <osgEarthSymbology/Style>
#include <osgEarth/ModelLayer>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/feature_wfs/WFSFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <osgEarth/GLUtils>
#include <osgViewer/Viewer>
#include <osgEarth/GLUtils>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "mapcontroller.h"
#include "osg/Group"
#include "osgEarth/ModelLayer"
#include "osgEarth/Layer"
#include <QQuickItem>
#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/AnnotationLayer>

using namespace osgEarth::Annotation;

const QString CATEGORY = "Draw";
const QString LINESTRIP = "Line Strip";
const QString LINE = "Line";


drawLine::drawLine(QWidget *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(drawLine);
}

bool drawLine::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mAnnoLayer = new osgEarth::Annotation::AnnotationLayer;
    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{LINESTRIP, CATEGORY, "qrc:/resources/line_string.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{LINE, CATEGORY, "qrc:/resources/line.png", true});
    return true;
}

void drawLine::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if(CATEGORY == category)
        if(name == LINESTRIP)
        {
            if(checked)
            {
                mShape = Shape::LINESTRIP;
            }
            else
            {
                mShape = Shape::NONE;
                mDrawingState = DrawingState::NONE;
            }
        }

    if(name == LINE)
    {
        if(checked)
        {
            mShape = Shape::LINE;

        }
        else
        {
            mShape = Shape::NONE;
            mDrawingState = DrawingState::NONE;
        }
    }
}

bool drawLine::setup(MapController *mapController, UIHandle *UIHandle)
{
    mMapController = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *lineLayer = new osgEarth::ModelLayer();
    lineLayer->setName(DRAW_LAYER_NAME);
    mMapController->addLayer(lineLayer);
    return true;
}

void drawLine::mousePressEvent(QMouseEvent *event)
{
    switch (mShape) {
    case Shape::NONE:
        break;
    case Shape::LINESTRIP:
        onLineStripBtnClick(event);
        break;
    case Shape::LINE:
        onLineBtnClick(event);
        break;
    }
}

void drawLine::mouseMoveEvent(QMouseEvent *event)
{
    if(mShape == Shape::LINESTRIP || mShape == Shape::LINE){
        onLineMouseMove(event);
    }
}

void drawLine::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(mShape == Shape::LINESTRIP){
        onNodeBtnDoubleClick(event);
    }
}

bool drawLine::addNodeToLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
}

void drawLine::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}

void drawLine::onLineStripBtnClick(QMouseEvent *event)
{

    if(event->button() == Qt::MouseButton::LeftButton)
    {
        osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());

        mShape = Shape::LINESTRIP;

        if(mDrawingState != DrawingState::START)
        {
            mDrawingState = DrawingState::START;
            mLine = new LineNode(mMapController);
            mLine->setColor(osgEarth::Color::Purple);
            mLine->setPointColor(osgEarth::Color::Yellow);
            mLine->setWidth(7);
            mLine->setPointVisible(true);
            mLine->setPointWidth(8);
            addNodeToLayer(mLine);

        }

        mLine->addPoint(geoPos);
        event->accept();

    }
    if(event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::START)
    {
        mDrawingState = DrawingState::DELETE;
        removeNodeFromLayer(mLine);
        event->accept();
    }
    if(event->button() == Qt::MouseButton::MiddleButton && mDrawingState == DrawingState::START)
    {
        mLine->setPointVisible(false);
        mLine->setHeight(1000000);
        mLine->setTessellation(100);
    }

}

void drawLine::onLineBtnClick(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());

        mShape = Shape::LINE;

        if(mDrawingState != DrawingState::START)
        {
            mDrawingState = DrawingState::START;
            mLine = new LineNode(mMapController);
            mLine->setColor(osgEarth::Color::Orange);
            mLine->setPointColor(osgEarth::Color::Black);
            mLine->setWidth(7);
            mLine->setPointVisible(true);
            mLine->setPointWidth(8);
            addNodeToLayer(mLine);

        }
        if (mLine->getSize()<2){
            mLine->addPoint(geoPos);
        }

        if (mLine->getSize()==2){mLine->addPoint(geoPos);
            mDrawingState = DrawingState::FINISH;
            mLine->clear();

        }
        event->accept();

    }
    if(event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::START)
    {
        mDrawingState = DrawingState::DELETE;
        removeNodeFromLayer(mLine);
        event->accept();
    }

}

void drawLine::onLineMouseMove(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::START)
    {

        if (mLine->getSize() >= 2)
        {
            mLine->removePoint();
        }
        osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());

            mLine->addPoint(geoPos);


    }
}

void drawLine::onNodeBtnDoubleClick(QMouseEvent *event, osg::Node *nodeEditor)
{
    if(mDrawingState == DrawingState::START)
    {
        mDrawingState = DrawingState::FINISH;
        if(nodeEditor)
            //mMapController->removeNode(nodeEditor);
            removeNodeFromLayer(nodeEditor);
            //mMapController->removeNode(mPolyHdragger);
        event->accept();
    }
}
