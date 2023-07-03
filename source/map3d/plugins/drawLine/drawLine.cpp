#include "drawLine.h"
#include <osgEarthSymbology/Style>
#include <osgEarth/ModelLayer>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/feature_wfs/WFSFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <QLabel>
#include <QLineEdit>
#include <osgEarth/GLUtils>
#include <osgViewer/Viewer>
#include <osgEarth/GLUtils>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "mapItem.h"
#include "osgEarth/Layer"
#include <QQuickItem>
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarth/GeoMath>

//using namespace osgEarth::Annotation;
int DrawLine::mCount{0};
DrawLine::DrawLine(QWidget *parent)
    : DrawShape(parent)
{
    Q_INIT_RESOURCE(drawLine);
    //    Q_INIT_RESOURCE(LineProperties);
//    mEnterLineZone = false;
    qmlRegisterType<LinePropertiesModel>("Crystal", 1, 0, "LineProperties");
}

bool DrawLine::setup()
{   
    auto toolboxItemLine =  new ToolboxItem{POLYLINE, CATEGORY, "qrc:/resources/line.png", true};
    QObject::connect(toolboxItemLine, &ToolboxItem::itemChecked, this, &DrawLine::onLineItemCheck);
    toolbox()->addItem(toolboxItemLine);

    auto toolboxItemRuler =  new ToolboxItem{RULER, M_CATEGORY, "qrc:/resources/ruler.png", true};
    QObject::connect(toolboxItemRuler, &ToolboxItem::itemChecked, this, &DrawLine::onRulerItemCheck);
    toolbox()->addItem(toolboxItemRuler);

    auto toolboxItemHeight =  new ToolboxItem{MEASUREHEIGHT, M_CATEGORY, "qrc:/resources/height.png", true};
    QObject::connect(toolboxItemHeight, &ToolboxItem::itemChecked, this, &DrawLine::onHeightItemCheck);
    toolbox()->addItem(toolboxItemHeight);

    auto toolboxItemSlope =  new ToolboxItem{SLOPE, M_CATEGORY, "qrc:/resources/slope.png", true};
    QObject::connect(toolboxItemSlope, &ToolboxItem::itemChecked, this, &DrawLine::onSlopeItemCheck);
    toolbox()->addItem(toolboxItemSlope);

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());



    //    addLayer();
    mLineLayer = new osgEarth::Annotation::AnnotationLayer();
    mLineLayer->setName(POLYLINE);

    mRulerLayer = new osgEarth::Annotation::AnnotationLayer();
    mRulerLayer->setName(RULER);

    mHeightLayer = new osgEarth::Annotation::AnnotationLayer();
    mHeightLayer->setName(MEASUREHEIGHT);

    mSlopeLayer = new osgEarth::Annotation::AnnotationLayer();
    mSlopeLayer->setName(SLOPE);
    return true;
}
//bool DrawLine::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//{
//    if (mEnterLineZone){
//        if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
//        {
//            if(mDrawingState == DrawingState::START && mType != Type::HEIGHT)
//            {
//                startDrawLine();
//                return true;
//            }
//            if(mDrawingState == DrawingState::DRAWING && mType != Type::HEIGHT)
//            {
//                if ((mType == Type::RULER || mType == Type::SLOPE)  && mLine->getSize()>= 2){
//                    finishDrawing(ea);
//                    //mDrawingState = DrawingState::START;
//                }
//                else
//                    drawingLine(ea);
//                    return true;
//            }

//            //height part
//            if(mDrawingState == DrawingState::START && mType == Type::HEIGHT){
//                startDrawMeasureHeight();
//                return true;
//            }
//            if(mDrawingState == DrawingState::DRAWING && mType == Type::HEIGHT)
//            {
//                if (mType == Type::HEIGHT && mMeasureHeight->started() ){
//                    finishDrawing(ea);
//                }
//                else
//                    drawingMeasureHeight(ea);
//                return true;
//            }
//        }
//        else if(ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON && mDrawingState == DrawingState::DRAWING)
//        {
//            cancelDrawingLine(ea);
//        }
//        else if(ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON && mDrawingState == DrawingState::DRAWING)
//        {
//            finishDrawing(ea);
//            return true;
//        }
//    }
//    return false;
//}
//bool DrawLine::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//{
//    if (mEnterLineZone){
//        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
//        mIconNode->setPosition(geoPos);

//        if (mDrawingState == DrawingState::DRAWING && mType!=Type::HEIGHT){
//            mouseMoveDrawing(ea);
//        }
//        else if (mDrawingState == DrawingState::DRAWING && mType==Type::HEIGHT){
//            mouseMoveMeasureHeightDrawing(ea);
//        }
//    }
//    return false;
//}

void DrawLine::onLineItemCheck(bool check)
{
//    auto layer = mapItem()->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
//    if (!layer) {
//        osgEarth::ModelLayer *lineLayer = new osgEarth::ModelLayer();
//        lineLayer->setName(DRAW_LAYER_NAME);
//        mapItem()->addLayer(lineLayer);

//    }
//    if(check)
//    {
//        mEnterLineZone = true;
//        mType = Type::LINE;
//        mDrawingState = DrawingState::START;
//        mLineProperties = new LineProperties(qmlEngine(),uiHandle());
//        mLineProperties->setIsRuler(0);
//        mLineProperties->show();
//        mIconNode = makeIconNode();
//        mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);
//    }
//    else
//    {
//        mEnterLineZone = false;
//        mType = Type::NONE;
//        mDrawingState = DrawingState::FINISH;
//        if(mLineProperties){
//            mLineProperties->hide();
//        }

//        mLineProperties->deleteLater();
//        mLineProperties = nullptr;
//        mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
//    }

    if (check) {
        if(mLineLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mLineLayer);
            auto shapeLayer = DrawShape::shapeLayer();
            mapItem()->getMapObject()->setParentLayer(mLineLayer, shapeLayer);
        }
        mType = Type::LINE;
        setState(State::READY);
        mLineProperties = new LineProperties( qmlEngine(), uiHandle(), mapItem());
        mLineProperties->setIsRuler(0);
        mLineProperties->show();
        mapItem()->addNode(iconNode());

    }
    else {
        if(mLineLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->setParentLayer(mLineLayer, nullptr);
            mapItem()->getMapObject()->removeLayer(mLineLayer);
        }
        if(state() == State::DRAWING)
            cancelDraw();
        setState(State::NONE);
        mLineProperties->deleteLater();
        mType = Type::NONE;
        mLine = nullptr;
        mLineProperties->hide();
        mapItem()->removeNode(iconNode());
    }


}

void DrawLine::onRulerItemCheck(bool check)
{
    if(check)
    {
        if(mRulerLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mRulerLayer);
            auto measureLayer = DrawShape::measureLayer();
            mapItem()->getMapObject()->setParentLayer(mRulerLayer, measureLayer);
        }
        makeIconNode("../data/images/draw/ruler.png");
        setState(State::READY);
        mType = Type::RULERR;
        mLineProperties = new LineProperties(qmlEngine(),uiHandle() );
        mLineProperties->setIsRuler(1);
        mLineProperties->show();
        mapItem()->addNode(iconNode());
    }
    else
    {
        if(mRulerLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->setParentLayer(mRulerLayer, nullptr);
            mapItem()->getMapObject()->removeLayer(mRulerLayer);
        }
        if(state() == State::DRAWING)
            cancelDraw();
        setState(State::NONE);
        mType = Type::NONE;
        mLineProperties->deleteLater();
        mLine = nullptr;
        mLineProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawLine::onHeightItemCheck(bool check)
{
    if(check)
    {
        if(mHeightLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mHeightLayer);
            auto measureLayer = DrawShape::measureLayer();
            mapItem()->getMapObject()->setParentLayer(mHeightLayer, measureLayer);
        }
        makeIconNode("../data/images/draw/height.png");
        setState(State::READY);
        mType = Type::HEIGHT;
        mLineProperties = new LineProperties(qmlEngine(),uiHandle() );
        mLineProperties->setIsRuler(2);
        mLineProperties->show();
        mapItem()->addNode(iconNode());
    }
    else
    {
        if(mHeightLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->setParentLayer(mHeightLayer, nullptr);
            mapItem()->getMapObject()->removeLayer(mHeightLayer);
        }
        if(state() == State::DRAWING)
            cancelDraw();
        setState(State::NONE);
        mType = Type::NONE;
        mLineProperties->deleteLater();
        mLine = nullptr;
        mLineProperties->hide();
        mapItem()->removeNode(iconNode());
    }





//    if(check)
//    {
//        mEnterLineZone = true;
//        mType = Type::HEIGHT;
//        mDrawingState = DrawingState::START;
//        mLineProperties = new LineProperties(qmlEngine(),uiHandle() );
//        mLineProperties->setIsRuler(2);
//        mLineProperties->show();
//        mIconNode = makeIconNode();
//        mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);
//    }
//    else
//    {
//        mEnterLineZone = false;
//        mType = Type::NONE;
//        mDrawingState = DrawingState::FINISH;
//        if(mLineProperties){
//            mLineProperties->hide();
//        }
//        mLineProperties->deleteLater();
//        mLineProperties = nullptr;
//        mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
//    }
}

void DrawLine::onSlopeItemCheck(bool check)
{
    if(check)
    {
        if(mSlopeLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mSlopeLayer);
            auto measureLayer = DrawShape::measureLayer();
            mapItem()->getMapObject()->setParentLayer(mSlopeLayer, measureLayer);
        }
        makeIconNode("../data/images/draw/slope.png");
        setState(State::READY);
        mType = Type::SLOPEE;
        mLineProperties = new LineProperties(qmlEngine(),uiHandle() );
        mLineProperties->setIsRuler(3);
        mLineProperties->show();
        mapItem()->addNode(iconNode());
    }
    else
    {
        if(mSlopeLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->setParentLayer(mSlopeLayer, nullptr);
            mapItem()->getMapObject()->removeLayer(mSlopeLayer);
        }
        if(state() == State::DRAWING)
            cancelDraw();
        setState(State::NONE);
        mType = Type::NONE;
        mLineProperties->deleteLater();
        mLine = nullptr;
        mLineProperties->hide();
        mapItem()->removeNode(iconNode());
    }


//    auto layer = mapItem()->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
//    if (!layer) {
//        osgEarth::ModelLayer *lineLayer = new osgEarth::ModelLayer();
//        lineLayer->setName(DRAW_LAYER_NAME);
//        mapItem()->addLayer(lineLayer);

//    }
//    if(check)
//    {
//        mEnterLineZone = true;
//        mType = Type::SLOPE;
//        mDrawingState = DrawingState::START;
//        mLineProperties = new LineProperties(qmlEngine(),uiHandle() );
//        mLineProperties->setIsRuler(3);
//        mLineProperties->show();
//        mIconNode = makeIconNode();
//        mapItem()->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

//    }
//    else
//    {
//        mEnterLineZone = false;
//        mType = Type::NONE;
//        mDrawingState = DrawingState::FINISH;
//        if(mLineProperties){
//            mLineProperties->hide();
//        }
//        mLineProperties->deleteLater();
//        mLineProperties = nullptr;
//        mapItem()->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
//    }
}

//void DrawLine::mouseDoubleClickEvent(QMouseEvent */*event*/)
//{
//    //    finishDrawing(event);
//}


void DrawLine::initDraw(const osgEarth::GeoPoint &geoPos)
{
//    QString name = "PolyLine" + QString::number(mCount);
    mLine = new LineNode(mapItem());
    QString name;
    switch (mType) {
    case Type::LINE:
        name = POLYLINE + QString::number(mCount);
        mapItem()->getMapObject()->addNodeToLayer(mLine, mLineLayer);
        mLineProperties->setLine(mLine);
        mLine->setName(name.toStdString());
        mLine->addPoint(geoPos);
        break;
    case Type::RULERR:
        name = RULER + QString::number(mCount);
        mapItem()->getMapObject()->addNodeToLayer(mLine, mRulerLayer);
        mLineProperties->setLine(mLine);
        mLine->setName(name.toStdString());
        mLine->addPoint(geoPos);
        break;
    case Type::HEIGHT:
        mMeasureHeight = new MeasureHeight(mapItem());
        mLineProperties->setMeasureHeight(mMeasureHeight);
        name = MEASUREHEIGHT + QString::number(mCount);
        mapItem()->getMapObject()->addNodeToLayer(mMeasureHeight, mHeightLayer);
        mMeasureHeight->setName(name.toStdString());
        mMeasureHeight->setFirstPoint(geoPos);
        break;
    case Type::SLOPEE:
        name = SLOPE + QString::number(mCount);
        mapItem()->getMapObject()->addNodeToLayer(mLine, mSlopeLayer);
        mLineProperties->setLine(mLine);
        mLine->setName(name.toStdString());
        mLine->addPoint(geoPos);
        break;
    default:
        name = POLYLINE + QString::number(mCount);
        break;
    }
    setState(State::DRAWING);
    mCount++;
}

void DrawLine::tempDrawing(const osgEarth::GeoPoint &geoPos)
{
    if (mLine->getSize() >= 2)
    {
        mLine->removePoint();
    }
    if (mType == Type::HEIGHT){
        mMeasureHeight->clear();
        mMeasureHeight->setSecondPoint(geoPos);
    }
    mLine->addPoint(geoPos);
}

void DrawLine::drawing(const osgEarth::GeoPoint &geoPos)
{
    if(mType == Type::RULERR || mType == Type::SLOPEE  && mLine->getSize()>=2){
        mLine->removePoint();
        confirmDraw();
    }

        mLine->addPoint(geoPos);

//    qDebug()<<"size is: "<<mLine->getSize();
}

void DrawLine::cancelDraw()
{
    if(state() == State::DRAWING){
        mapItem()->getMapObject()->removeNodeFromLayer(mLine, mLineLayer);
        mapItem()->getMapObject()->removeNodeFromLayer(mLine, mRulerLayer);
        mapItem()->getMapObject()->removeNodeFromLayer(mLine, mSlopeLayer);
        mapItem()->getMapObject()->removeNodeFromLayer(mMeasureHeight, mHeightLayer);
        mLine = nullptr;
        mMeasureHeight = nullptr;
        mLineProperties->setLine(mLine);
        mLineProperties->setMeasureHeight(mMeasureHeight);
        setState(State::READY);
        mCount--;
    }
}
//void DrawLine::startDrawLine()
//{
//    mLine = new LineNode(mapItem());
//    mapItem()->addNodeToLayer(mLine, DRAW_LAYER_NAME);
//    mLineProperties->setLine(mLine);
//    mDrawingState = DrawingState::DRAWING;
//}

//void DrawLine::startDrawMeasureHeight()
//{
//    mMeasureHeight = new MeasureHeight(mapItem());
//    mapItem()->addNodeToLayer(mMeasureHeight, DRAW_LAYER_NAME);
//    mLineProperties->setMeasureHeight(mMeasureHeight);
//    mDrawingState = DrawingState::DRAWING;
//}

//bool DrawLine::drawingMeasureHeight(const osgGA::GUIEventAdapter &event)
//{
//    mMeasureHeight->setFirstPoint(mapItem()->screenToGeoPoint(event.getX(), event.getY()));
//    return false;
//}

//bool DrawLine::mouseMoveMeasureHeightDrawing(const osgGA::GUIEventAdapter &event)
//{
//    mMeasureHeight->clear();
//    mMeasureHeight->setSecondPoint(mapItem()->screenToGeoPoint(event.getX(), event.getY()));
//    return false;
//}

//bool DrawLine::drawingLine(const osgGA::GUIEventAdapter &event)
//{
//        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(event.getX(), event.getY());
//        mLine->addPoint(geoPos);
//        return false;
//}

//bool DrawLine::cancelDrawingLine(const osgGA::GUIEventAdapter &event)
//{
//    mapItem()->removeNodeFromLayer(mLine, DRAW_LAYER_NAME);
//    mapItem()->removeNodeFromLayer(mMeasureHeight, DRAW_LAYER_NAME);
//    if(mLineProperties)
//        mLineProperties->setLine(nullptr);
//    return true;
//    mDrawingState = DrawingState::START;
//}

//bool DrawLine::mouseMoveDrawing(const osgGA::GUIEventAdapter &event)
//{

//    if (mLine->getSize() >= 2)
//    {
//        mLine->removePoint();
//    }
//    osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(event.getX(), event.getY());
//    mLine->addPoint(geoPos);
//    return false;

//}

//bool DrawLine::finishDrawing(const osgGA::GUIEventAdapter &event, osg::Node *nodeEditor)
//{
//    if(mDrawingState == DrawingState::DRAWING)
//    {
//        mDrawingState = DrawingState::START;
//        if(nodeEditor)
//            mapItem()->removeNodeFromLayer(nodeEditor, DRAW_LAYER_NAME);
//        return true;
//    }
//    return false;
//}

//PlaceNode *DrawLine::makeIconNode()
//{
//    switch(mType) {
//    case Type::LINE:
//        mIcon = osgDB::readImageFile("../data/images/draw/line.png");
//        break;
//    case Type::RULER:
//        mIcon = osgDB::readImageFile("../data/images/draw/ruler.png");
//        break;
//    case Type::HEIGHT:
//        mIcon = osgDB::readImageFile("../data/images/draw/height.png");
//        break;
//    case Type::SLOPE:
//        mIcon = osgDB::readImageFile("../data/images/draw/slope.png");
//        break;
//    case Type::NONE:
//        mIcon = nullptr;
//        break;
//    //default:
//        //mIcon = osgDB::readImageFile("../data/images/draw/line.png");
//    }

//    mIcon->scaleImage(24, 24, mIcon->r());
//    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
//    model->setIconImage(mIcon);
//    return model.release();
//}
