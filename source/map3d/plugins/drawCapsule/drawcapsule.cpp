#include "drawcapsule.h"


const QString CATEGORY = "Draw";
const QString CAPSULE = "Capsule";

DrawCapsule::DrawCapsule(QObject *parent): PluginInterface(parent)
{
}

bool DrawCapsule::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mQmlEngine = engine;
    qmlRegisterType<CapsulePropertiesModel>("Crystal", 1, 0, "CapsuleProperties");

    desc->toolboxItemsList.push_back(new ItemDesc{CAPSULE, CATEGORY, "qrc:/resources/capsule.png", true,  false, ""});

    return true;
}

bool DrawCapsule::setup(MapController *mapController, UIHandle *uiHandle)
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

void DrawCapsule::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == CAPSULE) {
            if (checked) {
                mEnterCapsuleZone = true;
                mDrawingState = DrawingState::START;
                mCapsuleProperties = new CapsuleProperties(mCapsule, mQmlEngine, mUiHandle, mMapcontroller);
                mCapsuleProperties->show();
                addNodeToLayer(mIconNode);

            }
            else {
                mEnterCapsuleZone = false;
                mDrawingState = DrawingState::FINISH;
                mCapsule = nullptr;
                removeNodeFromLayer(mIconNode);
                mCapsuleProperties->hide();
            }
        }
    }
}

void DrawCapsule::mousePressEvent(QMouseEvent *event)
{
    if (mEnterCapsuleZone) {
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
        else if (event->button() == Qt::MouseButton::MidButton && mDrawingState == DrawingState::DRAWING) {
            finishDrawing(event);
        }
    }
}

void DrawCapsule::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterCapsuleZone) {
        osgEarth::GeoPoint geoPos = mMapcontroller->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
    }
}

void DrawCapsule::startDraw(QMouseEvent *event)
{
    mCapsule = new Capsule();
    mCapsuleProperties->setCapsule(mCapsule);

    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);
    mCapsule->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));

    mCapsuleProperties->setLocation(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    addNodeToLayer(mCapsule);
    event->accept();
}

void DrawCapsule::finishDrawing(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        event->accept();
    }
}

void DrawCapsule::cancelDrawing(QMouseEvent *event)
{
    removeNodeFromLayer(mCapsule);
    mCapsule = nullptr;
    mCapsuleProperties->setCapsule(mCapsule);
    mDrawingState = DrawingState::START;

    event->accept();
}

bool DrawCapsule::addNodeToLayer(osg::Node *node)
{
    auto layer = mMapcontroller->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
}

void DrawCapsule::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapcontroller->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}

osgEarth::Annotation::PlaceNode *DrawCapsule::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/capsule.png");
    if (icon)
        icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}
