#include "model.h"
#include "mapItem.h"
#include "MoveableModelNode.h"
#include "flyableModelNode.h"
#include "serviceManager.h"
#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarth/ModelSource>
#include <osgEarth/ModelSource>

#include <QPainter>
#include <QRandomGenerator>
#include <osgFX/Outline>
#include <osgEarthSymbology/Style>
#include <osgEarthSymbology/StyleSheet>
#include <osg/ShapeDrawable>

using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;
int Model::mCount{0};
Model::Model(QObject *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(model);
}

Model::~Model()
{
    mIconNode.release();
}

bool Model::setup()
{

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(mapItem(), &MapItem::modeChanged, this, &Model::onModeChanged);
    mIs3D = mapItem()->getMode();

    //    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(serviceManager(), &ServiceManager::flyableNodeDataReceived, this, &Model::addUpdateFlyableNode);
    connect(serviceManager(), &ServiceManager::nodeDataReceived, this, &Model::addUpdateNode);
    connect(serviceManager(), &ServiceManager::statusNodeDataReceived, this, &Model::addUpdateStatusNode);
    connect(serviceManager(), &ServiceManager::movableNodeDataReceived, this, &Model::addUpdateMovableNode);

    mModelNodeLayer = new CompositeAnnotationLayer();
    mModelNodeLayer->setName(MODEL);
    mapItem()->getMapObject()->addLayer(mModelNodeLayer);


    auto treeToolboxItem =  new ToolboxItem{TREE, MODEL, "qrc:/resources/tree.png", true};
    QObject::connect(treeToolboxItem, &ToolboxItem::itemChecked, this, &Model::onTreeItemCheck);
    toolbox()->addItem(treeToolboxItem);

    auto carToolboxItem =  new ToolboxItem{CAR, MODEL, "qrc:/resources/car.png", true};
    QObject::connect(carToolboxItem, &ToolboxItem::itemChecked, this, &Model::onCarItemCheck);
    toolbox()->addItem(carToolboxItem);

    auto airplaneToolboxItem =  new ToolboxItem{AIRPLANE, MODEL, "qrc:/resources/airplane.png", true};
    QObject::connect(airplaneToolboxItem, &ToolboxItem::itemChecked, this, &Model::onAirplanItemCheck);
    toolbox()->addItem(airplaneToolboxItem);

    auto statusToolboxItem =  new ToolboxItem{STATUS, MODEL, "qrc:/resources/status.png", true};
    QObject::connect(statusToolboxItem, &ToolboxItem::itemChecked, this, &Model::onStatusItemCheck);
    toolbox()->addItem(statusToolboxItem);


    mSimpleNodeLayer = new ParenticAnnotationLayer();
    mSimpleNodeLayer->setName(TREE);

    mMoveableNodeLayer = new ParenticAnnotationLayer();
    mMoveableNodeLayer->setName(CAR);

    mFlyableNodelLayer = new ParenticAnnotationLayer();
    mFlyableNodelLayer->setName(AIRPLANE);

    mStatusNodelLayer = new ParenticAnnotationLayer();
    mStatusNodelLayer->setName(STATUS);
    return true;
}

void Model::makeIconNode(const QString &fileName)
{
    osg::Image* icon = osgDB::readImageFile(fileName.toStdString());
    if(icon){
        icon->scaleImage(24, 24, icon->r());
        mIconNode = new osgEarth::Annotation::PlaceNode();
        mIconNode->setIconImage(icon);
    }
}

osgEarth::Annotation::PlaceNode *Model::iconNode() const
{
    return mIconNode.get();
}

Model::State Model::state() const
{
    return mState;
}

void Model::setState(State newState)
{
    mState = newState;
}

bool Model::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        SimpleModelNode* modelNode = pick(ea.getX(), ea.getY());
        if(modelNode){
            modelNode->selectModel();
        }
        if(mState == State::NONE)
            return false;
        if (mState == State::READY) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            initModel(geoPos);
            return true;
        }

        if (mState == State::MOVING) {
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            moving(geoPos);
            return true;
        }

    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::MOVING)) {
        cancel();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::MOVING)) {
        confirm();

        return false;
    }
    return false;
}

bool Model::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    //move icon-----------------------
    if(mIconNode){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }

    return false;
}

bool Model::frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    return false;
}

osgEarth::Symbology::Style &Model::getDefaultStyle()
{
    static osgEarth::Symbology::Style style;
    static bool bFirst = true;
    if (bFirst) {
        static osg::Node *node = new osg::Node;
        style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);
        bFirst = false;
    }

    return style;
}

void Model::onTreeItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/tree.png");

        mType = Type::SIMPLE;

        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}


void Model::onCarItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/car.png");
        mType = Type::MOVEABLE;

        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}

void Model::onAirplanItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/models/aircraft/aircraft.png");

        mType = Type::FLYABLE;
        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}

void Model::onStatusItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/status.png");

        mType = Type::INFO;
        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}

void Model::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
}

void Model::addUpdateFlyableNode(NodeData *nodeData)
{

    osgEarth::GeoPoint geoPoint(mapItem()->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
    osg::ref_ptr<FlyableModelNode> flyableNode;

    if(!mFlyableNodeMap.contains(nodeData->id)){
        flyableNode = new FlyableModelNode(mapItem(), nodeData->url3D, nodeData->url2D);
        flyableNode->setPosition(geoPoint);
        mFlyableNodeMap[nodeData->id] = flyableNode;
    }
    else{
        flyableNode = mFlyableNodeMap[nodeData->id];
        for(auto layer: flyableNode->nodeData()->layers){
            layer->removeChild(flyableNode);
        }
        flyableNode->flyTo(geoPoint, nodeData->speed);
    }
    for(auto layer: nodeData->layers){
        layer->addChild(flyableNode);
    }
    flyableNode->setName(nodeData->name);
    flyableNode->setNodeData(nodeData);
}

void Model::addUpdateNode(NodeData *nodeData)
{
    osgEarth::GeoPoint geoPoint(mapItem()->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
    osg::ref_ptr<SimpleModelNode> node;

    if(!mNodeMap.contains(nodeData->id)){
        node = new SimpleModelNode(mapItem(), nodeData->url3D, nodeData->url2D);
        node->setPosition(geoPoint);
        mNodeMap[nodeData->id] = node;
    }
    else{
        node = mNodeMap[nodeData->id];
        for(auto layer: node->nodeData()->layers){
            layer->removeChild(node);
        }
    }
    for(auto layer: nodeData->layers){
        layer->addChild(node);
    }
    node->setName(nodeData->name);
    node->setNodeData(nodeData);
}

void Model::addUpdateStatusNode(StatusNodeData *statusnNodeData)
{
    osgEarth::GeoPoint geoPoint(mapItem()->getMapObject()->getSRS(), statusnNodeData->longitude, statusnNodeData->latitude, statusnNodeData->altitude);
    osg::ref_ptr<StatusNode> statusNode;

    std::list<StatusNode::Data> dataList;
    for(auto& data: statusnNodeData->data) {
        qDebug() << data.value;
        dataList.push_back(StatusNode::Data{data.name, data.value});
    }

    if(!mStatusNodeMap.contains(statusnNodeData->id)){
        statusNode = new StatusNode(mapItem());
        statusNode->setData(QString::fromStdString(statusnNodeData->name), &dataList);
        mStatusNodeMap[statusnNodeData->id] = statusNode;
    }
    else{
        statusNode = mStatusNodeMap[statusnNodeData->id];
        statusNode->nodeData()->layer->removeChild(statusNode);
    }
    statusNode->setPosition(geoPoint);
    statusnNodeData->layer->addChild(statusNode);

    statusNode->setName(statusnNodeData->name);
    statusNode->setNodeData(statusnNodeData);
}

void Model::addUpdateMovableNode(NodeData *nodeData)
{
    osgEarth::GeoPoint geoPoint(mapItem()->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
    osg::ref_ptr<MoveableModelNode> movableNode;

    if(!mMovableNodeMap.contains(nodeData->id)){
        movableNode = new MoveableModelNode(mapItem(), nodeData->url3D, nodeData->url2D);
        movableNode->setPosition(geoPoint);
        mMovableNodeMap[nodeData->id] = movableNode;
    }
    else{
        movableNode = mMovableNodeMap[nodeData->id];
        for(auto layer: movableNode->nodeData()->layers){
            layer->removeChild(movableNode);
        }
        movableNode->moveTo(geoPoint, nodeData->speed);
    }
    for(auto layer: nodeData->layers){
        layer->addChild(movableNode);
    }
    movableNode->setName(nodeData->name);
    movableNode->setNodeData(nodeData);
}

void Model::initModel(const osgEarth::GeoPoint &geoPos){
    QString name;
    switch (mType) {
    case Type::SIMPLE:
        name = "Tree" + QString::number(mCount);
        mCurrentModel = new SimpleModelNode(mapItem(),"../data/models/tree_I.osgb", "../data/images/model/tree.png");
        mCurrentModel->setModelColor(osgEarth::Color::Aqua);
        if(!mModelNodeLayer->containsLayer(mSimpleNodeLayer)){
            mSimpleNodeLayer->clear();
            mModelNodeLayer->addLayer(mSimpleNodeLayer);
        }
        mSimpleNodeLayer->addChild(mCurrentModel);
        break;
    case Type::MOVEABLE:
        name = "Car" + QString::number(mCount);
        mCurrentModel = new MoveableModelNode(mapItem(),"../data/models/car.osgb", "../data/images/model/car.png");
        mCurrentModel->setModelColor(osgEarth::Color::Green);
        if(!mModelNodeLayer->containsLayer(mMoveableNodeLayer)){
            mMoveableNodeLayer->clear();
            mModelNodeLayer->addLayer(mMoveableNodeLayer);
        }
        mMoveableNodeLayer->addChild(mCurrentModel);
        break;
    case Type::FLYABLE:
        name = "Airplane" + QString::number(mCount);
        mCurrentModel = new FlyableModelNode(mapItem(),"../data/models/aircraft/boeing-747.osgb", "../data/models/aircraft/aircraft.png");
        mCurrentModel->setModelColor(osgEarth::Color::Red);
        if(!mModelNodeLayer->containsLayer(mFlyableNodelLayer)){
            mFlyableNodelLayer->clear();
            mModelNodeLayer->addLayer(mFlyableNodelLayer);
        }
        mFlyableNodelLayer->addChild(mCurrentModel);
        break;
    case Type::INFO:
        name = "Status" + QString::number(mCount);
        mStatusModel = new StatusNode(mapItem());
        {
            StatusNode::Data data;
            data.name = "name";
            data.value = 10;
            StatusNode::Data data1;
            data1.name = "name";
            data1.value = 30000;
            StatusNode::Data data2;
            data2.name = "name";
            data2.value = "kasjdf";
            std::list<StatusNode::Data> dataList;
            dataList.push_back(data);
            dataList.push_back(data1);
            dataList.push_back(data2);
            mStatusModel->setData("title", &dataList);
        }

        if(!mModelNodeLayer->containsLayer(mStatusNodelLayer)){
            mStatusNodelLayer->clear();
            mModelNodeLayer->addLayer(mStatusNodelLayer);
        }
        mStatusNodelLayer->addChild(mStatusModel);
        break;
    default:
        break;
    }

    if (mCurrentModel){
        mCurrentModel->setName(name.toStdString());
        mCurrentModel->setPosition(geoPos);
    }
    else{
        mStatusModel->setName(name.toStdString());
        mStatusModel->setPosition(geoPos);
    }


    ;
    setState(State::MOVING);
    mCount++;
}

void Model::moving(osgEarth::GeoPoint &geoPos){

    if (mCurrentModel){
        if (mCurrentModel->asFlyableModelNode()){
            double randomHeight = 50 + (QRandomGenerator::global()->generate() % (100 - 50));
            geoPos.z() += randomHeight;
            mCurrentModel->asFlyableModelNode()->flyTo(geoPos,20);
            return;
        }
        if (mCurrentModel->asMoveableModelNode()){
            mCurrentModel->asMoveableModelNode()->moveTo(geoPos,20);
            return;
        }
    }
    if (mCurrentModel){
        mCurrentModel->setPosition(geoPos);
    }
    else{
        mStatusModel->setPosition(geoPos);
    }
}

void Model::confirm()
{
    if (state() == State::MOVING) {
        setState(State::READY);
    }
}

void Model::cancel(){

    if(state() == State::MOVING){
        switch (mType) {
        case Type::SIMPLE:
            mSimpleNodeLayer->removeChild(mCurrentModel);
            break;
        case Type::MOVEABLE:
            mMoveableNodeLayer->removeChild(mCurrentModel);
            break;
        case Type::FLYABLE:
            mFlyableNodelLayer->removeChild(mCurrentModel);
            break;
        case Type::INFO:
            mStatusNodelLayer->removeChild(mStatusModel);
            break;
        default:
            break;
        }
        mCurrentModel.release();
        mStatusModel.release();
        setState(State::READY);
        mCount--;
    }
}

SimpleModelNode *Model::pick(float x, float y)
{
    SimpleModelNode *simpleModelNode = nullptr;
    osgViewer::Viewer *viewer = mapItem()->getViewer();
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (viewer->computeIntersections(x, y, intersections))
    {
        for(const osgUtil::LineSegmentIntersector::Intersection& hit : intersections)
        {
            const osg::NodePath& nodePath = hit.nodePath;
            for(osg::NodePath::const_iterator nitr=nodePath.begin();
                 nitr!=nodePath.end();
                 ++nitr)
            {
                simpleModelNode = dynamic_cast<SimpleModelNode*>(*nitr);
                if (simpleModelNode)
                {
                    osg::ref_ptr<osg::MatrixTransform> _selectionBox;
                    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
                    geode->addDrawable(
                        new osg::ShapeDrawable(new osg::Box(osg::Vec3(),
                                                            1.0f)) );
                    _selectionBox = new osg::MatrixTransform;
                    _selectionBox->setNodeMask( 0x1 );
                    _selectionBox->addChild( geode.get() );
                    osg::StateSet* ss = _selectionBox->getOrCreateStateSet();
                    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
                    ss->setAttributeAndModes(new osg::PolygonMode(
                        osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE));

                    osg::BoundingBox bb = hit.drawable->getBoundingBox();
                    qDebug()<<"radius: "<<bb.radius();
                    osg::Vec3 worldCenter = bb.center() *
                                            osg::computeLocalToWorld(hit.nodePath);
                    _selectionBox->setMatrix(
                        osg::Matrix::scale(bb.xMax()-bb.xMin(),
                                           bb.yMax()-bb.yMin(),
                                           bb.zMax()-bb.zMin()) *
                        osg::Matrix::translate(worldCenter) );

                    return simpleModelNode;
                }
            }
        }
    }
    return simpleModelNode;
}
