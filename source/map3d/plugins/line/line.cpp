#include "line.h"
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
const QString LINE = "Line";
const QString LINE_II = "Line II";


Line::Line(QWidget *parent)
    : PluginInterface(parent)
{
//    Q_INIT_RESOURCE(line);
}

bool Line::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mAnnoLayer = new osgEarth::Annotation::AnnotationLayer;
    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{LINE, CATEGORY, "qrc:/resources/line.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{LINE_II, CATEGORY, "qrc:/resources/line.png", true});
    return true;
}

void Line::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if(CATEGORY == category)
        if(name == LINE)
        {
            if(checked)
            {
                mShape = Shape::LINE;
                //mLine = new LineNode(mMapController);
                //mLineCircleGr = new osg::Group;
                //mMapController->addNode(mLineCircleGr);
            }
            else
            {
                mShape = Shape::NONE;
                mDrawingState = DrawingState::NONE;
            }
        }
}

bool Line::setup(MapController *mapController, UIHandle *UIHandle)
{
    mMapController = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *lineLayer = new osgEarth::ModelLayer();
    lineLayer->setName(DRAW_LAYER_NAME);
    mMapController->addLayer(lineLayer);
    return true;
}

void Line::mousePressEvent(QMouseEvent *event)
{
    switch (mShape) {
    case Shape::NONE:
        break;
    case Shape::LINE:
        onLineBtnClick(event);
        break;
    case Shape::LINE_II:
        onLineIIBtnClick(event);
        break;
    }
}

void Line::onLineBtnClick(QMouseEvent *event)
{

}

void Line::onLineIIBtnClick(QMouseEvent *event)
{

}
