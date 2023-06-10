#include "drawcone.h"


const QString CATEGORY = "Draw";
const QString CONE = "Cone";

DrawCone::DrawCone(QObject *parent): PluginInterface(parent)
{
}

bool DrawCone::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<ConePropertiesModel>("Crystal", 1, 0, "ConeProperties");

    desc->toolboxItemsList.push_back(new ItemDesc{CONE, CATEGORY, "qrc:/resources/cone.png", true,  false, ""});

    return true;
}

bool DrawCone::setup()
{
    mIconNode = makeIconNode();
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *circleLayer = new osgEarth::ModelLayer();
    circleLayer->setName(DRAW_LAYER_NAME);
    mapItem()->addLayer(circleLayer);

    return true;
}

void DrawCone::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == CONE) {
            if (checked) {
                mEnterConeZone = true;
                mDrawingState = DrawingState::START;
                mConeProperties = new ConeProperties(mCone, qmlEngine(), uiHandle(), mapItem());
                mConeProperties->show();
                mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

            }
            else {
                mEnterConeZone = false;
                mDrawingState = DrawingState::FINISH;
                mCone = nullptr;
                mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
                mConeProperties->hide();
            }
        }
    }
}

bool DrawCone::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterConeZone) {
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

bool DrawCone::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterConeZone) {
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }
    return false;
}

bool DrawCone::startDraw(const osgGA::GUIEventAdapter &event)
{
    mCone = new Cone();
    mConeProperties->setCone(mCone);

    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;
    mapItem()->screenToWorld(event.getX(), event.getY(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mapItem()->getMapSRS(), worldPos);
    mCone->setPosition(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));

    mConeProperties->setLocation(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));
    mapItem()->addNodeToLayer(mCone, DRAW_LAYER_NAME);
    return true;
}

bool DrawCone::finishDrawing(const osgGA::GUIEventAdapter& event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        return true;
    }
    return false;
}

bool DrawCone::cancelDrawing(const osgGA::GUIEventAdapter &event)
{
    if(mDrawingState == DrawingState::DRAWING){
    mapItem()->removeNodeFromLayer(mCone, DRAW_LAYER_NAME);
    mCone = nullptr;
    mConeProperties->setCone(mCone);
    mDrawingState = DrawingState::START;
    return true;
    }
    return false;
}

osgEarth::Annotation::PlaceNode *DrawCone::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/cone.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}
