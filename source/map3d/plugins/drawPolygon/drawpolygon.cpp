#include "drawpolygon.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString POLYGON = "Polygon";
const QString M_CATEGORY = "Measurement";
const QString MEASUREAREA = "Measure Area";

DrawPolygon::DrawPolygon(QObject *parent)
    : PluginInterface(parent)
{
    qmlRegisterType<PolygonPropertiesModel>("Crystal", 1, 0, "PolygonProperties");
}

//bool DrawPolygon::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
//{
//    qmlRegisterType<PolygonPropertiesModel>("Crystal", 1, 0, "PolygonProperties");
//    desc->toolboxItemsList.push_back(new ItemDesc{POLYGON, CATEGORY, "qrc:/resources/polygon.png", true,  false, ""});
//    return true;
//}

//void DrawPolygon::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
//{
//    if (CATEGORY == category) {
//        if (name == POLYGON) {
//            if (checked) {
//                mEnterPolygonZone = true;
//                mPolygonProperties = new PolygonProperties(qmlEngine(), uiHandle());
//                if(/*mUiHandle &&*/ mPolygonProperties){
//                    mPolygonProperties->show();
//                }
//                mDrawingState = DrawingState::START;
//                mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

//            }
//            else {
//                mEnterPolygonZone = false;
//                mDrawingState = DrawingState::FINISH;
//                if(mPolygonProperties){
//                        mPolygonProperties->hide();
//                    }

//                mPolygonProperties->deleteLater();
//                mPolygonProperties = nullptr;
//                mPolygon = nullptr;
//                mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
//            }
//        }
//    }
//}

bool DrawPolygon::setup()
{
    auto toolboxItem =  new ToolboxItem{POLYGON, CATEGORY, "qrc:/resources/polygon.png", true};
    auto toolboxItemArea =  new ToolboxItem{MEASUREAREA, M_CATEGORY, "qrc:/resources/polygon.png", true};
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawPolygon::onPolygonItemCheck);
    toolbox()->addItem(toolboxItem);
    QObject::connect(toolboxItemArea, &ToolboxItem::itemChecked, this, &DrawPolygon::onMeasureAreaItemCheck);
    toolbox()->addItem(toolboxItemArea);

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    mIconNode = makeIconNode();
    osgEarth::ModelLayer *polygonLayer = new osgEarth::ModelLayer();
    polygonLayer->setName(DRAW_LAYER_NAME);
    mapItem()->addLayer(polygonLayer);
    return true;
}

bool DrawPolygon::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterPolygonZone){
        if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
        {
            if(mDrawingState == DrawingState::START)
            {
                startDraw(ea);
                return true;
            }

            if(mDrawingState == DrawingState::DRAWING)
            {
                drawing(ea);
                return true;
            }
        }
        //cancel
        if(ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
        {
            if(mDrawingState == DrawingState::DRAWING)
            {
                cancelDraw();
                return true;
            }
        }
        //finish
        if(ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
        {
            if(mDrawingState == DrawingState::DRAWING)
            {
                finishDraw();
                return true;
            }
        }
    }
    return false;
}

bool DrawPolygon::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (mEnterPolygonZone){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
        if (mDrawingState == DrawingState::DRAWING){
            mouseMoveDrawing(ea);
        }
    }
    return false;
}

void DrawPolygon::onPolygonItemCheck(bool check)
{
    if (check) {
        mEnterPolygonZone = true;
        mPolygonProperties = new PolygonProperties(qmlEngine(), uiHandle());
        if(/*mUiHandle &&*/ mPolygonProperties){
            mPolygonProperties->show();
        }
        mDrawingState = DrawingState::START;
        mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

    }
    else {
        mEnterPolygonZone = false;
        mDrawingState = DrawingState::FINISH;
        if(mPolygonProperties){
            mPolygonProperties->hide();
        }

        mPolygonProperties->deleteLater();
        mPolygonProperties = nullptr;
        mPolygon = nullptr;
        mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
    }
}

void DrawPolygon::onMeasureAreaItemCheck(bool check)
{
    if(check){
        mAreaMode = true;
        onPolygonItemCheck(true);
    }
}

osgEarth::Annotation::PlaceNode *DrawPolygon::makeIconNode()
{
    osg::ref_ptr<osg::Image> icon = osgDB::readImageFile("../data/images/draw/polygon.png");
    icon->scaleImage(24, 24, icon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(icon);
    return model.release();
}

//void DrawPolygon::mouseDoubleClickEvent(QMouseEvent *event)
//{

//}

void DrawPolygon::startDraw(const osgGA::GUIEventAdapter &event)
{
    mPolygon = new Polygon(mapItem());
    mapItem()->addNodeToLayer(mPolygon, DRAW_LAYER_NAME);
    mDrawingState = DrawingState::DRAWING;
    mPolygonProperties->setPolygon(mPolygon);
}
bool DrawPolygon::drawing(const osgGA::GUIEventAdapter &event)
{
    osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(event.getX(), event.getY());
    mPolygon->addPoints(geoPos);
    return false;
}

void DrawPolygon::cancelDraw()
{
    mapItem()->removeNodeFromLayer(mPolygon, DRAW_LAYER_NAME);
    mDrawingState = DrawingState::START;
    if(mPolygonProperties)
        mPolygonProperties->setPolygon(nullptr);
}

void DrawPolygon::finishDraw()
{
    if(mDrawingState == DrawingState::DRAWING)
    {
        mDrawingState = DrawingState::START;
    }
}
bool DrawPolygon::mouseMoveDrawing(const osgGA::GUIEventAdapter &event)
{
    if (mPolygon->getSize() >= 2)
    {
        mPolygon->removePoint();

    }
    osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(event.getX(), event.getY());
    mPolygon->addPoints(geoPos);
    if (mPolygon->getSize() > 4 && mAreaMode){
        qDebug()<<"Area is: "<<mPolygon->CalculateAreaOfPolygon();
    }
    return false;

}
