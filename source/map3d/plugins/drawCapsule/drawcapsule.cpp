#include "drawcapsule.h"


const QString CATEGORY = "Draw";
const QString CAPSULE = "Capsule";

DrawCapsule::DrawCapsule(QObject *parent): PluginInterface(parent)
{
}

bool DrawCapsule::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<CapsulePropertiesModel>("Crystal", 1, 0, "CapsuleProperties");

    desc->toolboxItemsList.push_back(new ItemDesc{CAPSULE, CATEGORY, "qrc:/resources/capsule.png", true,  false, ""});

    return true;
}

bool DrawCapsule::setup()
{
    mIconNode = makeIconNode();
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *circleLayer = new osgEarth::ModelLayer();
    circleLayer->setName(DRAW_LAYER_NAME);
    mapItem()->addLayer(circleLayer);

    return true;
}

void DrawCapsule::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == CAPSULE) {
            if (checked) {
                mEnterCapsuleZone = true;
                mDrawingState = DrawingState::START;
                mCapsuleProperties = new CapsuleProperties(mCapsule, qmlEngine(), uiHandle(), mapItem());
                mCapsuleProperties->show();
                mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

            }
            else {
                mEnterCapsuleZone = false;
                mDrawingState = DrawingState::FINISH;
                mCapsule = nullptr;
                mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
                mCapsuleProperties->hide();
            }
        }
    }
}

bool DrawCapsule::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterCapsuleZone) {
        if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {
            if (mDrawingState == DrawingState::START) {
                mDrawingState = DrawingState::DRAWING;
                startDraw(ea);
//                finishDrawing(event);
                return true;
            }
        }
        else if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
            cancelDrawing(ea);
        }
        else if (ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON && mDrawingState == DrawingState::DRAWING) {
            finishDrawing(ea);
        }
    }
    return false;
}

bool DrawCapsule::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterCapsuleZone) {
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }
    return false;
}

bool DrawCapsule::startDraw(const osgGA::GUIEventAdapter &event)
{
    mCapsule = new Capsule();
    mCapsuleProperties->setCapsule(mCapsule);
    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;
    mapItem()->screenToWorld(event.getX(), event.getY(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mapItem()->getMapSRS(), worldPos);
    mCapsule->setPosition(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));

    mCapsuleProperties->setLocation(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));
    mapItem()->addNodeToLayer(mCapsule, DRAW_LAYER_NAME);
    return true;
}

bool DrawCapsule::finishDrawing(const osgGA::GUIEventAdapter &event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        return true;
    }
    return false;
}

bool DrawCapsule::cancelDrawing(const osgGA::GUIEventAdapter &event)
{
    if(mDrawingState == DrawingState::DRAWING){
        mapItem()->removeNodeFromLayer(mCapsule, DRAW_LAYER_NAME);
        mCapsule = nullptr;
        mCapsuleProperties->setCapsule(mCapsule);
        mDrawingState = DrawingState::START;
    }

    return true;
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
