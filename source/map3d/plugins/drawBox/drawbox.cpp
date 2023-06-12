#include "drawbox.h"


const QString CATEGORY = "Draw";
const QString BOX = "Box";

DrawBox::DrawBox(QObject *parent): PluginInterface(parent)
{
//    Q_INIT_RESOURCE(drawBox);
    qmlRegisterType<BoxPropertiesModel>("Crystal", 1, 0, "BoxProperties");
}

//bool DrawBox::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
//{
////    qmlRegisterType<BoxPropertiesModel>("Crystal", 1, 0, "BoxProperties");

////    desc->toolboxItemsList.push_back(new ItemDesc{BOX, CATEGORY, "qrc:/resources/box.png", true,  false, ""});

//    return true;
//}

bool DrawBox::setup()
{
    auto toolboxItem =  new ToolboxItem{BOX, CATEGORY, "qrc:/resources/box.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawBox::onBoxItemCheck);
    toolbox()->addItem(toolboxItem);

    mIconNode = makeIconNode();
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    osgEarth::ModelLayer *boxLayer = new osgEarth::ModelLayer();
    boxLayer->setName(DRAW_LAYER_NAME);
    mapItem()->addLayer(boxLayer);

    return true;
}

//void DrawBox::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
//{
//    if (CATEGORY == category) {
//        if (name == BOX) {
//            if (checked) {
//                mEnterBoxZone = true;
//                mDrawingState = DrawingState::START;
//                mBoxProperties = new BoxProperties(mBox, qmlEngine(), uiHandle(), mapItem());
//                mBoxProperties->show();
//                mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

//            }
//            else {
//                mEnterBoxZone = false;
//                mDrawingState = DrawingState::FINISH;
//                mBox = nullptr;
//                mBoxProperties->hide();
//                mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
//            }
//        }
//    }
//}

bool DrawBox::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterBoxZone) {
        if (ea.getButton() == osgGA::GUIEventAdapter::MouseButtonMask::LEFT_MOUSE_BUTTON) {
            if (mDrawingState == DrawingState::START) {
                mDrawingState = DrawingState::DRAWING;
                return startDraw(ea);
//                finishDrawing(event);
                return true;
            }
        }
        else if (ea.getButton() == osgGA::GUIEventAdapter::MouseButtonMask::RIGHT_MOUSE_BUTTON && mDrawingState == DrawingState::DRAWING) {
            cancelDrawing(ea);
            return false;
        }
        else if (ea.getButton() == osgGA::GUIEventAdapter::MouseButtonMask::MIDDLE_MOUSE_BUTTON && mDrawingState == DrawingState::DRAWING) {
            finishDrawing(ea);
            return false;
        }
    }
    return false;
}

bool DrawBox::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterBoxZone) {
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
        return true;
    }
    return false;
}

void DrawBox::onBoxItemCheck(bool check)
{
    if (check) {
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


bool DrawBox::startDraw(const osgGA::GUIEventAdapter &ea)
{
    mBox = new Box();
    mBox->setHeight(100000);
    mBox->setWidth(100000);
    mBox->setLength(100000);
    mBoxProperties->setBox(mBox);
    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;

    mapItem()->screenToWorld(ea.getX(), ea.getY(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mapItem()->getMapSRS(), worldPos);
    mBox->setPosition(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));
//    mBoxProperties->setLocation(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));
    mapItem()->addNodeToLayer(mBox, DRAW_LAYER_NAME);
    return true;
}

bool DrawBox::finishDrawing(const osgGA::GUIEventAdapter &ea)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
        return true;
    }
    return false;
}

bool DrawBox::cancelDrawing(const osgGA::GUIEventAdapter &ea)
{
    if(mDrawingState == DrawingState::DRAWING){
        mapItem()->removeNodeFromLayer(mBox, DRAW_LAYER_NAME);
        mBox = nullptr;
        mBoxProperties->setBox(mBox);
        mDrawingState = DrawingState::START;
        return true;
    }
    return false;
}

osgEarth::Annotation::PlaceNode *DrawBox::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/box.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}
