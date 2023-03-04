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
#include <osgEarth/GeoMath>

using namespace osgEarth::Annotation;

const QString CATEGORY = "Draw";
const QString LINESTRING = "Line String";
const QString LINE = "Line";


drawLine::drawLine(QWidget *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(drawLine);
//    Q_INIT_RESOURCE(LineProperties);

}

bool drawLine::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<LinePropertiesModel>("Crystal", 1, 0, "LineProperties");
    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{LINESTRING, CATEGORY, "qrc:/resources/line_string.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{LINE, CATEGORY, "qrc:/resources/line.png", true});
    return true;
}

void drawLine::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if(CATEGORY == category)
        if(name == LINESTRING)
        {
            if(checked)
            {
                mShape = Shape::LINESTRING;
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
            event->accept();
        }
        if(mDrawingState == DrawingState::DRAWING)
        {
            if (mShape == Shape::LINE && mLine->getSize()>= 2){
                finishDrawing(event);
            }
            else
                drawingLine(event);
            event->accept();
        }
    }
    if(event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::DRAWING)
    {
        cancelDrawingLine(event);
        if (mLineProperties)
            mLineProperties->hide();
    }
    if(event->button() == Qt::MouseButton::MidButton && mDrawingState == DrawingState::DRAWING)
    {
        finishDrawing(event);
        event->accept();
    }
}

void drawLine::mouseMoveEvent(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::DRAWING)
        mouseMoveDrawing(event);

}

void drawLine::mouseDoubleClickEvent(QMouseEvent */*event*/)
{
//    finishDrawing(event);
//    event->accept();
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
    mLine->showLenght(true);
    addNodeToLayer(mLine);

    if (mLineProperties)
        mLineProperties->hide();
    mLineProperties = new LineProperties(mQmlEngine, mLine);
    mLineProperties->show();

    mDrawingState = DrawingState::DRAWING;
}

void drawLine::drawingLine(QMouseEvent *event)
{
    osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());
    mLine->addPoint(geoPos);
}

void drawLine::cancelDrawingLine(QMouseEvent *event)
{
    removeNodeFromLayer(mLine);
    if (mLineProperties)
        mLineProperties->hide();
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
            removeNodeFromLayer(nodeEditor);
        event->accept();
//        if (mLineProperties)
//            mLineProperties->hide();
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

