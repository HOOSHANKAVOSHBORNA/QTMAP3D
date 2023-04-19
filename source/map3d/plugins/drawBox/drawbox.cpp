#include "drawbox.h"


const QString CATEGORY = "Draw";
const QString BOX = "Box";

DrawBox::DrawBox(QObject *parent): PluginInterface(parent)
{
}

bool DrawBox::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mQmlEngine = engine;
    qmlRegisterType<BoxPropertiesModel>("Crystal", 1, 0, "BoxProperties");

    desc->toolboxItemsList.push_back(new ItemDesc{BOX, CATEGORY, "qrc:/resources/box.png", true,  false, ""});

    return true;
}

bool DrawBox::setup(MapController *mapController, UIHandle *uiHandle)
{
    mUiHandle = uiHandle;
    mMapcontroller = mapController;
    mIconNode = makeIconNode();
    osgEarth::GLUtils::setGlobalDefaults(mMapcontroller->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *boxLayer = new osgEarth::ModelLayer();
    boxLayer->setName(DRAW_LAYER_NAME);
    mMapcontroller->addLayer(boxLayer);

    return true;
}

void DrawBox::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == BOX) {
            if (checked) {
                mEnterBoxZone = true;
                mDrawingState = DrawingState::START;
                mBoxProperties = new BoxProperties(mBox, mQmlEngine, mUiHandle, mMapcontroller);
                mBoxProperties->show();
                addNodeToLayer(mIconNode);

            }
            else {
                mEnterBoxZone = false;
                mDrawingState = DrawingState::FINISH;
                mBox = nullptr;
                removeNodeFromLayer(mIconNode);
                mBoxProperties->hide();
            }
        }
    }
}

void DrawBox::mousePressEvent(QMouseEvent *event)
{
    if (mEnterBoxZone) {
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

void DrawBox::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterBoxZone) {
        osgEarth::GeoPoint geoPos = mMapcontroller->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
    }
}

void DrawBox::startDraw(QMouseEvent *event)
{
    mBox = new Box();
    mBoxProperties->setBox(mBox);

    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);
    mBox->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));


    mBoxProperties->setLocation(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    addNodeToLayer(mBox);
    event->accept();
}

void DrawBox::finishDrawing(QMouseEvent *event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        event->accept();
    }
}

void DrawBox::cancelDrawing(QMouseEvent *event)
{
    removeNodeFromLayer(mBox);
    mBox = nullptr;
//    mBoxProperties->setBox(mBox);
    mDrawingState = DrawingState::START;

    event->accept();
}

bool DrawBox::addNodeToLayer(osg::Node *node)
{
    auto layer = mMapcontroller->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
}

void DrawBox::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapcontroller->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}

osgEarth::Annotation::PlaceNode *DrawBox::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/box.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}
