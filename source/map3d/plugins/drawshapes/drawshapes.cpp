#include "drawshapes.h"
#include "spherenodeeditor.h"

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
const QString IMAGE_OVERLAY = "ImageOverlay";

DrawShapes::DrawShapes(QWidget *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(drawShapes);
}
bool DrawShapes::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{

    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{IMAGE_OVERLAY, CATEGORY, "qrc:/resources/image.png", true});
    return true;
}

void DrawShapes::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if(CATEGORY == category)
    {
        mEnterShapeZone = checked;
        if(name == IMAGE_OVERLAY)
        {
            if(checked)
            {
                mShape = Shape::IMGOVLY;
            }
            else
            {
                mShape = Shape::NONE;
                mDrawingState = DrawingState::NONE;
                if (mImageOverlay && mDrawingState != DrawingState::FINISH){
                    mMapController->removeNodeFromLayer(mImageOverlay, DRAW_LAYER_NAME);
                    mMapController->removeNodeFromLayer(mImgOvlEditor, DRAW_LAYER_NAME);
                }
            }
        }
    }
}

bool DrawShapes::setup(MapController *mapController,
                       UIHandle */*UIHandle*/)
{
    mMapController = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *drawShapeLayer = new osgEarth::ModelLayer();
    drawShapeLayer->setName(DRAW_LAYER_NAME);
    mMapController->addLayer(drawShapeLayer);
    return true;
}

void DrawShapes::mousePressEvent(QMouseEvent *event)
{
    if(!mEnterShapeZone)
        return;

    switch (mShape) {
    case Shape::NONE:
        break;
    case Shape::IMGOVLY:
        onImgOvlyBtnClick(event);
        break;
    default:
        break;
    }
}


void DrawShapes::mouseDoubleClickEvent(QMouseEvent */*event*/)
{
    if(!mEnterShapeZone)
        return;
    switch (mShape) {
    case Shape::NONE:
        break;
    case Shape::IMGOVLY:
        break;
    default:
        break;
    }
}
void DrawShapes::onImgOvlyBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::LeftButton && !(mDrawingState==DrawingState::START))
    {
        osg::Image* image = osgDB::readImageFile("../data/ImgOverLay/usa_flag.png");
        if (image)
        {
            mDrawingState = DrawingState::START;
            mImageOverlay = new osgEarth::Annotation::ImageOverlay(mMapController->getMapNode(), image);
            //imageOverlay->setBounds(osgEarth::Bounds(-100.0, 35.0, -90.0, 40.0));
            mImageOverlay->setCenter(geoPos.x(),geoPos.y());
            mMapController->addNodeToLayer(mImageOverlay, DRAW_LAYER_NAME);
            //mMapController->addNode(mImageOverlay);
            mImgOvlEditor = new osgEarth::Annotation::ImageOverlayEditor(mImageOverlay, false);
            //mMapController->getMapNode()->addChild(mImgOvlEditor);
            mMapController->addNodeToLayer(mImgOvlEditor, DRAW_LAYER_NAME);


        }
    }

    if(event->button() == Qt::MouseButton::RightButton && mDrawingState==DrawingState::START)
    {
        mDrawingState = DrawingState::DELETE;
        mMapController->removeNodeFromLayer(mImageOverlay, DRAW_LAYER_NAME);
        //mMapController->removeNode(mImageOverlay);
        mMapController->removeNodeFromLayer(mImgOvlEditor, DRAW_LAYER_NAME);
        //mMapController->removeNode(mImgOvlEditor);
        mImageOverlay = nullptr;
        mImgOvlEditor = nullptr;
    }

    if(event->button() == Qt::MouseButton::MidButton && mDrawingState==DrawingState::START)
    {
        mDrawingState = DrawingState::FINISH;
        mMapController->removeNodeFromLayer(mImgOvlEditor, DRAW_LAYER_NAME);
        //mMapController->removeNode(mImgOvlEditor);
        mImageOverlay = nullptr;
        mImgOvlEditor = nullptr;
    }
}
