#include "drawAnnotation.h"
#include "mainwindow.h"
#include "property.h"

#include <osgEarthSymbology/Style>
#include <osgEarth/ModelLayer>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/feature_wfs/WFSFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <osgEarth/GLUtils>
#include <osgViewer/Viewer>
#include <osgEarth/GLUtils>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "mapItem.h"
#include "osgEarth/Layer"
#include <QQuickItem>
#include <osgEarthAnnotation/AnnotationLayer>
#include <utility.h>

using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;

int DrawAnnotation::mCount{0};

DrawAnnotation::DrawAnnotation(QObject *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(drawAnnotation);
    Q_INIT_RESOURCE(drawAnnotationQml);
}

DrawAnnotation::~DrawAnnotation()
{
    mIconNode.release();
}

void DrawAnnotation::addUpdateLineNode(PolyLineData *lineNodeData)
{
    LineNode *lineNode;
    if (!mLineNodeMap.contains(lineNodeData->id)) {
        lineNode = new LineNode(mapItem());
        lineNodeData->layer->addChild(lineNode);
        mLineNodeMap[lineNodeData->id] = lineNode;
    }
    else {
        lineNode = mLineNodeMap[lineNodeData->id];
        lineNode->clear();
    }

    lineNode->create(&lineNodeData->points);
    lineNode->setFillColor(osgEarth::Color(lineNodeData->color));
    lineNode->setWidth(lineNodeData->width);
    lineNode->setPolyLineData(lineNodeData);
}

bool DrawAnnotation::setup()
{
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mShapeLayer = new CompositeAnnotationLayer();
    mShapeLayer->setName(CATEGORY);
    mapItem()->getMapObject()->addLayer(mShapeLayer);


    /***************************draw Line*******************************/

    connect(serviceManager(), &ServiceManager::lineNodeDataReceived, this, &DrawAnnotation::LineNodeDataReceived);
    connect(serviceManager(), &ServiceManager::polygonDataReceived, this, &DrawAnnotation::polygonDataReceived);
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    auto toolboxItemPolygon =  new ToolboxItem{POLYGON, CATEGORY, "qrc:/resources/polygon.png", true};
    QObject::connect(toolboxItemPolygon, &ToolboxItem::itemChecked, this, &DrawAnnotation::onPolygonItemCheck);
    toolbox()->addItem(toolboxItemPolygon);

    auto toolboxItemLine =  new ToolboxItem{POLYLINE, CATEGORY, "qrc:/resources/line.png", true};
    QObject::connect(toolboxItemLine, &ToolboxItem::itemChecked, this, &DrawAnnotation::onLineItemCheck);
    toolbox()->addItem(toolboxItemLine);

    mLineLayer = new ParenticAnnotationLayer();
    mLineLayer->setName(POLYLINE);

    mPolygonLayer = new ParenticAnnotationLayer();
    mPolygonLayer->setName(POLYGON);

    return true;
}

void DrawAnnotation::makeIconNode(const QString &fileName)
{
    osg::Image* icon = osgDB::readImageFile(fileName.toStdString());
    if(icon){
        icon->scaleImage(24, 24, icon->r());
        mIconNode = new osgEarth::Annotation::PlaceNode();
        mIconNode->setIconImage(icon);
    }
}

osgEarth::Annotation::PlaceNode *DrawAnnotation::iconNode() const
{
    return mIconNode.get();
}

DrawAnnotation::State DrawAnnotation::state() const
{
    return mState;
}

void DrawAnnotation::setState(DrawAnnotation::State newState)
{
    mState = newState;
}

CompositeAnnotationLayer *DrawAnnotation::shapeLayer()
{
    if(!mShapeLayer)
        mShapeLayer = dynamic_cast<CompositeAnnotationLayer*>(mapItem()->getMapObject()->getLayerByName(CATEGORY));
    return mShapeLayer;
}



void DrawAnnotation::addUpdatePolygon(PolygonData *polygonData)
{
    Polygon *polygon;
    if (!mPolygonMap.contains(polygonData->id)) {
        polygon = new Polygon(mapItem());
        polygonData->layer->addChild(polygon);
        mPolygonMap[polygonData->id] = polygon;
    }
    else {
        polygon = mPolygonMap[polygonData->id];
        polygon->clearPoints();
    }
    polygon->setName(polygonData->name);
    polygon->create(&polygonData->points);
    polygon->setPolygonData(polygonData);
    //    for (auto point: polygonData->points){
    //        osgEarth::GeoPoint geopos(mapItem()->getMapSRS(), point.x(), point.y(), point.z());
    //        polygon->addPoint(geopos);
    //    }
    //    polygon->setHeight(0);
    polygon->setStrokeWidth(polygonData->width);
    QColor color(QString::fromStdString(polygonData->color));
    polygon->setStrokeColor(Utility::qColor2osgEarthColor(color));
    QColor fillColor(QString::fromStdString(polygonData->fillColor));
    polygon->setFillColor(Utility::qColor2osgEarthColor(fillColor));
    //    polygon->setClamp(osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN);
}

bool DrawAnnotation::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;

    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        if (mState == State::READY) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            initDraw(geoPos);
            ea.setHandled(true);
            return true;
        }

        if (mState == State::DRAWING) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            drawing(geoPos);
            ea.setHandled(true);
            return true;
        }

    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::EDIT || mState == State::DRAWING)) {
        cancelDraw();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::EDIT || mState == State::DRAWING)) {
        confirmDraw();
        return false;
    }
    return false;
}

bool DrawAnnotation::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;
    //move icon-----------------------
    if(mIconNode){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }


    if(mState == State::DRAWING){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        tempDrawing(geoPos);
        return true;
    }
    //--------------------------------
    return false;


}

void DrawAnnotation::onLineItemCheck(bool check)
{
    if (check)
        makeIconNode("../data/images/draw/line.png");
    onItemChecked(Type::LINE, check);
}



void DrawAnnotation::LineNodeDataReceived(PolyLineData *lineNodeData)
{
    if (lineNodeData->command == "REMOVE") {
        if (mLineNodeMap.contains(lineNodeData->id)){
            mLineNodeMap[lineNodeData->id]->polyLineData()->layer->removeChild(mLineNodeMap[lineNodeData->id]);
            mLineNodeMap[lineNodeData->id].release();
            mLineNodeMap.remove(lineNodeData->id);
        }
    } else if (lineNodeData->command == "UPDATE"){
        addUpdateLineNode(lineNodeData);
    } else {
        addUpdateLineNode(lineNodeData);
    }
}

void DrawAnnotation::polygonDataReceived(PolygonData *polygonData)
{
    if (polygonData->command == "REMOVE"){
        if (mPolygonMap.contains(polygonData->id)){
            mPolygonMap[polygonData->id]->polygonData()->layer->removeChild(mPolygonMap[polygonData->id]);
            mPolygonMap[polygonData->id].release();
            mPolygonMap.remove(polygonData->id);
        }
    } else if (polygonData->command == "UPDATE") {
        addUpdatePolygon(polygonData);
    }
    else {
        addUpdatePolygon(polygonData);
    }
}

void DrawAnnotation::onPolygonItemCheck(bool check)
{
    if (check)
        makeIconNode("../data/images/draw/polygon.png");
    onItemChecked(Type::POLYGONN, check);


//    if (check) {
//        setState(State::READY);
//        mPolygonProperty = new PolygonProperty();
//        createProperty("Polygon", QVariant::fromValue<PolygonProperty*>(mPolygonProperty));
//        mapItem()->addNode(iconNode());
//    }
//    else {
//        if(state() == State::DRAWING)
//            cancelDraw();

//        setState(State::NONE);
//        mPolygon = nullptr;
//        hideProperty();
//        mapItem()->removeNode(iconNode());
//    }
}

void DrawAnnotation::initDraw(const osgEarth::GeoPoint &geoPos)
{
  /************initDraw for Line*************/

    mLine = new LineNode(mapItem());
    QString name;
    auto shapeLayer = DrawAnnotation::shapeLayer();
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

    case Type::POLYGONN:
        mPolygon = new Polygon(mapItem());
        name = POLYGON + QString::number(mCount);
        mPolygon->setName(name.toStdString());
        mPolygonProperty->setPolygon(mPolygon, mapItem()->getMapSRS());

        if(!shapeLayer->containsLayer(mPolygonLayer)){
            mPolygonLayer->clear();
            shapeLayer->addLayer(mPolygonLayer);
        }
        mPolygonLayer->addChild(mPolygon);
        break;
    default:
        //name = POLYLINE + QString::number(mCount);
        break;
    }
    setState(State::DRAWING);
    mCount++;
}

void DrawAnnotation::onItemChecked(Type type, bool check)
{
    if (check) {
        setState(State::READY);
        if (type == Type::POLYGONN){
            mType = type;
            mPolygonProperty = new PolygonProperty();
            createProperty("Polygon", QVariant::fromValue<PolygonProperty*>(mPolygonProperty));
            mapItem()->addNode(iconNode());
        }
        else{
            mType = type;
            mLineProperty = new LineProperty();
            createProperty("Line", QVariant::fromValue<LineProperty*>(mLineProperty));
            mapItem()->addNode(iconNode());
        }
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

void DrawAnnotation::drawing(const osgEarth::GeoPoint &geoPos)
{
    if((mType != Type::LINE && mType != Type::POLYGONN) || mType == Type::SLOPEE  && mLine->getSize()>=2)
        return;

    if(mType == Type::POLYGONN)
        mPolygon->addPoint(geoPos);

    else
        mLine->addPoint(geoPos);


}

void DrawAnnotation::tempDrawing(const osgEarth::GeoPoint &geoPos)
{
    if (mLine->getSize() > 1)
    {
        mLine->removePoint();
    }
    if (mType == Type::HEIGHT){
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

    if (mType == Type::POLYGONN){
        if (mPolygon->getSize() > 1)
        {
            mPolygon->removePoint();
        }
        mPolygon->addPoint(geoPos);
    }
}

void DrawAnnotation::confirmDraw()
{
    if (state() == State::EDIT || state() == State::DRAWING) {
        setState(State::READY);
    }
}

void DrawAnnotation::cancelDraw()
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
        case Type::POLYGONN:
            if(state() == State::DRAWING){
                mPolygonLayer->removeChild(mPolygon);
                mPolygon = nullptr;
                mPolygonProperty->setPolygon(mPolygon, mapItem()->getMapSRS());
                setState(State::READY);
                mCount--;

                if(!mPolygonLayer->hasNode())
                    DrawAnnotation::shapeLayer()->removeLayer(mPolygonLayer);
                break;
            }
        case Type::NONE:
            break;
        }
        if (mType != Type::POLYGONN){
        mLine = nullptr;
        mMeasureHeight = nullptr;
        mLineProperty->setLine(mLine);
        setState(State::READY);
        mCount--;

        if(!mLineLayer->hasNode())
            DrawAnnotation::shapeLayer()->removeLayer(mLineLayer);
        }
    }
}

void DrawAnnotation::createProperty(QString name, QVariant property)
{
    QQmlComponent* comp = new QQmlComponent(qmlEngine());
    connect(comp, &QQmlComponent::statusChanged, [comp, property, name, this](){
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }
        mItem = qobject_cast<QQuickItem*>(comp->create());
        mItem->setProperty("cppInterface", property);

        mainWindow()->getToolboxManager()->addPropertyItem(mItem, name);
    });


    comp->loadUrl(QUrl("qrc:/AnnotationProperties.qml"));
}

void DrawAnnotation::hideProperty()
{
    mainWindow()->getToolboxManager()->removePropertyItem();
}

