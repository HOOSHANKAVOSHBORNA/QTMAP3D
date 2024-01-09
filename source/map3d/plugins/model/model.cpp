
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
    mIconNode.release();
}

bool Model::setup()
{

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(mapItem(), &MapItem::modeChanged, this, &Model::onModeChanged);
    mIs3D = mapItem()->getMode();

    mDataManager = new DataManager(mapItem(), mainWindow());
    //    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(serviceManager(), &ServiceManager::nodeDataReceived, mDataManager, &DataManager::nodeDataReceived);

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
    mSimpleNodeLayer->setName("Fixed");

    mMoveableNodeLayer = new ParenticAnnotationLayer();
    mMoveableNodeLayer->setName("Moving");

    mFlyableNodelLayer = new ParenticAnnotationLayer();
    mFlyableNodelLayer->setName("Flying");

    // property item setup
    mProperty = new Property(mapItem());

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
    if(mPickModelNode){
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
        mNodeData = new NodeData;
        mNodeData->id = 0;
        mNodeData->name = "Tree";
        mNodeData->type = NodeType::Fixed;
        mNodeData->category = "User";
        mNodeData->url2D = "../data/models/tree/tree.png";
        mNodeData->url3D = "../data/models/tree/tree.osgb";
        mNodeData->imgInfoUrl = "qrc:/resources/tree.png";
        mNodeData->iconInfoUrl = "qrc:/resources/tree.png";
        mNodeData->color = QColorConstants::Green.name();

        makeIconNode("../data/models/tree/tree.png");
        mType = Type::SIMPLE;
        mState = State::READY;
        mapItem()->addNode(iconNode());

        mProperty->setModelNode(nullptr);
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
//        mBaseNodeData = sampleNodeData("Car",
//                                   "../data/models/car/car.png",
//                                   "../data/models/car/car.osgb",
//                                   "../data/models/airplane/airplane.png",
//                                   "qrc:/resources/car.png");
        makeIconNode("../data/models/car/car.png");
        mType = Type::MOVEABLE;
        mState = State::READY;
        mapItem()->addNode(iconNode());
        mProperty->setModelNode(nullptr);
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
//        mBaseNodeData = sampleNodeData("Airplane",
//                                   "../data/models/airplane/airplane.png",
//                                   "../data/models/airplane/airplane.osgb",
//                                   "../data/models/airplane/airplane.png",
//                                   "qrc:/resources/airplane.png");
        makeIconNode("../data/models/airplane/airplane.png");
        mType = Type::FLYABLE;
        mState = State::READY;
        mapItem()->addNode(iconNode());
        mProperty->setModelNode(nullptr);
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
//        mBaseNodeData = sampleNodeData("Tank",
//                                   "../data/models/tank/tank.png",
//                                   "../data/models/tank/tank.osg",
//                                   "../data/models/tank/tank.png",
//                                   "qrc:/resources/tank.png");
//        mBaseNodeData->isAttacker = true;

        makeIconNode("../data/models/tank/tank.png");
        mType = Type::MOVEABLE;
        mState = State::READY;
        mapItem()->addNode(iconNode());
    }
    else {
        if(mState == State::MOVING)
            cancel();
        mState = State::NONE;
        mapItem()->removeNode(iconNode());
    }
}

void Model::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
}

void Model::initModel(const osgEarth::GeoPoint &geoPos)
{
//    mNodeData = new NodeData();
//    mNodeData->name = mBaseNodeData->name;
//    mNodeData->url2D = mBaseNodeData->url2D;
//    mNodeData->url3D = mBaseNodeData->url3D;
////    mNodeData->imgSrc = mBaseNodeData->imgSrc;
////    mNodeData->iconSrc = mBaseNodeData->iconSrc;
//    mNodeData->color = mBaseNodeData->color;
//    mNodeData->isAttacker = mBaseNodeData->isAttacker;

    mNodeData->longitude = geoPos.x();
    mNodeData->latitude = geoPos.y();
    mNodeData->altitude = geoPos.z();
//    mNodeData->name = mNodeData->name + std::to_string(mCount);

    switch (mType) {
    case Type::SIMPLE:

        mNodeData->id = 100 + mCount;

        if (!mModelNodeLayer->containsLayer(mSimpleNodeLayer)) {
            mSimpleNodeLayer->clear();
            mModelNodeLayer->addLayer(mSimpleNodeLayer);
        }
//        mNodeData->layers.push_back(mSimpleNodeLayer);
//        mCurrentModel = mDataManager->addUpdateNode(mNodeData);
        break;
    case Type::MOVEABLE:

        mNodeData->id = 200 + mCount;
        if (!mModelNodeLayer->containsLayer(mMoveableNodeLayer)) {
            mMoveableNodeLayer->clear();
            mModelNodeLayer->addLayer(mMoveableNodeLayer);
        }
//        mNodeData->layers.push_back(mMoveableNodeLayer);
//        mCurrentModel = mDataManager->addUpdateMovableNode(mNodeData);
        break;
    case Type::FLYABLE:

        mNodeData->id = 300 + mCount;
        if (!mModelNodeLayer->containsLayer(mFlyableNodelLayer)) {
            mFlyableNodelLayer->clear();
            mModelNodeLayer->addLayer(mFlyableNodelLayer);
        }
//        mNodeData->layers.push_back(mFlyableNodelLayer);
//        mCurrentModel = mDataManager->addUpdateFlyableNode(mNodeData);
        break;
    default:
        break;
    }
    mProperty->setModelNode(mCurrentModel);
    mState = State::MOVING;
    mCount++;
}

void Model::moving(osgEarth::GeoPoint &geoPos)
{
    if (mCurrentModel) {
        if (mCurrentModel->asFlyableModelNode()) {
            double randomHeight = 50 + (QRandomGenerator::global()->generate() % (100 - 50));
            geoPos.z() += randomHeight;
//            mCurrentModel->asFlyableModelNode()->flyTo(geoPos, 20);
            mProperty->flyTo(geoPos);
            return;
        }

        if (mCurrentModel->asMoveableModelNode()) {
//            mCurrentModel->asMoveableModelNode()->moveTo(geoPos, 20);
            mProperty->moveTo(geoPos);
            return;
        }

        mCurrentModel->setPosition(geoPos);
        mProperty->setPosition(geoPos);
        qDebug() << "position changed";
    }
}

void Model::confirm()
{
    if (mState == State::MOVING) {
        mState = State::READY;
    }
}

void Model::cancel()
{
    if (mState == State::MOVING) {
        switch (mType) {
        case Type::SIMPLE:
//            mDataManager->removeNode(mNodeData);
            mSimpleNodeLayer->removeChild(mCurrentModel);
            break;
        case Type::MOVEABLE:
//            mDataManager->removeNode(mNodeData);
            mMoveableNodeLayer->removeChild(mCurrentModel);
            break;
        case Type::FLYABLE:
//            mDataManager->removeNode(mNodeData);
            mFlyableNodelLayer->removeChild(mCurrentModel);
            break;
        default:
            break;
        }
        mCurrentModel.release();
        mState = State::READY;
        mCount--;
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

//NodeData *Model::sampleNodeData(std::string name,
//                                std::string url2d,
//                                std::string url3d,
//                                std::string imgSrc,
//                                std::string iconSrc/*,
//                                osgEarth::GeoPoint geoPos*/)
//{
//    NodeData *nodeData = new NodeData();
//    //    flyableNodeData->id = 100;
////    nodeData->name = name;
////    nodeData->type = name;
////    nodeData->longitude = geoPos.x();
////    nodeData->latitude = geoPos.y();
////    nodeData->altitude = geoPos.z();
////    nodeData->url2D = url2d;
////    nodeData->url3D = url3d;
////    nodeData->imgSrc = imgSrc;
////    nodeData->iconSrc = iconSrc;
////    nodeData->color = QColor("white").name().toStdString();
//    nodeData->speed = 100;
//    nodeData->fieldData.push_back(NodeFieldData{"name",
//                                                "Aircraft" + QString::number(mCount),
//                                                "Main Information",
//                                                "qrc:/Resources/exclamation-mark.png"});
//    nodeData->fieldData.push_back(NodeFieldData{"Id",
//                                                QString::number(100 + mCount),
//                                                "Main Information",
//                                                "qrc:/Resources/exclamation-mark.png"});
//    nodeData->fieldData.push_back(NodeFieldData{"Longitude",
//                                                QString::number(nodeData->longitude),
//                                                "Location Information",
//                                                "qrc:/Resources/location.png"});
//    nodeData->fieldData.push_back(NodeFieldData{"Latitude",
//                                                QString::number(nodeData->latitude),
//                                                "Location Information",
//                                                "qrc:/Resources/location.png"});
//    nodeData->fieldData.push_back(NodeFieldData{"Altitude",
//                                                QString::number(nodeData->altitude),
//                                                "Location Information",
//                                                "qrc:/Resources/location.png"});
//    nodeData->fieldData.push_back(NodeFieldData{"speed",
//                                                QString::number(nodeData->speed),
//                                                "Location Information",
//                                                "qrc:/Resources/location.png"});
//    return nodeData;
//}
