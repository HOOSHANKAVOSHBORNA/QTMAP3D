#include "drawbox.h"


const QString CATEGORY = "Draw";
const QString BOX = "Box";

DrawBox::DrawBox(QObject *parent): PluginInterface(parent)
{
}

bool DrawBox::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<BoxPropertiesModel>("Crystal", 1, 0, "BoxProperties");

    desc->toolboxItemsList.push_back(new ItemDesc{BOX, CATEGORY, "qrc:/resources/box.png", true,  false, ""});

    return true;
}

bool DrawBox::setup()
{
    mIconNode = makeIconNode();
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *boxLayer = new osgEarth::ModelLayer();
    boxLayer->setName(DRAW_LAYER_NAME);
    mapItem()->addLayer(boxLayer);

    return true;
}

void DrawBox::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == BOX) {
            if (checked) {
                mEnterBoxZone = true;
                mDrawingState = DrawingState::START;
                mBoxProperties = new BoxProperties(mBox, qmlEngine(), uiHandle(), mapItem());
                mBoxProperties->show();
                mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

            }
            else {
                mEnterBoxZone = false;
                mDrawingState = DrawingState::FINISH;
                mBox = nullptr;
                mBoxProperties->hide();
                mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
            }
        }
    }
}

//void DrawBox::mousePressEvent(QMouseEvent *event)
//{
//    if (mEnterBoxZone) {
//        if (event->button() == Qt::MouseButton::LeftButton) {
//            if (mDrawingState == DrawingState::START) {
//                mDrawingState = DrawingState::DRAWING;
//                startDraw(event);
////                finishDrawing(event);
//                event->accept();
//            }
//        }
//        else if (event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::DRAWING) {
//            cancelDrawing(event);
//        }
//        else if (event->button() == Qt::MouseButton::MiddleButton && mDrawingState == DrawingState::DRAWING) {
//            finishDrawing(event);
//        }
//    }
//}

//void DrawBox::mouseMoveEvent(QMouseEvent *event)
//{
//    if (mEnterBoxZone) {
//        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(event->x(), event->y());
//        mIconNode->setPosition(geoPos);
//    }
//}

void DrawBox::startDraw(QMouseEvent *event)
{
    mBox = new Box();
    mBoxProperties->setBox(mBox);
    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;
    mapItem()->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mapItem()->getMapSRS(), worldPos);
    mBox->setPosition(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));
    mBoxProperties->setLocation(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));
    mapItem()->addNodeToLayer(mBox, DRAW_LAYER_NAME);
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
    if(mDrawingState == DrawingState::DRAWING){
        mapItem()->removeNodeFromLayer(mBox, DRAW_LAYER_NAME);
        mBox = nullptr;
        mBoxProperties->setBox(mBox);
        mDrawingState = DrawingState::START;
        event->accept();
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
