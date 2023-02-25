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
                mDrawingState = DrawingState::START;
            }
            else
            {
                mShape = Shape::NONE;
                mDrawingState = DrawingState::FINISH;
            }
        }

    if(name == LINE)
    {
        if(checked)
        {
            mShape = Shape::LINE;
            mDrawingState = DrawingState::START;
        }
        else
        {
            mShape = Shape::NONE;
            mDrawingState = DrawingState::FINISH;
        }
    }
}

bool drawLine::setup(MapController *mapController, UIHandle */*UIHandle*/)
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
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        if(mDrawingState == DrawingState::START)
        {
            startDrawLine();
        }
        if(mDrawingState == DrawingState::DRAWING)
        {
            drawingLine(event);
        }
        event->accept();

    }
    if(event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::DRAWING)
    {
        cancelDrawingLine(event);
    }
}

void drawLine::mouseMoveEvent(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::DRAWING)
        mouseMoveDrawing(event);
}

void drawLine::mouseDoubleClickEvent(QMouseEvent *event)
{
    finishDrawing(event);
}

void drawLine::startDrawLine()
{
    mLine = new LineNode(mMapController);
    mLine->setColor(osgEarth::Color::Orange);
    mLine->setPointColor(osgEarth::Color::Black);
    mLine->setWidth(7);
    mLine->setPointVisible(false);
    mLine->setPointWidth(8);
    mLine->setTessellation(20);
    addNodeToLayer(mLine);

    mDrawingState = DrawingState::DRAWING;
}

void drawLine::drawingLine(QMouseEvent *event)
{
    osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());
    mLine->addPoint(geoPos);
    if (mShape == Shape::LINE && mLine->getSize()>= 2){
        finishDrawing(event);
    }
}

void drawLine::cancelDrawingLine(QMouseEvent *event)
{
    removeNodeFromLayer(mLine);
    event->accept();

    mDrawingState = DrawingState::START;
}

void drawLine::mouseMoveDrawing(QMouseEvent *event)
{
    if (mLine->getSize() >= 2)
    {
        mLine->removePoint();
    }
    osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());
    mLine->addPoint(geoPos);
}

void drawLine::finishDrawing(QMouseEvent *event, osg::Node *nodeEditor)
{
    if(mDrawingState == DrawingState::DRAWING)
    {
        mDrawingState = DrawingState::START;
        if(nodeEditor)
            //mMapController->removeNode(nodeEditor);
            removeNodeFromLayer(nodeEditor);
        //mMapController->removeNode(mPolyHdragger);
        event->accept();
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
