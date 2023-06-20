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
bool DrawShape::mHasLayer = false;
DrawShape::DrawShape(QObject *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(drawShape);
}

bool DrawShape::setup()
{
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    return true;
}

void DrawShape::makeIconNode(const QString &fileName)
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile(fileName.toStdString());
    if(icon.valid()){
        icon->scaleImage(24, 24, icon->r());
        mIconNode = new osgEarth::Annotation::PlaceNode();
        mIconNode->setIconImage(icon);
    }
}

osg::ref_ptr<osgEarth::Annotation::PlaceNode> DrawShape::iconNode() const
{
    return mIconNode;
}

DrawShape::State DrawShape::state() const
{
    return mState;
}

void DrawShape::setState(DrawShape::State newState)
{
    mState = newState;
}

void DrawShape::addLayer()
{
    if(!mHasLayer){
        auto shapeLayer = new osgEarth::Annotation::AnnotationLayer();
        shapeLayer->setName(CATEGORY);
        mapItem()->addLayer(shapeLayer);
        mHasLayer = true;
    }
}

//osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> DrawShape::shapeLayer() const
//{
//    return mShapeLayer;
//}

bool DrawShape::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;

    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        if (mState == State::START) {
           osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            startDraw(geoPos);
            return true;
        }
    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && mState == State::DRAWING) {
        cancelDrawing();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && mState == State::DRAWING) {
        finishDrawing();
        return false;
    }
    return false;
}

bool DrawShape::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;
    //move icon-----------------------
    if(mIconNode.valid()){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }
    //--------------------------------
    return false;
}

