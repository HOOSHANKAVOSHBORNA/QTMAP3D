#include "drawCylinder.h"


const QString CATEGORY = "Draw";
const QString CYLINDER = "Cylinder";

DrawCylinder::DrawCylinder(QObject *parent): PluginInterface(parent)
{
}

bool DrawCylinder::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<CylinderPropertiesModel>("Crystal", 1, 0, "CylinderProperties");

    desc->toolboxItemsList.push_back(new ItemDesc{CYLINDER, CATEGORY, "qrc:/resources/cylinder.png", true,  false, ""});

    return true;
}

bool DrawCylinder::setup()
{
    mIconNode = makeIconNode();
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *circleLayer = new osgEarth::ModelLayer();
    circleLayer->setName(DRAW_LAYER_NAME);
    mapItem()->addLayer(circleLayer);

    return true;
}

void DrawCylinder::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == CYLINDER) {
            if (checked) {
                mEnterCylinderZone = true;
                mDrawingState = DrawingState::START;
                mCylinderProperties = new CylinderProperties(mCylinder, qmlEngine(), uiHandle(), mapItem());
                mCylinderProperties->show();
                mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

            }
            else {
                mEnterCylinderZone = false;
                mDrawingState = DrawingState::FINISH;
                mCylinder = nullptr;
                mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
                mCylinderProperties->hide();
            }
        }
    }
}

bool DrawCylinder::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterCylinderZone) {
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

bool DrawCylinder::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterCylinderZone) {
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }
    return false;
}

bool DrawCylinder::startDraw(const osgGA::GUIEventAdapter& event)
{
    mCylinder = new Cylinder();
    mCylinderProperties->setCylinder(mCylinder);
    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;
    mapItem()->screenToWorld(event.getX(), event.getY(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mapItem()->getMapSRS(), worldPos);
    mCylinder->setPosition(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));

    mCylinderProperties->setLocation(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));
    mapItem()->addNodeToLayer(mCylinder, DRAW_LAYER_NAME);
    return true;
}

bool DrawCylinder::finishDrawing(const osgGA::GUIEventAdapter& event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        return true;
    }
    return false;
}

bool DrawCylinder::cancelDrawing(const osgGA::GUIEventAdapter& event)
{
    if(mDrawingState == DrawingState::DRAWING){
        mapItem()->removeNodeFromLayer(mCylinder, DRAW_LAYER_NAME);
        mCylinder = nullptr;
        mCylinderProperties->setCylinder(mCylinder);
        mDrawingState = DrawingState::START;

        return true;
    }
    return false;
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
