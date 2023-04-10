#include "drawsphere.h"


const QString CATEGORY = "Draw";
const QString SPHERE = "SphereTest";

drawSphere::drawSphere(QObject *parent)
    : PluginInterface(parent)
{

}

bool drawSphere::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<SpherePropertiesModel>("Crystal", 1, 0, "SphereProperties");
    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{SPHERE, CATEGORY, "qrc:/resources/sphere.png", true,  false, ""});

    return true;
}

void drawSphere::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == SPHERE) {
            if (checked) {
                mEnterSphereZone = true;
                mDrawingState = DrawingState::START;
                mSphereProperties = new SphereProperties(mSphere, mQmlEngine, mUiHandle, mMapcontroller);
                mSphereProperties->show();

            }
            else {
                mEnterSphereZone = false;
                mDrawingState = DrawingState::FINISH;
                mSphere = nullptr;
                mSphereProperties->hide();
            }
        }
    }
}

bool drawSphere::setup(MapController *mapController, UIHandle *uiHandle)
{
    mUiHandle = uiHandle;
    mMapcontroller = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapcontroller->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *sphereLayer = new osgEarth::ModelLayer();
    sphereLayer->setName(DRAW_LAYER_NAME);
    mMapcontroller->addLayer(sphereLayer);

    return true;
}

void drawSphere::mousePressEvent(QMouseEvent *event)
{
    if (mEnterSphereZone) {
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

void drawSphere::startDraw(QMouseEvent *event)
{
    mSphere = new SphereNode();
    mSphereProperties->setSphere(mSphere);
    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);
    mSphere->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));

    addNodeToLayer(mSphere);
    event->accept();
}

void drawSphere::finishDrawing(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        event->accept();
    }
}

void drawSphere::cancelDrawing(QMouseEvent *event)
{
    removeNodeFromLayer(mSphere);
    mSphere = nullptr;
    mDrawingState = DrawingState::START;

    event->accept();
}

bool drawSphere::addNodeToLayer(osg::Node *node)
{
    auto layer = mMapcontroller->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
}

void drawSphere::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapcontroller->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}
