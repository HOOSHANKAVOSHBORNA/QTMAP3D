#include "drawcone.h"


const QString CATEGORY = "Draw";
const QString CONE = "Cone";

DrawCone::DrawCone(QObject *parent): PluginInterface(parent)
{
}

bool DrawCone::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mQmlEngine = engine;
    qmlRegisterType<ConePropertiesModel>("Crystal", 1, 0, "ConeProperties");

    desc->toolboxItemsList.push_back(new ItemDesc{CONE, CATEGORY, "qrc:/resources/cone.png", true,  false, ""});

    return true;
}

bool DrawCone::setup(MapController *mapController, UIHandle *uiHandle)
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

void DrawCone::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == CONE) {
            if (checked) {
                mEnterConeZone = true;
                mDrawingState = DrawingState::START;
                mConeProperties = new ConeProperties(mCone, mQmlEngine, mUiHandle, mMapcontroller);
                mConeProperties->show();
                addNodeToLayer(mIconNode);

            }
            else {
                mEnterConeZone = false;
                mDrawingState = DrawingState::FINISH;
                mCone = nullptr;
                removeNodeFromLayer(mIconNode);
                mConeProperties->hide();
            }
        }
    }
}

void DrawCone::mousePressEvent(QMouseEvent *event)
{
    if (mEnterConeZone) {
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

void DrawCone::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterConeZone) {
        osgEarth::GeoPoint geoPos = mMapcontroller->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
    }
}

void DrawCone::startDraw(QMouseEvent *event)
{
    mCone = new Cone(false);
    mConeProperties->setCone(mCone);

    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);

    mCone->setRadius(100000);
    mCone->setHeight(100000);
    mCone->setColor(osgEarth::Color::Red);
    mCone->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));


    addNodeToLayer(mCone);
    event->accept();
}

void DrawCone::finishDrawing(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        event->accept();
    }
}

void DrawCone::cancelDrawing(QMouseEvent *event)
{
    removeNodeFromLayer(mCone);
    mCone = nullptr;
    mConeProperties->setCone(mCone);
    mDrawingState = DrawingState::START;

    event->accept();
}

bool DrawCone::addNodeToLayer(osg::Node *node)
{
    auto layer = mMapcontroller->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
}

void DrawCone::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapcontroller->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}

osgEarth::Annotation::PlaceNode *DrawCone::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/cone.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}
