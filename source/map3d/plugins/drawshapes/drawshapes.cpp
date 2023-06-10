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
#include "mapItem.h"
#include "osgEarth/ModelLayer"
#include "osgEarth/Layer"
#include <QQuickItem>
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
                    mapItem()->removeNodeFromLayer(mImageOverlay, DRAW_LAYER_NAME);
                    mapItem()->removeNodeFromLayer(mImgOvlEditor, DRAW_LAYER_NAME);
                }
            }
        }
    }
}

bool DrawShapes::setup()
{
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *drawShapeLayer = new osgEarth::ModelLayer();
    drawShapeLayer->setName(DRAW_LAYER_NAME);
    mapItem()->addLayer(drawShapeLayer);
    return true;
}

bool DrawShapes::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(!mEnterShapeZone)
        return false;

    switch (mShape) {
    case Shape::NONE:
        break;
    case Shape::IMGOVLY:
//        onImgOvlyBtnClick(ea);
        break;
    default:
        break;
    }
    return false;
}


bool DrawShapes::mouseDoubleClickEvent(const osgGA::GUIEventAdapter &/*event*/, osgGA::GUIActionAdapter &aa)
{
    if(!mEnterShapeZone)
        return false;
    switch (mShape) {
    case Shape::NONE:
        break;
    case Shape::IMGOVLY:
        break;
    default:
        break;
    }
    return false;
}
bool DrawShapes::onImgOvlyBtnClick(const osgGA::GUIEventAdapter &event)
{
    osg::Vec3d worldPos;
    mapItem()->screenToWorld(event.getX(), event.getY(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mapItem()->getMapSRS(), worldPos);

    if(event.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON && !(mDrawingState==DrawingState::START))
    {
        osg::Image* image = osgDB::readImageFile("../data/ImgOverLay/usa_flag.png");
        if (image)
        {
            mDrawingState = DrawingState::START;
            mImageOverlay = new osgEarth::Annotation::ImageOverlay(mapItem()->getMapNode(), image);
            //imageOverlay->setBounds(osgEarth::Bounds(-100.0, 35.0, -90.0, 40.0));
            mImageOverlay->setCenter(geoPos.x(),geoPos.y());
            mapItem()->addNodeToLayer(mImageOverlay, DRAW_LAYER_NAME);
            //mapItem()->addNode(mImageOverlay);
            mImgOvlEditor = new osgEarth::Annotation::ImageOverlayEditor(mImageOverlay, false);
            //mapItem()->getMapNode()->addChild(mImgOvlEditor);
            mapItem()->addNodeToLayer(mImgOvlEditor, DRAW_LAYER_NAME);

            return true;
        }
        return false;
    }

    if(event.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON && mDrawingState==DrawingState::START)
    {
        mDrawingState = DrawingState::DELETE;
        mapItem()->removeNodeFromLayer(mImageOverlay, DRAW_LAYER_NAME);
        //mapItem()->removeNode(mImageOverlay);
        mapItem()->removeNodeFromLayer(mImgOvlEditor, DRAW_LAYER_NAME);
        //mapItem()->removeNode(mImgOvlEditor);
        mImageOverlay = nullptr;
        mImgOvlEditor = nullptr;
    }

    if(event.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON && mDrawingState==DrawingState::START)
    {
        mDrawingState = DrawingState::FINISH;
        mapItem()->removeNodeFromLayer(mImgOvlEditor, DRAW_LAYER_NAME);
        //mapItem()->removeNode(mImgOvlEditor);
        mImageOverlay = nullptr;
        mImgOvlEditor = nullptr;
    }
    return false;
}
