#include "model.h"
#include "mapItem.h"
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

    mDataManager = new DataManager(mapItem(), mainWindow());
    //    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(serviceManager(), &ServiceManager::flyableNodeDataReceived, mDataManager, &DataManager::flyableNodeDataReceived);
    connect(serviceManager(), &ServiceManager::nodeDataReceived, mDataManager, &DataManager::nodeDataReceived);
    connect(serviceManager(), &ServiceManager::movableNodeDataReceived, mDataManager, &DataManager::movableNodeDataReceived);

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

    mSimpleNodeLayer = new ParenticAnnotationLayer();
    mSimpleNodeLayer->setName(TREE);

    mMoveableNodeLayer = new ParenticAnnotationLayer();
    mMoveableNodeLayer->setName(CAR);

    mFlyableNodelLayer = new ParenticAnnotationLayer();
    mFlyableNodelLayer->setName(AIRPLANE);


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

bool Model::mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {

        SimpleModelNode* modelNode = pick(ea.getX(), ea.getY());
        if(modelNode) {
            modelNode->select();
            // serviceManager()->sendAction("select");
            return false;
        }

        if(mState == State::NONE){
            return false;
        }
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
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON ) {
        if(mState == State::MOVING){
            cancel();
        }else if(mState == State::NONE){
            SimpleModelNode* modelNode = pick(ea.getX(), ea.getY());
            if(modelNode) {
                modelNode->showMenu();
                return false;
            }
        }
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

void Model::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
}

void Model::initModel(const osgEarth::GeoPoint &geoPos){
    QString name;
    switch (mType) {
    case Type::SIMPLE:
        mNodeData = sampleNodeData("Tree", "../data/models/tree/tree.png", "../data/models/tree/tree.osgb",
                                   "../data/models/airplane/airplane.png", "qrc:/resources/tree.png", geoPos);
        mNodeData->id = 500 + mCount;
        if(!mModelNodeLayer->containsLayer(mSimpleNodeLayer)){
            mSimpleNodeLayer->clear();
            mModelNodeLayer->addLayer(mSimpleNodeLayer);
        }
        mNodeData->layers.push_back(mSimpleNodeLayer);
        mCurrentModel = mDataManager->addUpdateNode(mNodeData);
        break;
    case Type::MOVEABLE:
        mNodeData = sampleNodeData("Car", "../data/models/car/car.png", "../data/models/car/car.osgb",
                                   "../data/models/airplane/airplane.png","qrc:/resources/car.png", geoPos);
        mNodeData->id = 500 + mCount;
        if(!mModelNodeLayer->containsLayer(mMoveableNodeLayer)){
            mMoveableNodeLayer->clear();
            mModelNodeLayer->addLayer(mMoveableNodeLayer);
        }
        mNodeData->layers.push_back(mMoveableNodeLayer);
        mCurrentModel = mDataManager->addUpdateMovableNode(mNodeData);
        break;
    case Type::FLYABLE:
        mNodeData = sampleNodeData("Airplane", "../data/models/airplane/airplane.png", "../data/models/airplane/airplane.osgb",
                                   "../data/models/airplane/airplane.png","qrc:/resources/airplane.png", geoPos);
        mNodeData->id = 500 + mCount;
        if(!mModelNodeLayer->containsLayer(mFlyableNodelLayer)){
            mFlyableNodelLayer->clear();
            mModelNodeLayer->addLayer(mFlyableNodelLayer);
        }
        mNodeData->layers.push_back(mFlyableNodelLayer);
        mCurrentModel = mDataManager->addUpdateFlyableNode(mNodeData);
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
    if (state() == State::MOVING) {
        setState(State::READY);
    }
}

void Model::cancel(){

    if(state() == State::MOVING){
        switch (mType) {
        case Type::SIMPLE:
            mDataManager->removeNodeData(mNodeData);
            mSimpleNodeLayer->removeChild(mCurrentModel);
            break;
        case Type::MOVEABLE:
            mDataManager->removeNodeData(mNodeData);
            mMoveableNodeLayer->removeChild(mCurrentModel);
            break;
        case Type::FLYABLE:
            mDataManager->removeNodeData(mNodeData);
            mFlyableNodelLayer->removeChild(mCurrentModel);
            break;
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

NodeData *Model::sampleNodeData(std::string name, std::string url2d, std::string url3d, std::string imgSrc, std::string iconSrc,
                                osgEarth::GeoPoint geoPos)
{
    NodeData* nodeData = new NodeData();
    //    flyableNodeData->id = 100;
    nodeData->name = name + std::to_string(mCount);
    nodeData->type = name;
    nodeData->longitude = geoPos.x();
    nodeData->latitude = geoPos.y();
    nodeData->altitude = geoPos.z();
    nodeData->url2D = url2d;
    nodeData->url3D = url3d;
    nodeData->imgSrc = imgSrc;
    nodeData->iconSrc = iconSrc;
    nodeData->color = QColor("white").name().toStdString();
    nodeData->speed = 100;
    nodeData->fieldData.push_back(NodeFieldData{"name", "Aircraft" + QString::number(mCount), "Main Information","qrc:/Resources/exclamation-mark.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Id",QString::number(100 + mCount), "Main Information","qrc:/Resources/exclamation-mark.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Longitude",QString::number(nodeData->longitude), "Location Information","qrc:/Resources/location.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Latitude",QString::number(nodeData->latitude), "Location Information","qrc:/Resources/location.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Altitude",QString::number(nodeData->altitude), "Location Information","qrc:/Resources/location.png"});
    nodeData->fieldData.push_back(NodeFieldData{"speed",QString::number(nodeData->speed), "Location Information","qrc:/Resources/location.png"});
    return nodeData;
}



