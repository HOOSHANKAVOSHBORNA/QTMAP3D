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
    makeIconNode("../data/images/draw/line.png");
    onItemChecked(Type::LINE, check);
}
void DrawLine::onRulerItemCheck(bool check)
{
    makeIconNode("../data/images/draw/ruler.png");
    onItemChecked(Type::RULERR, check);
}

void DrawLine::onHeightItemCheck(bool check)
{
    makeIconNode("../data/images/draw/height.png");
    onItemChecked(Type::HEIGHT, check);
}

void DrawLine::onSlopeItemCheck(bool check)
{
    makeIconNode("../data/images/draw/slope.png");
    onItemChecked(Type::SLOPEE, check);
}

void DrawLine::onItemChecked(Type type, bool check)
{
    if (check) {
        mType = type;
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

void DrawLine::initDraw(const osgEarth::GeoPoint &geoPos)
{

    mLine = new LineNode(mapItem());
    QString name;
    auto shapeLayer = DrawShape::shapeLayer();
    auto measureLayer = DrawShape::measureLayer();
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
        mLineProperty->setRuler(mLine);
        mLine->addPoint(geoPos);

        if(!measureLayer->containsLayer(mRulerLayer)){
            mRulerLayer->clear();
            measureLayer->addLayer(mRulerLayer);
        }
        mRulerLayer->addChild(mLine);
        break;
    case Type::HEIGHT:
        name = MEASUREHEIGHT + QString::number(mCount);
        mLine->setName(name.toStdString());
        mLineProperty->setMeasureHeight(mLine);
        mLine->addPoint(geoPos);

        if(!measureLayer->containsLayer(mHeightLayer)){
            mHeightLayer->clear();
            measureLayer->addLayer(mHeightLayer);
        }
        mHeightLayer->addChild(mLine);
        break;
    case Type::SLOPEE:
        name = SLOPE + QString::number(mCount);
        mLine->setName(name.toStdString());
        mLineProperty->setMesureSlope(mLine);
        mLine->addPoint(geoPos);

        if(!measureLayer->containsLayer(mSlopeLayer)){
            mSlopeLayer->clear();
            measureLayer->addLayer(mSlopeLayer);
        }
        mSlopeLayer->addChild(mLine);

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
        //        mMeasureHeight->clear();
        //        mMeasureHeight->setSecondPoint(geoPos);
        if (mLine->getSize() > 1)
        {
            mLine->removePoint();
        }
        auto firstPoint = mLine->getPoint(0);
        double h = geoPos.z() - firstPoint.z();
        osgEarth::GeoPoint midPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y(), firstPoint.z());
        if(h > 0)
            midPoint.set(mapItem()->getMapSRS(), firstPoint.x(), firstPoint.y(), geoPos.z(),osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
        mLine->addPoint(midPoint);
    }
    mLine->addPoint(geoPos);
}

void DrawLine::drawing(const osgEarth::GeoPoint &geoPos)
{
    if(mType != Type::LINE || mType == Type::SLOPEE  && mLine->getSize()>=2){
        //        mLine->removePoint();
        //        confirmDraw();
        return;
    }
    mLine->addPoint(geoPos);
}

void DrawLine::cancelDraw()
{
    if(state() == State::DRAWING){
        switch (mType) {
        case Type::LINE:
            mLineLayer->removeChild(mLine);
            break;
        case Type::RULERR:
            mRulerLayer->removeChild(mLine);
            break;
        case Type::HEIGHT:
            mHeightLayer->removeChild(mLine);
            break;
        case Type::SLOPEE:
            mSlopeLayer->removeChild(mLine);
            break;
        }
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
