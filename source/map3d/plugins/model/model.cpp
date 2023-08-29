#include "model.h"
#include "mapItem.h"
#include "MoveableModelNode.h"
#include "flyableModelNode.h"
#include "serviceManager.h"
#include <osgEarth/GLUtils>
#include <QRandomGenerator>
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
    //    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(serviceManager(), &ServiceManager::flyableAdded, this, &Model::addFlyable);
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

bool Model::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;

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

    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::MOVING)) {
        cancel();
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::MOVING)) {
        mCurrentModel->setScalability(false);
        confirm();
        return false;
    }
    return false;
}

bool Model::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mState == State::NONE)
        return false;
    //move icon-----------------------
    if(mIconNode){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }


    //    if(mState == State::MOVING){
    //        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
    //        moving(geoPos);
    //        return true;
    //    }
    //--------------------------------
    return false;
}

osgEarth::Symbology::Style &Model::getDefaultStyle()
{
    static osgEarth::Symbology::Style _style;
    static bool bFirst = true;
    if (bFirst) {
        static osg::Node *node = new osg::Node;
        _style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);
        bFirst = false;
    }

    return _style;
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
        makeIconNode("../data/images/model/airplane.png");

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

void Model::addFlyable(ServiseModel *model, ParenticAnnotationLayer *layer)
{
    QString name = "Airplane88";
    FlyableModelNode *fmodel = new FlyableModelNode(mapItem(),"../data/models/aircraft/boeing-747.osgb", "../data/images/model/airplane.png");
//    fmodel->setUserData()
    fmodel->setName(name.toStdString());
    fmodel->setPosition(model->position);
    qDebug() << model->position.toString();
    layer->addChild(fmodel);
//    fmodel->setScalability(false);
    confirm();

}

void Model::initModel(const osgEarth::GeoPoint &geoPos){
    QString name;
    switch (mType) {
    case Type::SIMPLE:
        name = "Tree" + QString::number(mCount);
        mCurrentModel = new SimpleModelNode(mapItem(),"../data/models/tree_I.osgb", "../data/images/model/tree.png");
        if(!mModelNodeLayer->containsLayer(mSimpleNodeLayer)){
            mSimpleNodeLayer->clear();
            mModelNodeLayer->addLayer(mSimpleNodeLayer);
        }
        mSimpleNodeLayer->addChild(mCurrentModel);
        break;
    case Type::MOVEABLE:
        name = "Car" + QString::number(mCount);
        mCurrentModel = new MoveableModelNode(mapItem(),"../data/models/car.osgb", "../data/images/model/car.png");
        if(!mModelNodeLayer->containsLayer(mMoveableNodeLayer)){
            mMoveableNodeLayer->clear();
            mModelNodeLayer->addLayer(mMoveableNodeLayer);
        }
        mMoveableNodeLayer->addChild(mCurrentModel);
        break;
    case Type::FLYABLE:
        name = "Airplane" + QString::number(mCount);
        mCurrentModel = new FlyableModelNode(mapItem(),"../data/models/aircraft/boeing-747.osgb", "../data/images/model/airplane.png");
        if(!mModelNodeLayer->containsLayer(mFlyableNodelLayer)){
            mFlyableNodelLayer->clear();
            mModelNodeLayer->addLayer(mFlyableNodelLayer);
        }
        mFlyableNodelLayer->addChild(mCurrentModel);
        break;
    default:
        break;
    }
    mCurrentModel->setName(name.toStdString());
    mCurrentModel->setPosition(geoPos);

    setState(State::MOVING);
    mCount++;

}

void Model::moving(osgEarth::GeoPoint &geoPos){

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

    mCurrentModel->setPosition(geoPos);
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
        default:
            break;
        }
        mCurrentModel.release();
        setState(State::READY);
        mCount--;
    }
}
