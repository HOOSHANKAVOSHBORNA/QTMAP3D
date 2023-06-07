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

bool drawSphere::setup(MapItem *mapItem, UIHandle *uiHandle)
{
    mUiHandle = uiHandle;
    mMapcontroller = mapItem;
    mIconNode = makeIconNode();
    osgEarth::GLUtils::setGlobalDefaults(mMapcontroller->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *sphereLayer = new osgEarth::ModelLayer();
    sphereLayer->setName(DRAW_LAYER_NAME);
    mMapcontroller->addLayer(sphereLayer);

    return true;
}

bool drawSphere::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{

    if (mEnterSphereZone) {
        if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {
            if (mDrawingState == DrawingState::START) {
                mDrawingState = DrawingState::DRAWING;
                startDraw(ea);
                //                finishDrawing(event);
                return true;
            }
        }
        else if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON && mDrawingState == DrawingState::DRAWING) {
            cancelDrawing(ea);
        }
        else if (ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON && mDrawingState == DrawingState::DRAWING) {
            finishDrawing(ea);
        }
    }
    return false;
}

bool drawSphere::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterSphereZone) {
        osgEarth::GeoPoint geoPos = mMapcontroller->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
        return true;
    }
    return false;
}

bool drawSphere::startDraw( const osgGA::GUIEventAdapter &event)
{
    mSphere = new SphereNode();
    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event.getX(), event.getY(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);
    mSphere->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    //mSphereProperties->setLocation(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    mSphereProperties->setSphere(mSphere);

    mMapcontroller->addNodeToLayer(mSphere, DRAW_LAYER_NAME);
    return true;
}

bool drawSphere::finishDrawing(const osgGA::GUIEventAdapter &event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        return true;
    }
    return false;
}

bool drawSphere::cancelDrawing(const osgGA::GUIEventAdapter &event)
{
    if(mDrawingState == DrawingState::DRAWING){
        mMapcontroller->removeNodeFromLayer(mSphere, DRAW_LAYER_NAME);
        mSphere = nullptr;
        mSphereProperties->setSphere(mSphere);
        mDrawingState = DrawingState::START;
        return true;
    }
    return false;
}

osgEarth::Annotation::PlaceNode *drawSphere::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/sphere.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}
