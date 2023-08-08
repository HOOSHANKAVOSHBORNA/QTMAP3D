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
}

bool DrawLine::setup()
{
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

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

    mLineLayer = new ParenticAnnotationLayer();
    mLineLayer->setName(POLYLINE);

    mRulerLayer = new ParenticAnnotationLayer();
    mRulerLayer->setName(RULER);

    mHeightLayer = new ParenticAnnotationLayer();
    mHeightLayer->setName(MEASUREHEIGHT);

    mSlopeLayer = new ParenticAnnotationLayer();
    mSlopeLayer->setName(SLOPE);
    return true;
}

void DrawLine::onLineItemCheck(bool check)
{
    if (check) {
        mType = Type::LINE;
        setState(State::READY);
        mLineProperty = new LineProperty();
        createProperty("Line", QVariant::fromValue<LineProperty*>(mLineProperty));
        mapItem()->addNode(iconNode());
    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        setState(State::NONE);
        mType = Type::NONE;
        mLine = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}
void DrawLine::onRulerItemCheck(bool check)
{
    auto measureLayer = DrawShape::measureLayer();
    if(check)
    {
        if(mRulerLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mRulerLayer, measureLayer);
        }
        makeIconNode("../data/images/draw/ruler.png");
        setState(State::READY);
        mType = Type::RULERR;
//        mLineProperties = new LineProperties(qmlEngine(),uiHandle() );
//        mLineProperties->setIsRuler(1);
//        mLineProperties->show();
        mapItem()->addNode(iconNode());
    }
    else
    {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mRulerLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->removeLayer(mRulerLayer, measureLayer);
        }
        setState(State::NONE);
        mType = Type::NONE;
//        mLineProperties->deleteLater();
        mLine = nullptr;
//        mLineProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawLine::onHeightItemCheck(bool check)
{
    auto measureLayer = DrawShape::measureLayer();
    if(check)
    {
        if(mHeightLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mHeightLayer, measureLayer);
        }
        makeIconNode("../data/images/draw/height.png");
        setState(State::READY);
        mType = Type::HEIGHT;
//        mLineProperties = new LineProperties(qmlEngine(),uiHandle() );
//        mLineProperties->setIsRuler(2);
//        mLineProperties->show();
        mapItem()->addNode(iconNode());
    }
    else
    {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mHeightLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->removeLayer(mHeightLayer, measureLayer);
        }
        setState(State::NONE);
        mType = Type::NONE;
//        mLineProperties->deleteLater();
        mLine = nullptr;
//        mLineProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawLine::onSlopeItemCheck(bool check)
{
    auto measureLayer = DrawShape::measureLayer();
    if(check)
    {
        if(mSlopeLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->addLayer(mSlopeLayer, measureLayer);
        }
        makeIconNode("../data/images/draw/slope.png");
        setState(State::READY);
        mType = Type::SLOPEE;
//        mLineProperties = new LineProperties(qmlEngine(),uiHandle() );
//        mLineProperties->setIsRuler(3);
//        mLineProperties->show();
        mapItem()->addNode(iconNode());
    }
    else
    {
        if(state() == State::DRAWING)
            cancelDraw();

        if(mSlopeLayer->getGroup()->getNumChildren() <= 0){
            mapItem()->getMapObject()->removeLayer(mSlopeLayer, measureLayer);
        }
        setState(State::NONE);
        mType = Type::NONE;
//        mLineProperties->deleteLater();
        mLine = nullptr;
//        mLineProperties->hide();
        mapItem()->removeNode(iconNode());
    }
}

void DrawLine::initDraw(const osgEarth::GeoPoint &geoPos)
{

    mLine = new LineNode(mapItem());
    QString name;
    auto shapeLayer = DrawShape::shapeLayer();
    switch (mType) {
    case Type::LINE:
        name = POLYLINE + QString::number(mCount);
        mLine->setName(name.toStdString());
        mLineProperty->setLine(mLine);
        mLine->addPoint(geoPos);

        if(!shapeLayer->containsLayer(mLineLayer)){
            mLineLayer->clear();
            shapeLayer->addLayer(mLineLayer);
        }
        mLineLayer->addChild(mLine);
        break;
    case Type::RULERR:
        name = RULER + QString::number(mCount);
        mLine->setName(name.toStdString());
//        mapItem()->getMapObject()->addNodeToLayer(mLine, mRulerLayer);
        mLineProperty->setLine(mLine);
        mLine->addPoint(geoPos);
        break;
    case Type::HEIGHT:
        mMeasureHeight = new MeasureHeight(mapItem());
//        mLineProperties->setMeasureHeight(mMeasureHeight);
        name = MEASUREHEIGHT + QString::number(mCount);
        mMeasureHeight->setName(name.toStdString());
//        mapItem()->getMapObject()->addNodeToLayer(mMeasureHeight, mHeightLayer);
        mMeasureHeight->setFirstPoint(geoPos);
        break;
    case Type::SLOPEE:
        name = SLOPE + QString::number(mCount);
        mLine->setName(name.toStdString());
//        mapItem()->getMapObject()->addNodeToLayer(mLine, mSlopeLayer);
        mLineProperty->setLine(mLine);
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
    if (mLine->getSize() > 1)
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
//        mLine->removePoint();
//        confirmDraw();
        return;
    }
        mLine->addPoint(geoPos);
}

void DrawLine::cancelDraw()
{
    if(state() == State::DRAWING){
        mLineLayer->removeChild(mLine);
        mLine = nullptr;
        mMeasureHeight = nullptr;
        mLineProperty->setLine(mLine);
//        mLineProperties->setMeasureHeight(mMeasureHeight);
        setState(State::READY);
        mCount--;

        if(!mLineLayer->hasNode())
            DrawShape::shapeLayer()->removeLayer(mLineLayer);
    }
}
