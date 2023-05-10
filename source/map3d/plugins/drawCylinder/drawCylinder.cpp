#include "drawCylinder.h"


const QString CATEGORY = "Draw";
const QString CYLINDER = "Cylinder";

DrawCylinder::DrawCylinder(QObject *parent): PluginInterface(parent)
{
}

bool DrawCylinder::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mQmlEngine = engine;
    qmlRegisterType<CylinderPropertiesModel>("Crystal", 1, 0, "CylinderProperties");

    desc->toolboxItemsList.push_back(new ItemDesc{CYLINDER, CATEGORY, "qrc:/resources/cylinder.png", true,  false, ""});

    return true;
}

bool DrawCylinder::setup(MapController *mapController, UIHandle *uiHandle)
{
    mUiHandle = uiHandle;
    mMapcontroller = mapController;
    mIconNode = makeIconNode();
    osgEarth::GLUtils::setGlobalDefaults(mMapcontroller->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *circleLayer = new osgEarth::ModelLayer();
    circleLayer->setName(DRAW_LAYER_NAME);
    mMapcontroller->addLayer(circleLayer);

    return true;
}

void DrawCylinder::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == CYLINDER) {
            if (checked) {
                mEnterCylinderZone = true;
                mDrawingState = DrawingState::START;
                mCylinderProperties = new CylinderProperties(mCylinder, mQmlEngine, mUiHandle, mMapcontroller);
                mCylinderProperties->show();
                mMapcontroller->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

            }
            else {
                mEnterCylinderZone = false;
                mDrawingState = DrawingState::FINISH;
                mCylinder = nullptr;
                mMapcontroller->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
                mCylinderProperties->hide();
            }
        }
    }
}

void DrawCylinder::mousePressEvent(QMouseEvent *event)
{
    if (mEnterCylinderZone) {
        if (event->button() == Qt::MouseButton::LeftButton) {
            if (mDrawingState == DrawingState::START) {
                mDrawingState = DrawingState::DRAWING;
                startDraw(event);
//                finishDrawing(event);
                event->accept();
            }
        }
        else if (event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::DRAWING) {
            cancelDrawing(event);
        }
        else if (event->button() == Qt::MouseButton::MidButton && mDrawingState == DrawingState::DRAWING) {
            finishDrawing(event);
        }
    }
}

void DrawCylinder::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterCylinderZone) {
        osgEarth::GeoPoint geoPos = mMapcontroller->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
    }
}

void DrawCylinder::startDraw(QMouseEvent *event)
{
    mCylinder = new Cylinder();
    mCylinderProperties->setCylinder(mCylinder);
    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);
    mCylinder->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));

    mCylinderProperties->setLocation(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    mMapcontroller->addNodeToLayer(mCylinder, DRAW_LAYER_NAME);
    event->accept();
}

void DrawCylinder::finishDrawing(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        event->accept();
    }
}

void DrawCylinder::cancelDrawing(QMouseEvent *event)
{
    if(mDrawingState == DrawingState::DRAWING){
        mMapcontroller->removeNodeFromLayer(mCylinder, DRAW_LAYER_NAME);
        mCylinder = nullptr;
        mCylinderProperties->setCylinder(mCylinder);
        mDrawingState = DrawingState::START;

        event->accept();
    }
}

osgEarth::Annotation::PlaceNode *DrawCylinder::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/cylinder.png");
    if (icon)
        icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}
