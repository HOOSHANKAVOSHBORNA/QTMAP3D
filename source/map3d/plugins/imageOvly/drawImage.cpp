#include "drawImage.h"
#include "imageProperties.h"
#include <QFileDialog>


#include <osgEarthSymbology/Style>
#include <osgEarth/ModelLayer>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/feature_wfs/WFSFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <osgEarth/GLUtils>
#include <osgViewer/Viewer>
#include <osgEarth/GLUtils>
#include <osg/ShapeDrawable>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "mapItem.h"
#include "osg/Group"
#include "osgEarth/ModelLayer"
#include "osgEarth/Layer"
#include <QQuickItem>
#include <osg/Image>


//using namespace osgEarth::Annotation;

const QString CATEGORY = "Draw";
const QString IMAGE_OVERLAY = "Add Image Overlay";

DrawImage::DrawImage(QObject *parent): PluginInterface(parent)
{



}

bool DrawImage::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{

    mQmlEngine = engine;
    qmlRegisterType<ImagePropertiesModel>("Crystal", 1, 0, "ImageProperties");
    desc->toolboxItemsList.push_back(new ItemDesc{IMAGE_OVERLAY, CATEGORY, "qrc:/resources/image.png", true,  false, ""});
    return true;
}


bool DrawImage::setup(MapItem *mapItem, UIHandle *uIHandle)
{
    mUiHandle = uIHandle;
    mMapcontroller = mapItem;
    mIconNode = makeIconNode();
    osgEarth::GLUtils::setGlobalDefaults(mMapcontroller->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *drawImageLayer = new osgEarth::ModelLayer();
    drawImageLayer->setName(DRAW_LAYER_NAME);
    mMapcontroller->addLayer(drawImageLayer);
    return true;
}


void DrawImage::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == IMAGE_OVERLAY) {
            if (checked) {
                mEnterImageZone = true;
                mDrawingState = DrawingState::START;
                loadImage();
                mImageProperties = new ImageProperties(mImageOverlay, mQmlEngine, mUiHandle, mMapcontroller);
                mImageProperties->show();
                mMapcontroller->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

            }
            else {
                mEnterImageZone = false;
                mDrawingState = DrawingState::FINISH;
//                mMapcontroller->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);
                mMapcontroller->removeNodeFromLayer(mImageOverlay, DRAW_LAYER_NAME);
//                mMapcontroller->removeNodeFromLayer(mImgOvlEditor, DRAW_LAYER_NAME);
                mImageProperties->hide();
                mMapcontroller->removeNodeFromLayer(mIconNode,DRAW_LAYER_NAME);
            }
        }
    }
}

void DrawImage::mousePressEvent(QMouseEvent *event)
{


    if (mEnterImageZone) {
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
        else if (event->button() == Qt::MouseButton::MiddleButton && mDrawingState == DrawingState::DRAWING) {
            finishDrawing(event);
        }
    }
}

void DrawImage::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterImageZone) {
        osgEarth::GeoPoint geoPos = mMapcontroller->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
    }
}


void DrawImage::loadImage()
{
    QUrl fileDialog = QFileDialog::getOpenFileUrl();
    imageAddr = osgDB::readImageFile(fileDialog.toLocalFile().toStdString());
}

void DrawImage::startDraw(QMouseEvent *event)
{

    osg::Vec3d worldPos;

    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);

    if (imageAddr)
    {
        mDrawingState = DrawingState::START;
        mImageOverlay = new osgEarth::Annotation::ImageOverlay(mMapcontroller->getMapNode(), imageAddr);
        mImageOverlay->setCenter(geoPos.x(),geoPos.y());
        mImageProperties->setImage(mImageOverlay);
        mImageProperties->setLocation(mImageOverlay->getCenter());
        mImageProperties->setTL(mImageOverlay->getUpperLeft());
        mImageProperties->setTR(mImageOverlay->getUpperRight());
        mImageProperties->setBR(mImageOverlay->getLowerRight());
        mImageProperties->setBL(mImageOverlay->getLowerLeft());
        mMapcontroller->addNodeToLayer(mImageOverlay, DRAW_LAYER_NAME);
//        mImgOvlEditor = new osgEarth::Annotation::ImageOverlayEditor(mImageOverlay, false);
//        mMapcontroller->addNodeToLayer(mImgOvlEditor, DRAW_LAYER_NAME);

    }
}

void DrawImage::finishDrawing(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        event->accept();
        mImageProperties->setImage(mImageOverlay);
//        mMapcontroller->removeNodeFromLayer(mImageOverlay, DRAW_LAYER_NAME);
//        mMapcontroller->removeNodeFromLayer(mImgOvlEditor, DRAW_LAYER_NAME);
        mImageOverlay = nullptr;


    }
}

void DrawImage::cancelDrawing(QMouseEvent *event)
{

//    mMapcontroller->addNodeToLayer(mImage, DRAW_LAYER_NAME);
    mImageProperties->setImage(mImageOverlay);
    mDrawingState = DrawingState::START;
    mMapcontroller->removeNodeFromLayer(mImageOverlay , DRAW_LAYER_NAME);
//    mMapcontroller->removeNodeFromLayer(mImgOvlEditor, DRAW_LAYER_NAME);
    mImageOverlay = nullptr;
//    mImgOvlEditor = nullptr;

    event->accept();
}

osgEarth::Annotation::PlaceNode *DrawImage::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/image.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}









