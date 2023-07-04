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

int DrawLine::mCount{0};
DrawLine::DrawLine(QWidget *parent)
    : DrawShape(parent)
{
    Q_INIT_RESOURCE(drawLine);
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
void DrawLine::onLineItemCheck(bool check)
{
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
}

void DrawLine::initDraw(const osgEarth::GeoPoint &geoPos)
{

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

