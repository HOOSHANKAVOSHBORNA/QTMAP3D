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

bool DrawBox::setup(MapItem *mapItem, UIHandle *uiHandle)
{
    mUiHandle = uiHandle;
    mMapcontroller = mapItem;
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
                mMapcontroller->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

            }
            else {
                mEnterBoxZone = false;
                mDrawingState = DrawingState::FINISH;
                mBox = nullptr;
                mBoxProperties->hide();
                mMapcontroller->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
            }
        }
    }
}

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
        osgEarth::GeoPoint geoPos = mMapcontroller->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
        return true;
    }
    return false;
}

bool DrawBox::startDraw(const osgGA::GUIEventAdapter &ea)
{
    mBox = new Box();
    mBoxProperties->setBox(mBox);
    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;
    mMapcontroller->screenToWorld(ea.getX(), ea.getY(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapcontroller->getMapSRS(), worldPos);
    mBox->setPosition(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    mBoxProperties->setLocation(osgEarth::GeoPoint(mMapcontroller->getMapSRS(), geoPos.x(), geoPos.y()));
    mMapcontroller->addNodeToLayer(mBox, DRAW_LAYER_NAME);
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
        mMapcontroller->removeNodeFromLayer(mBox, DRAW_LAYER_NAME);
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
