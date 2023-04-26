#include "drawsphere.h"


const QString CATEGORY = "Draw";
const QString SPHERE = "Sphere";

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
                mSphereProperties = new SphereProperties(mQmlEngine, mUiHandle, mMapcontroller);
                mSphereProperties->show();
                mMapcontroller->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

            }
            else {
                mEnterSphereZone = false;
                mDrawingState = DrawingState::FINISH;
                mSphere = nullptr;
                mSphereProperties->hide();
                mMapcontroller->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
            }
        }
    }
}

bool drawSphere::setup(MapController *mapController, UIHandle *uiHandle)
{
    mUiHandle = uiHandle;
    mMapcontroller = mapController;
    mIconNode = makeIconNode();
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

void drawSphere::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterSphereZone) {
        osgEarth::GeoPoint geoPos = mMapcontroller->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
    }
}

void drawSphere::startDraw(QMouseEvent *event)
{
    mSphere = new SphereNode();
    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);
    mSphere->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    //mSphereProperties->setLocation(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    mSphereProperties->setSphere(mSphere);

    mMapcontroller->addNodeToLayer(mSphere, DRAW_LAYER_NAME);
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
    mMapcontroller->removeNodeFromLayer(mSphere, DRAW_LAYER_NAME);
    mSphere = nullptr;
    mSphereProperties->setSphere(mSphere);
    mDrawingState = DrawingState::START;

    event->accept();
}

osgEarth::Annotation::PlaceNode *drawSphere::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/sphere.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}
