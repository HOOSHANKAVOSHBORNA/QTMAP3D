#include "drawrect.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString RECT = "Rect";

DrawRect::DrawRect(QObject *parent)
    : PluginInterface(parent)
{
    qmlRegisterType<RectPropertiesModel>("Crystal", 1, 0, "RectProperties");
}
//bool DrawRect::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
//{
//    qmlRegisterType<RectPropertiesModel>("Crystal", 1, 0, "RectProperties");

//    desc->toolboxItemsList.push_back(new ItemDesc{RECT, CATEGORY, "qrc:/resources/rectangle.png", true,  false, ""});
//    return true;
//}

//void DrawRect::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
//{
//    if (CATEGORY == category) {
//        if (name == RECT) {
//            if (checked) {
//                mEnterRectZone = true;
//                mDrawingState = DrawingState::START;
//                addNodeToLayer(mIconNode);
//                mRectProperties = new RectProperties(mRect, qmlEngine(), uiHandle());
//                mRectProperties->show();

//            }
//            else {
//                mEnterRectZone = false;
//                mDrawingState = DrawingState::FINISH;
//                mRect = nullptr;
//                removeNodeFromLayer(mIconNode);
//                mRectProperties->hide();
//            }
//        }
//    }
//}

bool DrawRect::setup()
{
    auto toolboxItem =  new ToolboxItem{RECT, CATEGORY, "qrc:/resources/rectangle.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawRect::onRectItemCheck);
    toolbox()->addItem(toolboxItem);

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    mIconNode = makeIconNode();
    osgEarth::ModelLayer *rectLayer = new osgEarth::ModelLayer();
    rectLayer->setName(DRAW_LAYER_NAME);
    mapItem()->addLayer(rectLayer);
    return true;
}
bool DrawRect::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterRectZone){
        if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
        {
            if (mDrawingState == DrawingState::START) {
                mDrawingState = DrawingState::DRAWING;
                startDraw(ea);
//                finishDraw();
                return true;
            }
        }
        //cancel
        if(ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
        {
            if(mDrawingState == DrawingState::DRAWING)
            {
                cancelDraw(ea);
                return true;
            }
        }
        //finish
        if(ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
        {
            if(mDrawingState == DrawingState::DRAWING)
            {
                finishDraw(ea);
            }
        }
    }
    return false;
}

bool DrawRect::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterRectZone){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }
    return false;
}

void DrawRect::onRectItemCheck(bool check)
{
    if (check) {
        mEnterRectZone = true;
        mDrawingState = DrawingState::START;
        addNodeToLayer(mIconNode);
        mRectProperties = new RectProperties(mRect, qmlEngine(), uiHandle());
        mRectProperties->show();

    }
    else {
        mEnterRectZone = false;
        mDrawingState = DrawingState::FINISH;
        mRect = nullptr;
        removeNodeFromLayer(mIconNode);
        mRectProperties->hide();
    }
}

osgEarth::Annotation::PlaceNode *DrawRect::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/rectangle.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    if (icon){
        model->setIconImage(icon);
    }
    return model.release();
}

void DrawRect::startDraw(const osgGA::GUIEventAdapter &event)
{
    mRect = new Rect(mapItem());
    mRectProperties->setRect(mRect);

    mDrawingState = DrawingState::DRAWING;
    osg::Vec3d worldPos;
    mapItem()->screenToWorld(event.getX(), event.getY(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mapItem()->getMapSRS(), worldPos);

    mRect->setPosition(osgEarth::GeoPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y()));

    addNodeToLayer(mRect);
}

bool DrawRect::cancelDraw(const osgGA::GUIEventAdapter &event)
{
    if(mDrawingState == DrawingState::DRAWING){
    removeNodeFromLayer(mRect);
    mDrawingState = DrawingState::START;
    return true;
    }
    return false;
}

bool DrawRect::finishDraw(const osgGA::GUIEventAdapter &event)
{
    if (mDrawingState == DrawingState::DRAWING) {
        mDrawingState = DrawingState::START;
    return true;
    }
    return false;
}

bool DrawRect::addNodeToLayer(osg::Node *node)
{
    auto layer = mapItem()->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
    return true;

        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
    return false;
}
void DrawRect::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mapItem()->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}
