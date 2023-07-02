#include "drawShape.h"

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
#include "osgEarth/Layer"
#include <QQuickItem>
#include <osgEarthAnnotation/AnnotationLayer>

using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;

DrawShape::DrawShape(QObject *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(drawShape);
}

DrawShape::~DrawShape()
{
    mIconNode.release();
//    qDebug()<<name();
//    if(mIconNode.valid()){
//    qDebug()<<mIconNode->referenceCount();
//    qDebug()<<mIconNode->referenceCount();
//    }
}

bool DrawShape::setup()
{
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mShapeLayer = new osgEarth::Annotation::AnnotationLayer();
    mShapeLayer->setName(CATEGORY);
    mapItem()->getMapObject()->addLayer(mShapeLayer);

    mMeasureLayer = new osgEarth::Annotation::AnnotationLayer();
    mMeasureLayer->setName(M_CATEGORY);
    mapItem()->getMapObject()->addLayer(mMeasureLayer);


    return true;
}

void DrawShape::makeIconNode(const QString &fileName)
{
    osg::Image* icon = osgDB::readImageFile(fileName.toStdString());
    if(icon){
        icon->scaleImage(24, 24, icon->r());
        mIconNode = new osgEarth::Annotation::PlaceNode();
        mIconNode->setIconImage(icon);
    }
}

osgEarth::Annotation::PlaceNode *DrawShape::iconNode() const
{
    return mIconNode.get();
}

DrawShape::State DrawShape::state() const
{
    return mState;
}

void DrawShape::setState(DrawShape::State newState)
{
    mState = newState;
}

//void DrawShape::addLayer()
//{
//    if(!mHasLayer){
//        auto shapeLayer = new osgEarth::Annotation::AnnotationLayer();
//        shapeLayer->setName(CATEGORY);
//        mapItem()->addLayer(shapeLayer);
//        mHasLayer = true;
//    }
//}

osgEarth::Annotation::AnnotationLayer *DrawShape::shapeLayer()
{
    if(!mShapeLayer)
        mShapeLayer = dynamic_cast<osgEarth::Annotation::AnnotationLayer*>(mapItem()->getMapObject()->getLayerByName(CATEGORY));
    return mShapeLayer;
}

osgEarth::Annotation::AnnotationLayer *DrawShape::measureLayer()
{
    if(!mMeasureLayer)
        mMeasureLayer = dynamic_cast<osgEarth::Annotation::AnnotationLayer*>(mapItem()->getMapObject()->getLayerByName(M_CATEGORY));
    return mMeasureLayer;
}

bool DrawShape::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;

    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        if (mState == State::READY) {
           osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            initDraw(geoPos);

            return true;
        }

        if (mState == State::DRAWING) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            drawing(geoPos);
            return true;
        }

    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::EDIT || mState == State::DRAWING)) {
        cancelDraw();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::EDIT || mState == State::DRAWING)) {
        confirmDraw();
        return false;
    }
    return false;
}

bool DrawShape::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;
    //move icon-----------------------
    if(mIconNode){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }


    if(mState == State::DRAWING){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        tempDrawing(geoPos);
        return true;
    }
    //--------------------------------
    return false;


}

void DrawShape::confirmDraw()
{
    if (state() == State::EDIT || state() == State::DRAWING) {
        setState(State::READY);
    }
}

