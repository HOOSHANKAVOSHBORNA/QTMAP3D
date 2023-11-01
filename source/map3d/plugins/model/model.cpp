#include "model.h"
#include "mapItem.h"
#include "moveableModelNode.h"
#include "flyableModelNode.h"
#include "serviceManager.h"
#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarth/ModelSource>
#include <osgEarth/ModelSource>

#include <QPainter>
#include <QRandomGenerator>
#include <bookmark.h>
#include <osgFX/Outline>
#include <osgEarthSymbology/Style>
#include <osgEarthSymbology/StyleSheet>
#include <osg/ShapeDrawable>
#include "attackManager.h"

using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;
using osgKeyButton = osgGA::GUIEventAdapter::KeySymbol;
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

    auto tankToolboxItem =  new ToolboxItem{"Tank", MODEL, "qrc:/resources/tank.png", true};
    QObject::connect(tankToolboxItem, &ToolboxItem::itemChecked, this, &Model::onTankItemCheck);
    toolbox()->addItem(tankToolboxItem);

    mSimpleNodeLayer = new ParenticAnnotationLayer();
    mSimpleNodeLayer->setName(TREE);

    mMoveableNodeLayer = new ParenticAnnotationLayer();
    mMoveableNodeLayer->setName(CAR);

    mFlyableNodelLayer = new ParenticAnnotationLayer();
    mFlyableNodelLayer->setName(AIRPLANE);

    mAttackNodeLayer = new ParenticAnnotationLayer();
    mAttackNodeLayer->setName(ATTACKERS);


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
            return true;
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
        if(mState == State::ATTACKING){
            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            mCurrentModel->getAttackManager()->attackTo(mBulletID,geoPos);
            return true;
        }

    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::MOVING)) {
        cancel();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::ATTACKING)) {
        mCurrentModel->getAttackManager()->attackResult(true,mBulletID);
        mBulletID = mCurrentModel->getAttackManager()->readyBullet("../data/models/missile/missile.osgb", "../data/models/missile/missile.png");
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::MOVING)) {
        if(mType == Type::ATTACKER && !(mState == State::ATTACKING)){
            mBulletID = mCurrentModel->getAttackManager()->readyBullet("../data/models/missile/missile.osgb", "../data/models/missile/missile.png");
            mState = State::ATTACKING;
        }else{
        confirm();
        }
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::ATTACKING)) {
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
        makeIconNode("../data/models/tree/tree.png");

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
        makeIconNode("../data/models/car/car.png");
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
        makeIconNode("../data/models/airplane/airplane.png");

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

void Model::onTankItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/models/tank/tank.png");
        mType = Type::ATTACKER;
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

FlyableModelNode* Model::addUpdateFlyableNode(NodeData *nodeData)
{
    osgEarth::GeoPoint geoPoint(mapItem()->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
    osg::ref_ptr<FlyableModelNode> flyableNode;

    if(!mFlyableNodeMap.contains(nodeData->id)){
        flyableNode = new FlyableModelNode(mapItem(), nodeData->url3D, nodeData->url2D, qmlEngine(), bookmarkManager());
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
    return flyableNode;
//    mCurrentModel = flyableNode;
}

SimpleModelNode* Model::addUpdateNode(NodeData *nodeData)
{
    osgEarth::GeoPoint geoPoint(mapItem()->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
    osg::ref_ptr<SimpleModelNode> node;

    if(!mNodeMap.contains(nodeData->id)){
        node = new SimpleModelNode(mapItem(), nodeData->url3D, nodeData->url2D, qmlEngine(), bookmarkManager());
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
    node->setPosition(geoPoint);
    node->setNodeData(nodeData);
    return node;
//    mCurrentModel = node;
}

MoveableModelNode *Model::addUpdateMovableNode(NodeData *nodeData)
{
    osgEarth::GeoPoint geoPoint(mapItem()->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
    osg::ref_ptr<MoveableModelNode> movableNode;

    if(!mMovableNodeMap.contains(nodeData->id)){
        movableNode = new MoveableModelNode(mapItem(), nodeData->url3D, nodeData->url2D, qmlEngine(), bookmarkManager());
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
    return movableNode;
//    mCurrentModel = movableNode;
}

void Model::initModel(const osgEarth::GeoPoint &geoPos){
    QString name;
    switch (mType) {
    case Type::SIMPLE:
        mNodeData = sampleNodeData("Tree", "../data/models/tree/tree.png", "../data/models/tree/tree.osgb", "", geoPos);
        mNodeData->id = 500 + mCount;
        if(!mModelNodeLayer->containsLayer(mSimpleNodeLayer)){
            mSimpleNodeLayer->clear();
            mModelNodeLayer->addLayer(mSimpleNodeLayer);
        }
        mNodeData->layers.push_back(mSimpleNodeLayer);
        mCurrentModel = addUpdateNode(mNodeData);
        break;
    case Type::MOVEABLE:
        mNodeData = sampleNodeData("Car", "../data/models/car/car.png", "../data/models/car/car.osgb", "", geoPos);
        mNodeData->id = 500 + mCount;
        if(!mModelNodeLayer->containsLayer(mMoveableNodeLayer)){
            mMoveableNodeLayer->clear();
            mModelNodeLayer->addLayer(mMoveableNodeLayer);
        }
        mNodeData->layers.push_back(mMoveableNodeLayer);
        mCurrentModel = addUpdateMovableNode(mNodeData);
        break;
    case Type::FLYABLE:
        mNodeData = sampleNodeData("Airplane", "../data/models/airplane/airplane.png", "../data/models/airplane/airplane.osgb", "", geoPos);
        mNodeData->id = 500 + mCount;
        if(!mModelNodeLayer->containsLayer(mFlyableNodelLayer)){
            mFlyableNodelLayer->clear();
            mModelNodeLayer->addLayer(mFlyableNodelLayer);
        }
        mNodeData->layers.push_back(mFlyableNodelLayer);
        mCurrentModel = addUpdateFlyableNode(mNodeData);
        break;
    case Type::ATTACKER:
        mCurrentModel = new MoveableModelNode(mapItem(),"../data/models/tank/tank.osg", "../data/models/tank/tank.png", qmlEngine(), bookmarkManager());
        mCurrentModel->setModelColor(osgEarth::Color::Red);

        mCurrentModel->isAttacker(mAttackNodeLayer,5);
        if(!mModelNodeLayer->containsLayer(mAttackNodeLayer)){
            mAttackNodeLayer->clear();
            mModelNodeLayer->addLayer(mAttackNodeLayer);
        }
        mCurrentModel->setPosition(geoPos);
        mAttackNodeLayer->addChild(mCurrentModel);
        break;
    default:
        break;
    }
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
}


void Model::confirm()
{
    if (state() == State::MOVING || state() == State::ATTACKING) {
        setState(State::READY);
    }
}

void Model::cancel(){

    if(state() == State::MOVING){
        switch (mType) {
        case Type::SIMPLE:
            mSimpleNodeLayer->removeChild(mCurrentModel);
            mNodeMap.remove(mNodeData->id);
            break;
        case Type::MOVEABLE:
            mMoveableNodeLayer->removeChild(mCurrentModel);
            mMovableNodeMap.remove(mNodeData->id);
            break;
        case Type::FLYABLE:
            mFlyableNodelLayer->removeChild(mCurrentModel);
            mFlyableNodeMap.remove(mNodeData->id);
            break;
        case Type::ATTACKER:
            mAttackNodeLayer->removeChild(mCurrentModel);

        default:
            break;
        }
        mCurrentModel.release();
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

NodeData *Model::sampleNodeData(std::string name, std::string url2d, std::string url3d, std::string imgSrc,
                                osgEarth::GeoPoint geoPos)
{
    NodeData* nodeData = new NodeData();
//    flyableNodeData->id = 100;
    nodeData->name = name + std::to_string(mCount);
    nodeData->longitude = geoPos.x();
    nodeData->latitude = geoPos.y();
    nodeData->altitude = geoPos.z();
    nodeData->url2D = url2d;
    nodeData->url3D = url3d;
    nodeData->imgSrc = imgSrc;
    nodeData->color = "white";
    nodeData->speed = 100;
    nodeData->fieldData.push_back(NodeFieldData{"name", "Aircraft" + QString::number(mCount), "Main Information","qrc:/Resources/exclamation-mark.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Id",QString::number(100 + mCount), "Main Information","qrc:/Resources/exclamation-mark.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Longitude",QString::number(nodeData->longitude), "Location Information","qrc:/Resources/location.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Latitude",QString::number(nodeData->latitude), "Location Information","qrc:/Resources/location.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Altitude",QString::number(nodeData->altitude), "Location Information","qrc:/Resources/location.png"});
    nodeData->fieldData.push_back(NodeFieldData{"speed",QString::number(nodeData->speed), "Location Information","qrc:/Resources/location.png"});
    return nodeData;
}
