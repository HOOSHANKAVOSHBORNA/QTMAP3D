#include "drawcircle.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString CIRCLE = "Circle";

DrawCircle::DrawCircle(QObject *parent)
    : PluginInterface(parent)
{

}

bool DrawCircle::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<CirclePropertiesModel>("Crystal", 1, 0, "CircleProperties");
    desc->toolboxItemsList.push_back(new ItemDesc{CIRCLE, CATEGORY, "qrc:/resources/circle.png", true,  false, ""});

    return true;
}


bool DrawCircle::setup()
{
    mIconNode = makeIconNode();
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *circleLayer = new osgEarth::ModelLayer();
    circleLayer->setName(DRAW_LAYER_NAME);
    mapItem()->addLayer(circleLayer);

    return true;
}


void DrawCircle::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == CIRCLE) {
            if (checked) {
                mEnterCircleZone = true;
                mCircleProperties = new CircleProperties(mCircle, qmlEngine(), uiHandle(), mapItem());
                mCircleProperties->show();
                mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);
                mDrawingState = DrawingState::START;

            }
            else {
                mEnterCircleZone = false;
                mDrawingState = DrawingState::FINISH;
                mCircle = nullptr;
                mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
                mCircleProperties->hide();
            }
        }
    }
}


bool DrawCircle::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterCircleZone) {
        if (ea.getButton() == Qt::MouseButton::LeftButton) {
            if (mDrawingState == DrawingState::START) {
                mDrawingState = DrawingState::DRAWING;
                startDraw(ea);
                //                finishDrawing(event);
                return true;
            }
        }
        else if (ea.getButton() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::DRAWING) {
            cancelDrawing(ea);
        }
        else if (ea.getButton() == Qt::MouseButton::MiddleButton && mDrawingState == DrawingState::DRAWING) {
            finishDrawing(ea);
        }
    }
    return false;
}

bool DrawCircle::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterCircleZone) {
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }
    return false;
}

osgEarth::Annotation::PlaceNode *DrawCircle::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/circle.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}

bool DrawCircle::startDraw(const osgGA::GUIEventAdapter& event)
{
    mCircle = new Circle(mapItem());
    mCircleProperties->setCircle(mCircle);

    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;
    mapItem()->screenToWorld(event.getX(), event.getY(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mapItem()->getMapSRS(), worldPos);

    mCircle->setPosition(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));

    mCircleProperties->setLocation(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));
    mapItem()->addNodeToLayer(mCircle, DRAW_LAYER_NAME);
    return true;
}

bool DrawCircle::cancelDrawing(const osgGA::GUIEventAdapter& event)
{
    if(mDrawingState == DrawingState::DRAWING){
        mapItem()->removeNodeFromLayer(mCircle, DRAW_LAYER_NAME);
        mCircle = nullptr;
        mCircleProperties->setCircle(mCircle);
        mDrawingState = DrawingState::START;
        return true;
    }
    return false;
}

bool DrawCircle::finishDrawing(const osgGA::GUIEventAdapter& event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        return true;
    }
    return false;
}
