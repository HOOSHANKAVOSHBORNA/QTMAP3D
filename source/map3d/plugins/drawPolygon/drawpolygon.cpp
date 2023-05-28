#include "drawpolygon.h"

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <osgEarth/GLUtils>
#include "osgEarth/ModelLayer"

const QString CATEGORY = "Draw";
const QString POLYGON = "Polygon";

DrawPolygon::DrawPolygon(QObject *parent)
    : PluginInterface(parent)
{

}

bool DrawPolygon::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    qmlRegisterType<PolygonPropertiesModel>("Crystal", 1, 0, "PolygonProperties");
    mQmlEngine = engine;
    desc->toolboxItemsList.push_back(new ItemDesc{POLYGON, CATEGORY, "qrc:/resources/polygon.png", true,  false, ""});
    return true;
}

void DrawPolygon::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{
    if (CATEGORY == category) {
        if (name == POLYGON) {
            if (checked) {
                mEnterPolygonZone = true;
                mPolygonProperties = new PolygonProperties(mQmlEngine, mUiHandle);
                if(mUiHandle && mPolygonProperties){
                    mPolygonProperties->show();
                }
                mDrawingState = DrawingState::START;
                mMapItem->addNodeToLayer(mIconNode, DRAW_LAYER_NAME);

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
                mMapItem->removeNodeFromLayer(mIconNode, DRAW_LAYER_NAME);
            }
        }
    }
}

bool DrawPolygon::setup(MapItem *mapItem, UIHandle *uIHandle)
{
    mUiHandle = uIHandle;
    mMapItem = mapItem;
    osgEarth::GLUtils::setGlobalDefaults(mMapItem->getViewer()->getCamera()->getOrCreateStateSet());
    mIconNode = makeIconNode();
    osgEarth::ModelLayer *polygonLayer = new osgEarth::ModelLayer();
    polygonLayer->setName(DRAW_LAYER_NAME);
    mMapItem->addLayer(polygonLayer);
    return true;
}

void DrawPolygon::mousePressEvent(QMouseEvent *event)
{
    if (mEnterPolygonZone){
        if(event->button() == Qt::MouseButton::LeftButton)
        {
            if(mDrawingState == DrawingState::START)
            {
                startDraw(event);
                event->accept();
            }

            if(mDrawingState == DrawingState::DRAWING)
            {
                drawing(event);
                event->accept();
            }
        }
        //cancel
        if(event->button() == Qt::MouseButton::RightButton)
        {
            if(mDrawingState == DrawingState::DRAWING)
            {
                cancelDraw();
                event->accept();
            }
        }
        //finish
        if(event->button() == Qt::MouseButton::MiddleButton)
        {
            if(mDrawingState == DrawingState::DRAWING)
            {
                finishDraw();
                event->accept();
            }
        }
    }
}

void DrawPolygon::mouseMoveEvent(QMouseEvent *event)
{
    if (mEnterPolygonZone){
        osgEarth::GeoPoint geoPos = mMapItem->screenToGeoPoint(event->x(), event->y());
        mIconNode->setPosition(geoPos);
        if (mDrawingState == DrawingState::DRAWING){
            mouseMoveDrawing(event);
        }
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

void DrawPolygon::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void DrawPolygon::startDraw(QMouseEvent *event)
{
    mPolygon = new Polygon(mMapItem);
    mMapItem->addNodeToLayer(mPolygon, DRAW_LAYER_NAME);
    mDrawingState = DrawingState::DRAWING;
    mPolygonProperties->setPolygon(mPolygon);
}
void DrawPolygon::drawing(QMouseEvent *event)
{
    osgEarth::GeoPoint geoPos = mMapItem->screenToGeoPoint(event->x(), event->y());
    mPolygon->addPoints(geoPos);
}

void DrawPolygon::cancelDraw()
{
    mMapItem->removeNodeFromLayer(mPolygon, DRAW_LAYER_NAME);
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
void DrawPolygon::mouseMoveDrawing(QMouseEvent *event)
{
    if (mPolygon->getSize() >= 2)
    {
        mPolygon->removePoint();

    }
    osgEarth::GeoPoint geoPos = mMapItem->screenToGeoPoint(event->x(), event->y());
    mPolygon->addPoints(geoPos);
}
