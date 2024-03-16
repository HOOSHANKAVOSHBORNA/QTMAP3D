
#include <QPainter>
#include <QRandomGenerator>
#include <bookmark.h>
#include <osg/ShapeDrawable>
#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarth/ModelSource>
#include <osgEarthSymbology/Style>
#include <osgEarthSymbology/StyleSheet>
#include <osgFX/Outline>

#include "mainwindow.h"
#include "mapItem.h"
#include "model.h"
#include "property.h"
#include "serviceManager.h"

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
    qDebug() << "~Model";
//    mIconNode.release();
}

bool Model::setup()
{

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    //--mapItem setting-----------------------------------------------------------------
    connect(mapItem(), &MapItem::modeChanged, this, &Model::onModeChanged);
    mIs3D = mapItem()->getMode();
    //--dataManager setting-------------------------------------------------------------
    mDataManager = new DataManager(qmlEngine(), mapItem(), mainWindow(), this);
    connect(mDataManager, &DataManager::nodeRemoved, this, &Model::onNodeRemoved);
    //--serviceManager setting---------------------------------------------------------
    connect(serviceManager(), &ServiceManager::nodeDataReceived, mDataManager, &DataManager::onNodeDataReceived);

    //--toolbox item---------------------------------------------------------------------
    auto treeToolboxItem =  new ToolboxItem{TREE, MODEL, "qrc:/resources/tree.png", true, this};
    QObject::connect(treeToolboxItem, &ToolboxItem::itemChecked, this, &Model::onTreeItemCheck);
    toolbox()->addItem(treeToolboxItem);

    auto carToolboxItem =  new ToolboxItem{CAR, MODEL, "qrc:/resources/car.png", true, this};
    QObject::connect(carToolboxItem, &ToolboxItem::itemChecked, this, &Model::onCarItemCheck);
    toolbox()->addItem(carToolboxItem);

    auto airplaneToolboxItem =  new ToolboxItem{AIRPLANE, MODEL, "qrc:/resources/airplane.png", true, this};
    QObject::connect(airplaneToolboxItem, &ToolboxItem::itemChecked, this, &Model::onAirplanItemCheck);
    toolbox()->addItem(airplaneToolboxItem);

    auto tankToolboxItem =  new ToolboxItem{"Tank", MODEL, "qrc:/resources/tank.png", true, this};
    QObject::connect(tankToolboxItem, &ToolboxItem::itemChecked, this, &Model::onTankItemCheck);
    toolbox()->addItem(tankToolboxItem);
    //--layer data---------------------------------------------------------------------------
    mLayerData.id = layerId();
    mLayerData.text = "Model Node";
    mLayerData.parentId = -1;
    mLayerData.order = 1000;
    mLayerData.isComposite = true;
    mLayerData.command = Command::Add;

    LayerData layerDataFixed;
    layerDataFixed.id = layerId();
    layerDataFixed.text = "Fixed";
    layerDataFixed.parentId = mLayerData.id;
    layerDataFixed.order = 1;
    layerDataFixed.isComposite = false;
    layerDataFixed.command = Command::Remove;
    mLayerData.children.push_back(layerDataFixed);

    LayerData layerDataMovable;
    layerDataMovable.id = layerId();
    layerDataMovable.text = "Movable";
    layerDataMovable.parentId = mLayerData.id;
    layerDataMovable.order = 1;
    layerDataFixed.isComposite = false;
    layerDataMovable.command = Command::Remove;
    mLayerData.children.push_back(layerDataMovable);

    LayerData layerDataFlyable;
    layerDataFlyable.id = layerId();
    layerDataFlyable.text = "Flyable";
    layerDataFlyable.parentId = mLayerData.id;
    layerDataFlyable.order = 1;
    layerDataFixed.isComposite = false;
    layerDataFlyable.command = Command::Remove;
    mLayerData.children.push_back(layerDataFlyable);
    //--property setting------------------------------------------------------------------
    mProperty = new Property(qmlEngine(), mapItem(), this);
    connect(mProperty, &Property::nodeDataChanged, mDataManager, &DataManager::onNodeDataReceived);
    //--list window setting--------------------------------------------------------------
    mNodeList = new NodeList(qmlEngine(), mapItem(), mDataManager, this);
    mainWindow()->addTabToListWindow("Node List", mNodeList->qmlItem());

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

bool Model::mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    //--select model and show menu------------------------------------------
    if(mPickModelNode.valid()){
        mPickModelNode->select(false);
        mPickModelNode->showMenu(false);
    }

    SimpleModelNode *modelNode = pick(ea.getX(), ea.getY());
    if(modelNode){
        mPickModelNode = modelNode;
        if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON){
            modelNode->select(true);
            return false;
        }
        else if(ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON){
            modelNode->showMenu(true);
            modelNode->select(true);
            return false;
        }
    }

    //--add model----------------------------------------------------------
    if (mState == State::NONE) {
        return false;
    }

    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
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
    } else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && mState == State::MOVING) {
        cancel();
        return true;
    } else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && mState == State::MOVING) {
        confirm();
        return true;
    }

    return false;
}



bool Model::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    //    if(mHighliteModelNode){
    //        mHighliteModelNode->highlight(false);
    //    }
    //    SimpleModelNode *modelNode = pick(ea.getX(), ea.getY());
    //    if(modelNode){
    //        mHighliteModelNode = modelNode;
    //        modelNode->highlight(true);
    //    }
    //move icon-----------------------
    if(mIconNode){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }

    return false;
}

bool Model::mouseDragEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    mapItem()->getCameraController()->untrack();
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

DataManager *Model::getDataManager()
{
    return mDataManager;
}

void Model::onTreeItemCheck(bool check)
{
    if (check) {
        mNodeData.id = 0;
        mNodeData.name = "Tree";
        mNodeData.type = NodeType::Fixed;
        mNodeData.category = "User";
        mNodeData.url2D = "../data/models/tree/tree.png";
        mNodeData.url3D = "../data/models/tree/tree.osgb";
        mNodeData.imgInfoUrl = "qrc:/resources/tree.png";
        mNodeData.iconInfoUrl = "qrc:/resources/tree.png";
        mNodeData.color = QColorConstants::Green.name();
        mNodeData.command = Command::Add;
        mNodeData.layersId.clear();
        mNodeData.layersId.push_back(mLayerData.children[NodeType::Fixed].id);

        makeIconNode("../data/models/tree/tree.png");
        mType = NodeType::Fixed;
        mState = State::READY;
        mapItem()->addNode(iconNode());

        mainWindow()->getToolboxManager()->addPropertyItem(mProperty->qmlItem(), "Tree");
    } else {
        if (mState == State::MOVING)
            cancel();

        mState = State::NONE;
        mapItem()->removeNode(iconNode());
        mainWindow()->getToolboxManager()->removePropertyItem();
    }
}

void Model::onCarItemCheck(bool check)
{
    if (check) {

        mNodeData.id = 0;
        mNodeData.name = "Car";
        mNodeData.type = NodeType::Movable;
        mNodeData.category = "User";
        mNodeData.url2D = "../data/models/car/car.png";
        mNodeData.url3D = "../data/models/car/car.osgb";
        mNodeData.imgInfoUrl = "qrc:/resources/car.png";
        mNodeData.iconInfoUrl = "qrc:/resources/car.png";
        mNodeData.color = QColorConstants::White.name();
        mNodeData.command = Command::Add;
        mNodeData.layersId.clear();
        mNodeData.layersId.push_back(mLayerData.children[NodeType::Movable].id);

        makeIconNode("../data/models/car/car.png");
        mType = NodeType::Movable;
        mState = State::READY;
        mapItem()->addNode(iconNode());
        mainWindow()->getToolboxManager()->addPropertyItem(mProperty->qmlItem(), "Car");
    } else {
        if (mState == State::MOVING)
            cancel();

        mState = State::NONE;
        mapItem()->removeNode(iconNode());
        mainWindow()->getToolboxManager()->removePropertyItem();
    }
}

void Model::onAirplanItemCheck(bool check)
{
    if (check) {

        mNodeData.id = 0;
        mNodeData.name = "Airplane";
        mNodeData.type = NodeType::Flyable;
        mNodeData.category = "User";
        mNodeData.url2D = "../data/models/airplane/airplane.png";
        mNodeData.url3D = "../data/models/airplane/airplane.osgb";
        mNodeData.imgInfoUrl = "../data/models/airplane/airplane.png";
        mNodeData.iconInfoUrl = "qrc:/resources/airplane.png";
        mNodeData.color = QColorConstants::White.name();
        mNodeData.command = Command::Add;
        mNodeData.layersId.clear();
        mNodeData.layersId.push_back(mLayerData.children[NodeType::Flyable].id);

        makeIconNode("../data/models/airplane/airplane.png");
        mType = NodeType::Flyable;
        mState = State::READY;
        mapItem()->addNode(iconNode());
        mainWindow()->getToolboxManager()->addPropertyItem(mProperty->qmlItem(), "Airplane");
    } else {
        if (mState == State::MOVING)
            cancel();

        mState = State::NONE;
        mapItem()->removeNode(iconNode());
        mainWindow()->getToolboxManager()->removePropertyItem();
    }
}

void Model::onTankItemCheck(bool check)
{
    if (check) {

        mNodeData.id = 0;
        mNodeData.name = "Tank";
        mNodeData.type = NodeType::Movable;
        mNodeData.isAttacker = true;
        mNodeData.category = "User";
        mNodeData.url2D = "../data/models/tank/tank.png";
        mNodeData.url3D = "../data/models/tank/tank.osgb";
        mNodeData.imgInfoUrl = "../data/models/tank/tank.png";
        mNodeData.iconInfoUrl = "qrc:/resources/tank.png";
        mNodeData.color = QColorConstants::White.name();
        mNodeData.command = Command::Add;
        mNodeData.layersId.clear();
        mNodeData.layersId.push_back(mLayerData.children[NodeType::Movable].id);

        makeIconNode("../data/models/tank/tank.png");
        mType = NodeType::Movable;
        mState = State::READY;
        mapItem()->addNode(iconNode());
        mainWindow()->getToolboxManager()->addPropertyItem(mProperty->qmlItem(), "Tank");
    }
    else {
        if(mState == State::MOVING)
            cancel();
        mState = State::NONE;
        mapItem()->removeNode(iconNode());
        mainWindow()->getToolboxManager()->removePropertyItem();
    }
}

void Model::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
}

void Model::initModel(const osgEarth::GeoPoint &geoPos)
{
    mNodeData.id = nodeId();
    mNodeData.name = mNodeData.name + QString::number(mNodeData.id);
    mNodeData.longitude = geoPos.x();
    mNodeData.latitude = geoPos.y();
    mNodeData.altitude = geoPos.z();
    mNodeData.color = mProperty->nodeData().color;
    mNodeData.speed = mProperty->nodeData().speed;
    mNodeData.command = Command::Add;
    mNodeData.fieldData.clear();

    NodeFieldData nameField;
    nameField.name = "Name";
    nameField.value = mNodeData.name;
    nameField.category = "Main Information";
    mNodeData.fieldData.push_back(nameField);

    NodeFieldData idField;
    idField.name = "Id";
    idField.value = mNodeData.id;
    idField.category = "Main Information";
    mNodeData.fieldData.push_back(idField);

    NodeFieldData colorField;
    colorField.name = "Color";
    colorField.value = mNodeData.color;
    colorField.category = "Main Information";
    mNodeData.fieldData.push_back(colorField);

    NodeFieldData latField;
    latField.name = "Latitude";
    latField.value = mNodeData.latitude;
    latField.category = "Location Information";
    mNodeData.fieldData.push_back(latField);

    NodeFieldData longField;
    longField.name = "Longitude";
    longField.value = mNodeData.longitude;
    longField.category = "Location Information";
    mNodeData.fieldData.push_back(longField);

    NodeFieldData altField;
    altField.name = "Altitude";
    altField.value = mNodeData.altitude;
    altField.category = "Location Information";
    mNodeData.fieldData.push_back(altField);

    mLayerData.command = Command::Add;
    mLayerData.children[mType.type].command = Command::Add;

    mapItem()->getMapObject()->onLayerDataReceived(mLayerData);
    //    mCurrentModel = mDataManager->onNodeDataReceived(mNodeData);
    mProperty->sethasModel(true);
    mProperty->setNodeData(mNodeData);
    mState = State::MOVING;
    mCount++;
}

void Model::moving(osgEarth::GeoPoint &geoPos)
{
    //    if (mCurrentModel) {
    //        if (mCurrentModel->asFlyableModelNode()) {
    //            double randomHeight = 50 + (QRandomGenerator::global()->generate() % (100 - 50));
    //            geoPos.z() += randomHeight;
    //                // mCurrentModel->asFlyableModelNode()->flyTo(geoPos, 20);
    //            mProperty->flyTo(geoPos);
    //            return;
    //        }

    //        if (mCurrentModel->asMoveableModelNode()) {
    //            // mCurrentModel->asMoveableModelNode()->moveTo(geoPos, 20);
    //            mProperty->moveTo(geoPos);
    //            return;
    //        }

    //        mCurrentModel->setPosition(geoPos);
    //        //        mProperty->setPosition(geoPos);
    //        //        qDebug() << "position changed";
    //    }

    mNodeData.longitude = geoPos.x();
    mNodeData.latitude = geoPos.y();
    mNodeData.altitude = geoPos.z();
    if (mNodeData.type == NodeType::Flyable) {
        double randomHeight = 50 + (QRandomGenerator::global()->generate() % (100 - 50));
        mNodeData.altitude += randomHeight;
    }
    mProperty->setNodeData(mNodeData);
}

void Model::confirm()
{
    if (mState == State::MOVING) {
        mState = State::READY;
        mProperty->sethasModel(false);
    }
}

void Model::cancel()
{

    if (mState == State::MOVING) {
        ParenticAnnotationLayer *layer;
        mNodeData.command = Command::Remove;
        mDataManager->onNodeDataReceived(mNodeData);

        layer  = mapItem()->getMapObject()->getLayerByUserId(mLayerData.children[mType.type].id);
        if(layer && !layer->hasNode()){
            mLayerData.children[mType.type].command = Command::Remove;
            mapItem()->getMapObject()->onLayerDataReceived(mLayerData.children[mType.type]);
        }
        auto parentLayer  = mapItem()->getMapObject()->getLayerByUserId(mLayerData.id);
        if(parentLayer && !parentLayer->asCompositeAnnotationLayer()->hasChild()){
            mLayerData.command = Command::Remove;
            mapItem()->getMapObject()->onLayerDataReceived(mLayerData);
        }
        mProperty->sethasModel(false);
        mState = State::READY;
    }
}

void Model::onNodeRemoved(SimpleModelNode *simpleModelNode)
{
    if(mPickModelNode == simpleModelNode){
        mPickModelNode = nullptr;
    }

    if(mCurrentModel == simpleModelNode){
        mCurrentModel = nullptr;
    }
}

SimpleModelNode *Model::pick(float x, float y)
{
    SimpleModelNode *simpleModelNode = nullptr;
    osgViewer::Viewer *viewer = mapItem()->getViewer();
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (viewer->computeIntersections(x, y, intersections)) {
        for (const osgUtil::LineSegmentIntersector::Intersection &hit : intersections) {
            const osg::NodePath &nodePath = hit.nodePath;
            for (osg::NodePath::const_iterator nitr = nodePath.begin(); nitr != nodePath.end();
                 ++nitr) {
                simpleModelNode = dynamic_cast<SimpleModelNode *>(*nitr);
                if (simpleModelNode) {
                    osg::ref_ptr<osg::MatrixTransform> _selectionBox;
                    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
                    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 1.0f)));
                    _selectionBox = new osg::MatrixTransform;
                    _selectionBox->setNodeMask(0x1);
                    _selectionBox->addChild(geode.get());
                    osg::StateSet *ss = _selectionBox->getOrCreateStateSet();
                    ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
                    ss->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
                                                                  osg::PolygonMode::LINE));

                    osg::BoundingBox bb = hit.drawable->getBoundingBox();
                    //                    qDebug()<<"radius: "<<bb.radius();
                    osg::Vec3 worldCenter = bb.center() * osg::computeLocalToWorld(hit.nodePath);
                    _selectionBox->setMatrix(osg::Matrix::scale(bb.xMax() - bb.xMin(),
                                                                bb.yMax() - bb.yMin(),
                                                                bb.zMax() - bb.zMin())
                                             * osg::Matrix::translate(worldCenter));

                    return simpleModelNode;
                }
            }
        }
    }
    return simpleModelNode;
}
