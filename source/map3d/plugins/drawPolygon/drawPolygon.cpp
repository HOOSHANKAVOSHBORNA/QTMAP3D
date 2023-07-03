#include "drawPolygon.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>


int DrawPolygon::mCount{0};
DrawPolygon::DrawPolygon(QObject *parent)
    : DrawShape(parent)
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
    QObject::connect(toolboxItem, &ToolboxItem::itemChecked, this, &DrawPolygon::onPolygonItemCheck);
    toolbox()->addItem(toolboxItem);

    auto toolboxItemArea =  new ToolboxItem{MEASUREAREA, M_CATEGORY, "qrc:/resources/polygon.png", true};
    QObject::connect(toolboxItemArea, &ToolboxItem::itemChecked, this, &DrawPolygon::onMeasureAreaItemCheck);
    toolbox()->addItem(toolboxItemArea);

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    //    addLayer();
    mPolygonLayer = new osgEarth::Annotation::AnnotationLayer();
    mPolygonLayer->setName(POLYGON);

    mAreaLayer = new osgEarth::Annotation::AnnotationLayer();
    mAreaLayer->setName(MEASUREAREA);
    return true;
}

//bool DrawPolygon::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//{
//    if (mEnterPolygonZone){
//        if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
//        {
//            if(mDrawingState == DrawingState::START)
//            {
//                startDraw(ea);
//                return true;
//            }

//            if(mDrawingState == DrawingState::DRAWING)
//            {
//                drawing(ea);
//                return true;
//            }
//        }
//        //cancel
//        if(ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
//        {
//            if(mDrawingState == DrawingState::DRAWING)
//            {
//                cancelDraw();
//                return true;
//            }
//        }
//        //finish
//        if(ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
//        {
//            if(mDrawingState == DrawingState::DRAWING)
//            {
//                finishDraw();
//                return true;
//            }
//        }
//    }
//    return false;
//}

//bool DrawPolygon::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//{
//    if (mEnterPolygonZone){
//        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
//        mIconNode->setPosition(geoPos);
//        if (mDrawingState == DrawingState::DRAWING){
//            mouseMoveDrawing(ea);
//        }
//    }
//    return false;
//}

void DrawPolygon::onPolygonItemCheck(bool check)
{
    if (check) {
        if(mPolygonLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mPolygonLayer);
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->setParentLayer(mPolygonLayer, shapeLayer);
        }
        mType = Type::POLYGONN;
        setState(State::READY);


        mPolygonProperties = new PolygonProperties(qmlEngine(), uiHandle(), mapItem());
        mPolygonProperties->show();
        mapItem()->addNode(iconNode());
    }
    else {
        if(mPolygonLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->setParentLayer(mPolygonLayer, nullptr);
            mapItem()->getMapObject()->removeLayer(mPolygonLayer);
        }
        if(state() == State::DRAWING)
            cancelDraw();
        setState(State::NONE);
        mPolygonProperties->deleteLater();
        mType = Type::NONE;
        mPolygon = nullptr;
        mPolygonProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawPolygon::onMeasureAreaItemCheck(bool check)
{
    if(check)
    {
        onPolygonItemCheck(true);
        mType = Type::AREA;
        if(mAreaLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mAreaLayer);
            auto measureLayer = DrawShape::measureLayer();
            mapItem()->getMapObject()->setParentLayer(mAreaLayer, measureLayer);
        }
        makeIconNode("../data/images/draw/polygon.png");

    }
    else
    {
        if(mAreaLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->setParentLayer(mAreaLayer, nullptr);
            mapItem()->getMapObject()->removeLayer(mAreaLayer);
        }
        mType = Type::NONE;
        mPolygonProperties->deleteLater();
        mPolygon = nullptr;
        mPolygonProperties->hide();
        mapItem()->removeNode(iconNode());
    }

//    if(check){
//        onPolygonItemCheck(true);
////        mShowArea = true;

//    }
//    else {
////        mDrawingState = DrawingState::FINISH;
//        if(mPolygonProperties){
//            mPolygonProperties->hide();
//        }

//        mPolygonProperties->deleteLater();
//        mPolygonProperties = nullptr;
//        mPolygon = nullptr;
//    }
}

void DrawPolygon::initDraw(const osgEarth::GeoPoint &geoPos)
{
    QString name;
    mPolygon = new Polygon(mapItem());
    if (mType == Type::AREA){
        name = MEASUREAREA + QString::number(mCount);
        mapItem()->getMapObject()->addNodeToLayer(mPolygon, mAreaLayer);
        mPolygonProperties->setPolygon(mPolygon);
        mPolygon->setName(name.toStdString());
        mPolygon->setShowArea(true);
        mPolygon->clearPoints();
    }
    else{
        name = POLYGON + QString::number(mCount);
        mapItem()->getMapObject()->addNodeToLayer(mPolygon, mPolygonLayer);
        mPolygonProperties->setPolygon(mPolygon);
        mPolygon->setName(name.toStdString());
    }
    setState(State::DRAWING);
    mCount++;
}

void DrawPolygon::tempDrawing(const osgEarth::GeoPoint &geoPos)
{
    if (mPolygon->getSize() >= 2)
    {
        mPolygon->removePoint();

    }
    mPolygon->addPoints(geoPos);

}

void DrawPolygon::drawing(const osgEarth::GeoPoint &geoPos)
{
    mPolygon->addPoints(geoPos);
}


//void DrawPolygon::init(const osgGA::GUIEventAdapter &event)
//{
//    mPolygon = new Polygon(mapItem());
//    if (mShowArea){
//        mPolygon->setShowArea(true);
//        mPolygon->clearPoints();
//    }
////    mapItem()->getMapObject()->addNodeToLayer(mPolygon, DRAW_LAYER_NAME);
//    mDrawingState = DrawingState::DRAWING;
//    mPolygonProperties->setPolygon(mPolygon);
//}
//bool DrawPolygon::drawing(const osgGA::GUIEventAdapter &event)
//{
//    osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(event.getX(), event.getY());
//    mPolygon->addPoints(geoPos);
//    return false;
//}

void DrawPolygon::cancelDraw()
{
    if(state() == State::DRAWING){
        mapItem()->getMapObject()->removeNodeFromLayer(mPolygon, mPolygonLayer);
        mapItem()->getMapObject()->removeNodeFromLayer(mPolygon, mAreaLayer);
        mPolygon = nullptr;
        mPolygonProperties->setPolygon(mPolygon);
        setState(State::READY);
        mCount--;
    }

//    mDrawingState = DrawingState::START;
//    if(mPolygonProperties)
//        mPolygonProperties->setPolygon(nullptr);
}

//void DrawPolygon::finishDraw()
//{
//    if(mDrawingState == DrawingState::DRAWING)
//    {
//        mDrawingState = DrawingState::START;
//    }
//}
//bool DrawPolygon::mouseMoveDrawing(const osgGA::GUIEventAdapter &event)
//{
//    if (mPolygon->getSize() >= 2)
//    {
//        mPolygon->removePoint();

//    }
//    osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(event.getX(), event.getY());
//    mPolygon->addPoints(geoPos);

//    return false;

//}
