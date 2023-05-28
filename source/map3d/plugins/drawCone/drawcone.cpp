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

bool DrawCone::setup(MapItem *mapItem, UIHandle *uiHandle)
{
    mUiHandle = uiHandle;
    mMapcontroller = mapItem;
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
                mMapcontroller->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

            }
            else {
                mEnterConeZone = false;
                mDrawingState = DrawingState::FINISH;
                mCone = nullptr;
                mMapcontroller->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
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
//                finishDrawing(event);
                event->accept();
            }
        }
        else if (event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::DRAWING) {
            cancelDrawing(event);
        }
        else if (event->button() == Qt::MouseButton::MiddleButton && mDrawingState == DrawingState::DRAWING) {
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
    mCone = new Cone();
    mConeProperties->setCone(mCone);

    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);
    mCone->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));

    mConeProperties->setLocation(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    mMapcontroller->addNodeToLayer(mCone, DRAW_LAYER_NAME);
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
    if(mDrawingState == DrawingState::DRAWING){
    mMapcontroller->removeNodeFromLayer(mCone, DRAW_LAYER_NAME);
    mCone = nullptr;
    mConeProperties->setCone(mCone);
    mDrawingState = DrawingState::START;

    event->accept();
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
