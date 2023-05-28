#include "drawline.h"
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
#include "osg/Group"
#include "osgEarth/ModelLayer"
#include "osgEarth/Layer"
#include <QQuickItem>
#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarth/GeoMath>


using namespace osgEarth::Annotation;

const QString CATEGORY = "Draw";
const QString LINE = "Line";

const QString M_CATEGORY = "Measurement";
const QString RULER = "Ruler";
const QString MEASUREHEIGHT = "Measure Height";
const QString SLOPE = "Slope";


drawLine::drawLine(QWidget *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(drawLine);
    //    Q_INIT_RESOURCE(LineProperties);
    mEnterLineZone = false;
}

bool drawLine::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<LinePropertiesModel>("Crystal", 1, 0, "LineProperties");
    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{LINE, CATEGORY, "qrc:/resources/line.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{RULER, M_CATEGORY, "qrc:/resources/ruler.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{MEASUREHEIGHT, M_CATEGORY, "qrc:/resources/height.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{SLOPE, M_CATEGORY, "qrc:/resources/slope.png", true});
    return true;
}

void drawLine::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(DRAW_LAYER_NAME);
    if (!layer) {
        osgEarth::ModelLayer *lineLayer = new osgEarth::ModelLayer();
        lineLayer->setName(DRAW_LAYER_NAME);
        mMapItem->addLayer(lineLayer);

    }

    if(CATEGORY == category)
        if(name == LINE)
        {
            if(checked)
            {
                mEnterLineZone = true;
                mType = Type::LINE;
                mDrawingState = DrawingState::START;
                mLineProperties = new LineProperties(mQmlEngine,muiHandle );
                mLineProperties->setIsRuler(0);
                mLineProperties->show();
                mIconNode = makeIconNode();
                mMapItem->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);
            }
            else
            {
                mEnterLineZone = false;
                mType = Type::NONE;
                mDrawingState = DrawingState::FINISH;
                if(mLineProperties){
                        mLineProperties->hide();
             }

                mLineProperties->deleteLater();
                mLineProperties = nullptr;
                mMapItem->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
            }
        }
    if(name == RULER)
    {
        if(checked)
        {
            mEnterLineZone = true;
            mType = Type::RULER;
            mDrawingState = DrawingState::START;
            mLineProperties = new LineProperties(mQmlEngine,muiHandle );
            mLineProperties->setIsRuler(1);
            mLineProperties->show();
            mIconNode = makeIconNode();
            mMapItem->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

        }
        else
        {
            mEnterLineZone = false;
            mType = Type::NONE;
            mDrawingState = DrawingState::FINISH;
            if(mLineProperties){
                    mLineProperties->hide();
         }
            mLineProperties->deleteLater();
            mLineProperties = nullptr;
            mMapItem->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
        }
    }

    if(name == SLOPE)
    {
        if(checked)
        {
            mEnterLineZone = true;
            mType = Type::SLOPE;
            mDrawingState = DrawingState::START;
            mLineProperties = new LineProperties(mQmlEngine,muiHandle );
            mLineProperties->setIsRuler(3);
            mLineProperties->show();
            mIconNode = makeIconNode();
            mMapItem->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

        }
        else
        {
            mEnterLineZone = false;
            mType = Type::NONE;
            mDrawingState = DrawingState::FINISH;
            if(mLineProperties){
                    mLineProperties->hide();
         }
            mLineProperties->deleteLater();
            mLineProperties = nullptr;
            mMapItem->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
        }
    }


    if(name == MEASUREHEIGHT)
    {
        if(checked)
        {
            mEnterLineZone = true;
            mType = Type::HEIGHT;
            mDrawingState = DrawingState::START;
            mLineProperties = new LineProperties(mQmlEngine,muiHandle );
            mLineProperties->setIsRuler(2);
            mLineProperties->show();
            mIconNode = makeIconNode();
            mMapItem->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);
        }
        else
        {
            mEnterLineZone = false;
            mType = Type::NONE;
            mDrawingState = DrawingState::FINISH;
            if(mLineProperties){
                    mLineProperties->hide();
         }
            mLineProperties->deleteLater();
            mLineProperties = nullptr;
            mMapItem->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
        }
    }
}
bool drawLine::setup(MapItem *mapItem, UIHandle *uIHandle)
{
    muiHandle = uIHandle;
    mMapItem = mapItem;
    osgEarth::GLUtils::setGlobalDefaults(mMapItem->getViewer()->getCamera()->getOrCreateStateSet());
    return true;
}
void drawLine::mousePressEvent(QMouseEvent *event)
{
    if (mEnterLineZone){
        if(event->button() == Qt::MouseButton::LeftButton)
        {
            if(mDrawingState == DrawingState::START && mType != Type::HEIGHT)
            {
                startDrawLine();
                event->accept();
            }
            if(mDrawingState == DrawingState::DRAWING && mType != Type::HEIGHT)
            {
                if ((mType == Type::RULER || mType == Type::SLOPE)  && mLine->getSize()>= 2){
                    finishDrawing(event);
                    //mDrawingState = DrawingState::START;
                }
                else
                    drawingLine(event);
                    event->accept();
            }

            //height part
            if(mDrawingState == DrawingState::START && mType == Type::HEIGHT){
                startDrawMeasureHeight();
                event->accept();
            }
            if(mDrawingState == DrawingState::DRAWING && mType == Type::HEIGHT)
            {
                if (mType == Type::HEIGHT && mMeasureHeight->started() ){
                    finishDrawing(event);
                }
                else
                    drawingMeasureHeight(event);
                event->accept();
            }
        }
        else if(event->button() == Qt::MouseButton::RightButton && mDrawingState == DrawingState::DRAWING)
        {
            cancelDrawingLine(event);
        }
        else if(event->button() == Qt::MouseButton::MiddleButton && mDrawingState == DrawingState::DRAWING)
        {
            finishDrawing(event);
            event->accept();
        }
    }
}
void drawLine::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterLineZone){
        osgEarth::GeoPoint geoPos = mMapItem->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);

        if (mDrawingState == DrawingState::DRAWING && mType!=Type::HEIGHT){
            mouseMoveDrawing(event);
        }
        else if (mDrawingState == DrawingState::DRAWING && mType==Type::HEIGHT){
            mouseMoveMeasureHeightDrawing(event);
        }
    }
}

void drawLine::mouseDoubleClickEvent(QMouseEvent */*event*/)
{
    //    finishDrawing(event);
}

void drawLine::startDrawLine()
{
    mLine = new LineNode(mMapItem);
    mMapItem->addNodeToLayer(mLine, DRAW_LAYER_NAME);
    mLineProperties->setLine(mLine);
    mDrawingState = DrawingState::DRAWING;
}

void drawLine::startDrawMeasureHeight()
{
    mMeasureHeight = new MeasureHeight(mMapItem);
    mMapItem->addNodeToLayer(mMeasureHeight, DRAW_LAYER_NAME);
    mLineProperties->setMeasureHeight(mMeasureHeight);
    mDrawingState = DrawingState::DRAWING;
}

void drawLine::drawingMeasureHeight(QMouseEvent *event)
{
    mMeasureHeight->setFirstPoint(mMapItem->screenToGeoPoint(event->x(), event->y()));
}

void drawLine::mouseMoveMeasureHeightDrawing(QMouseEvent *event)
{
    mMeasureHeight->clear();
    mMeasureHeight->setSecondPoint(mMapItem->screenToGeoPoint(event->x(), event->y()));
}

void drawLine::drawingLine(QMouseEvent *event)
{
        osgEarth::GeoPoint geoPos = mMapItem->screenToGeoPoint(event->x(), event->y());
        mLine->addPoint(geoPos);
}

void drawLine::cancelDrawingLine(QMouseEvent *event)
{
    mMapItem->removeNodeFromLayer(mLine, DRAW_LAYER_NAME);
    mMapItem->removeNodeFromLayer(mMeasureHeight, DRAW_LAYER_NAME);
    if(mLineProperties)
        mLineProperties->setLine(nullptr);
    event->accept();
    mDrawingState = DrawingState::START;
}

void drawLine::mouseMoveDrawing(QMouseEvent *event)
{

    if (mLine->getSize() >= 2)
    {
        mLine->removePoint();
    }
    osgEarth::GeoPoint geoPos = mMapItem->screenToGeoPoint(event->x(), event->y());
    mLine->addPoint(geoPos);

}

void drawLine::finishDrawing(QMouseEvent *event, osg::Node *nodeEditor)
{
    if(mDrawingState == DrawingState::DRAWING)
    {
        mDrawingState = DrawingState::START;
        if(nodeEditor)
            mMapItem->removeNodeFromLayer(nodeEditor, DRAW_LAYER_NAME);
        event->accept();
    }
}

PlaceNode *drawLine::makeIconNode()
{
    switch(mType) {
    case Type::LINE:
        mIcon = osgDB::readImageFile("../data/images/draw/line.png");
        break;
    case Type::RULER:
        mIcon = osgDB::readImageFile("../data/images/draw/ruler.png");
        break;
    case Type::HEIGHT:
        mIcon = osgDB::readImageFile("../data/images/draw/height.png");
        break;
    case Type::SLOPE:
        mIcon = osgDB::readImageFile("../data/images/draw/slope.png");
        break;
    case Type::NONE:
        mIcon = nullptr;
        break;
    //default:
        //mIcon = osgDB::readImageFile("../data/images/draw/line.png");
    }

    mIcon->scaleImage(24, 24, mIcon->r());
    osg::ref_ptr<osgEarth::Annotation::PlaceNode>  model = new osgEarth::Annotation::PlaceNode();
    model->setIconImage(mIcon);
    return model.release();
}
