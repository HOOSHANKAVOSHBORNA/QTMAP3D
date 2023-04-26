#include "drawellipse.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString ELLIPSE = "Ellipse";

DrawEllipse::DrawEllipse(QObject *parent)
    : PluginInterface(parent)
{

}
bool DrawEllipse::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mQmlEngine = engine;
    qmlRegisterType<EllipsePropertiesModel>("Crystal", 1, 0, "EllipseProperties");

    desc->toolboxItemsList.push_back(new ItemDesc{ELLIPSE, CATEGORY, "qrc:/resources/ellipse.png", true,  false, ""});
    return true;
}

void DrawEllipse::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == ELLIPSE) {
            if (checked) {
                mEnterEllipseZone = true;
                mDrawingState = DrawingState::START;
                mEllipseProperties = new EllipseProperties(mEllipse, mQmlEngine, mUiHandle);
                mEllipseProperties->show();
//                addNodeToLayer(mIconNode);

                mMapController->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

            }
            else {
                mEnterEllipseZone = false;
                mDrawingState = DrawingState::FINISH;
                mEllipse = nullptr;

                mEllipseProperties->hide();
//                removeNodeFromLayer(mIconNode);
                mMapController->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);

            }
        }
    }
}

bool DrawEllipse::setup(MapController *mapController, UIHandle *uIHandle)
{
    mUiHandle = uIHandle;
    mMapController = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());
    mIconNode = makeIconNode();
    osgEarth::ModelLayer *ellipseLayer = new osgEarth::ModelLayer();
    ellipseLayer->setName(DRAW_LAYER_NAME);
    mMapController->addLayer(ellipseLayer);
    return true;
}
void DrawEllipse::mousePressEvent(QMouseEvent *event)
{
    if (mEnterEllipseZone){
        if(event->button() == Qt::MouseButton::LeftButton)
        {
            if (mDrawingState == DrawingState::START) {
                mDrawingState = DrawingState::DRAWING;
                startDraw(event);
                finishDraw();
                event->accept();
            }
        }
        //cancel
        if(event->button() == Qt::MouseButton::RightButton)
        {
            if(mDrawingState == DrawingState::START)
            {
                cancelDraw();
                event->accept();
            }
        }
        //finish
        if(event->button() == Qt::MouseButton::MidButton)
        {
            if(mDrawingState == DrawingState::START)
            {
                finishDraw();
            }
        }
    }
}

void DrawEllipse::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterEllipseZone){
        osgEarth::GeoPoint geoPos = mMapController->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
    }
}

osgEarth::Annotation::PlaceNode *DrawEllipse::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/ellipse.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}

void DrawEllipse::startDraw(QMouseEvent *event)
{
    mEllipse = new Ellipse(mMapController, true);
    mEllipseProperties->setEllipse(mEllipse);
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    mEllipse->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y()));

    mMapController->addNodeToLayer(mEllipse, DRAW_LAYER_NAME);
}

void DrawEllipse::cancelDraw()
{
    mMapController->removeNodeFromLayer(mEllipse, DRAW_LAYER_NAME);
    mDrawingState = DrawingState::START;
}

void DrawEllipse::finishDraw()
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
    }
}