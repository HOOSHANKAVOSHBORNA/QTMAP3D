#include "drawAnnotation.h"
#include "mainwindow.h"
#include "property.h"
#include "annotatedNode.h"
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

void DrawAnnotation::addUpdateAnnotatedNode(PolyLineData *lineNodeData)
{
    AnnotatedNode *lineNode;
    if (!mLineMap.contains(lineNodeData->id)) {
        lineNode = new AnnotatedNode(mapItem(), AnnotatedNode::GeneralType::POLYLINETYPE);
        lineNodeData->layer->addChild(lineNode);
        mLineMap[lineNodeData->id] = lineNode;
    }
    else {
        lineNode = mLineMap[lineNodeData->id];
        lineNode->clear();
    }

    lineNode->create(&lineNodeData->points);
    lineNode->setFillColor(osgEarth::Color(lineNodeData->color));
    lineNode->setWidth(lineNodeData->width);
    lineNode->setPolyLineData(lineNodeData);
}

void DrawAnnotation::addUpdatePolygon(PolygonData *polygonNodeData)
{
    AnnotatedNode *polygon;
    if (!mPolygonMap.contains(polygonNodeData->id)) {
        polygon = new AnnotatedNode(mapItem(), AnnotatedNode::GeneralType::POLYGONTYPE);
        polygonNodeData->layer->addChild(polygon);
        mPolygonMap[polygonNodeData->id] = polygon;
    }
    else {
        polygon = mPolygonMap[polygonNodeData->id];
        polygon->removePoint();
    }
    polygon->setName(polygonNodeData->name);
    polygon->create(&polygonNodeData->points);
    polygon->setPolygonData(polygonNodeData);
    polygon->setStrokeWidth(polygonNodeData->width);
    QColor color(QString::fromStdString(polygonNodeData->color));
    polygon->setStrokeColor(Utility::qColor2osgEarthColor(color));
    QColor fillColor(QString::fromStdString(polygonNodeData->fillColor));
    polygon->setFillColor(Utility::qColor2osgEarthColor(fillColor));
}

bool DrawAnnotation::setup()
{
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mShapeLayer = new CompositeAnnotationLayer();
    mShapeLayer->setName(CATEGORY);
    mapItem()->getMapObject()->addLayer(mShapeLayer);

    connect(serviceManager(), &ServiceManager::annotationNodeDataReceived, this, &DrawAnnotation::AnnotatedNodeDataReceived);
    connect(serviceManager(), &ServiceManager::polygonDataReceived, this, &DrawAnnotation::polygonDataReceived);
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    auto toolboxItemRuler =  new ToolboxItem{RULER, M_CATEGORY, "qrc:/resources/ruler.png", true};
    QObject::connect(toolboxItemRuler, &ToolboxItem::itemChecked, this, &DrawAnnotation::onRulerItemCheck);
    toolbox()->addItem(toolboxItemRuler);

    auto toolboxItemHeight =  new ToolboxItem{MEASUREHEIGHT, M_CATEGORY, "qrc:/resources/height.png", true};
    QObject::connect(toolboxItemHeight, &ToolboxItem::itemChecked, this, &DrawAnnotation::onHeightItemCheck);
    toolbox()->addItem(toolboxItemHeight);

    auto toolboxItemSlope =  new ToolboxItem{SLOPE, M_CATEGORY, "qrc:/resources/slope.png", true};
    QObject::connect(toolboxItemSlope, &ToolboxItem::itemChecked, this, &DrawAnnotation::onSlopeItemCheck);
    toolbox()->addItem(toolboxItemSlope);

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

    mRulerLayer = new ParenticAnnotationLayer();
    mRulerLayer->setName(RULER);

    mHeightLayer = new ParenticAnnotationLayer();
    mHeightLayer->setName(MEASUREHEIGHT);

    mSlopeLayer = new ParenticAnnotationLayer();
    mSlopeLayer->setName(SLOPE);

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

bool DrawAnnotation::mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
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

void DrawAnnotation::onRulerItemCheck(bool check)
{
    if (check)
    makeIconNode("../data/images/draw/ruler.png");

    onItemChecked(Type::RULERR, check);
}

void DrawAnnotation::onHeightItemCheck(bool check)
{
    if (check)

    makeIconNode("../data/images/draw/height.png");

    onItemChecked(Type::HEIGHT, check);

}

void DrawAnnotation::onSlopeItemCheck(bool check)
{
        if (check)
        makeIconNode("../data/images/draw/slope.png");

        onItemChecked(Type::SLOPEE, check);
}

void DrawAnnotation::onLineItemCheck(bool check)
{
        if (check)
        makeIconNode("../data/images/draw/line.png");
        mIsLine = true;
        onItemChecked(Type::LINE, check);
}

void DrawAnnotation::onPolygonItemCheck(bool check)
{
        if (check)
        makeIconNode("../data/images/draw/polygon.png");
        mIsLine = false;
        onItemChecked(Type::POLYGONN, check);

}

void DrawAnnotation::AnnotatedNodeDataReceived(PolyLineData *annotationNodeData)
{
    if (annotationNodeData->command == "REMOVE") {
        if (mLineMap.contains(annotationNodeData->id)){
            mLineMap[annotationNodeData->id]->polyLineData()->layer->removeChild(mLineMap[annotationNodeData->id]);
            mLineMap[annotationNodeData->id].release();
            mLineMap.remove(annotationNodeData->id);
        }
    } else if (annotationNodeData->command == "UPDATE"){
        addUpdateAnnotatedNode(annotationNodeData);
    } else {
        addUpdateAnnotatedNode(annotationNodeData);
    }
}

void DrawAnnotation::polygonDataReceived(PolygonData *polygonNodeData)
{
    if (polygonNodeData->command == "REMOVE"){
        if (mPolygonMap.contains(polygonNodeData->id)){
            mPolygonMap[polygonNodeData->id]->polygonData()->layer->removeChild(mPolygonMap[polygonNodeData->id]);
            mPolygonMap[polygonNodeData->id].release();
            mPolygonMap.remove(polygonNodeData->id);
        }
    } else if (polygonNodeData->command == "UPDATE") {
        addUpdatePolygon(polygonNodeData);
    }
    else {
        addUpdatePolygon(polygonNodeData);
    }
}


void DrawAnnotation::initDraw(const osgEarth::GeoPoint &geoPos)
{
    if(mIsLine){
        mAnnotation = new AnnotatedNode(mapItem(), AnnotatedNode::GeneralType::POLYLINETYPE);
    }
    else{
        mAnnotation = new AnnotatedNode(mapItem(), AnnotatedNode::GeneralType::POLYGONTYPE);
    }

    QString name;
    auto shapeLayer = DrawAnnotation::shapeLayer();
    auto measureLayer = DrawAnnotation::measureLayer();
    switch (mType) {
    case Type::LINE:
        name = POLYLINE + QString::number(mCount);
        mAnnotation->setName(name.toStdString());
        mAnnotationProperty->setAnnotatedNode(mAnnotation);

        if(!shapeLayer->containsLayer(mLineLayer)){
            mLineLayer->clear();
            shapeLayer->addLayer(mLineLayer);
        }
        mLineLayer->addChild(mAnnotation);
        break;

    case Type::POLYGONN:

        name = POLYGON + QString::number(mCount);
        mAnnotation->setName(name.toStdString());
        mAnnotationProperty->setAnnotatedNode(mAnnotation);

        if(!shapeLayer->containsLayer(mPolygonLayer)){
            mPolygonLayer->clear();
            shapeLayer->addLayer(mPolygonLayer);
        }
        mPolygonLayer->addChild(mAnnotation);
        break;
    case Type::RULERR:
        name = RULER + QString::number(mCount);
        mAnnotation->setName(name.toStdString());
        mAnnotationProperty->setRuler(mAnnotation);
        mAnnotation->addPoint(geoPos);

        if(!measureLayer->containsLayer(mRulerLayer)){
            mRulerLayer->clear();
            measureLayer->addLayer(mRulerLayer);
        }
        mRulerLayer->addChild(mAnnotation);
        break;
    case Type::HEIGHT:
        name = MEASUREHEIGHT + QString::number(mCount);
        mAnnotation->setName(name.toStdString());
        mAnnotationProperty->setMeasureHeight(mAnnotation);
        mAnnotation->addPoint(geoPos);

        if(!measureLayer->containsLayer(mHeightLayer)){
            mHeightLayer->clear();
            measureLayer->addLayer(mHeightLayer);
        }
        mHeightLayer->addChild(mAnnotation);
        break;
    case Type::SLOPEE:
        name = SLOPE + QString::number(mCount);
        mAnnotation->setName(name.toStdString());
        mAnnotationProperty->setMesureSlope(mAnnotation);
        mAnnotation->addPoint(geoPos);

        if(!measureLayer->containsLayer(mSlopeLayer)){
            mSlopeLayer->clear();
            measureLayer->addLayer(mSlopeLayer);
        }
        mSlopeLayer->addChild(mAnnotation);

        break;
    default:
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
            mAnnotationProperty = new AnnotationProperty();
            mPropertyItem = new PropertyItem(qmlEngine(), QVariant::fromValue<AnnotationProperty*>(mAnnotationProperty));
            mainWindow()->getToolboxManager()->addPropertyItem(mPropertyItem->getQuickItem(), "Polygon");
            mapItem()->addNode(iconNode());
        }
        else{
            mType = type;
            mAnnotationProperty = new AnnotationProperty();
            mPropertyItem = new PropertyItem(qmlEngine(), QVariant::fromValue<AnnotationProperty*>(mAnnotationProperty));
            mainWindow()->getToolboxManager()->addPropertyItem(mPropertyItem->getQuickItem(), "line");
            mapItem()->addNode(iconNode());
        }
    }
    else {
        if(state() == State::DRAWING)
            cancelDraw();

        setState(State::NONE);
        mType = Type::NONE;
        mAnnotation = nullptr;
        hideProperty();
        mapItem()->removeNode(iconNode());
    }
}

void DrawAnnotation::drawing(const osgEarth::GeoPoint &geoPos)
{
    if((mType != Type::LINE && mType != Type::POLYGONN) || mType == Type::SLOPEE  && mAnnotation->getSize()>=2)
        return;

    if(mType == Type::LINE || mType == Type::POLYGONN )
        mAnnotation->addPoint(geoPos);
}

void DrawAnnotation::tempDrawing(const osgEarth::GeoPoint &geoPos)
{
    if (mAnnotation->getSize() > 1)
    {
        mAnnotation->removePoint();
    }
    if (mType == Type::HEIGHT){
        if (mAnnotation->getSize() > 1)
        {
            mAnnotation->removePoint();
        }
        auto firstPoint = mAnnotation->getPoint(0);
        double h = geoPos.z() - firstPoint.z();
        osgEarth::GeoPoint midPoint(mapItem()->getMapSRS(), geoPos.x(), geoPos.y(), firstPoint.z());
        if(h > 0)
            midPoint.set(mapItem()->getMapSRS(), firstPoint.x(), firstPoint.y(), geoPos.z(),osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
        mAnnotation->addPoint(midPoint);
    }
    mAnnotation->addPoint(geoPos);

    if (mType == Type::POLYGONN){
        if (mAnnotation->getSize() > 1)
        {
            mAnnotation->removePoint();
        }
        mAnnotation->addPoint(geoPos);
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
            mLineLayer->removeChild(mAnnotation);
            break;
        case Type::RULERR:
            mRulerLayer->removeChild(mAnnotation);
            break;
        case Type::HEIGHT:
            mHeightLayer->removeChild(mAnnotation);
            break;
        case Type::SLOPEE:
            mSlopeLayer->removeChild(mAnnotation);
            break;
        case Type::POLYGONN:
            if(state() == State::DRAWING){
                mPolygonLayer->removeChild(mAnnotation);
                mAnnotation = nullptr;
                mAnnotationProperty->setAnnotatedNode(mAnnotation/*, mapItem()->getMapSRS()*/);
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
        mAnnotation = nullptr;
//        mMeasureHeight = nullptr;
        mAnnotationProperty->setAnnotatedNode(mAnnotation);
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

CompositeAnnotationLayer *DrawAnnotation::measureLayer()
{
    if(!mMeasureLayer)
        mMeasureLayer = dynamic_cast<CompositeAnnotationLayer*>(mapItem()->getMapObject()->getLayerByName(M_CATEGORY));
    return mMeasureLayer;
}

