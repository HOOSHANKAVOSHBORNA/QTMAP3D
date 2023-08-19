#include "model.h"
#include "mapItem.h"
#include "MoveableModel.h"
#include "flyableModel.h"
#include <osgEarth/GLUtils>

using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;
int model::mCount{0};
model::model(QObject *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(model);
}

model::~model()
{
    mIconNode.release();
}

bool model::setup()
{
//    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());

    mModelLayer = new osgEarth::Annotation::AnnotationLayer();
    mModelLayer->setName(MODEL);
    mapItem()->getMapObject()->addLayer(mModelLayer);


    auto treeToolboxItem =  new ToolboxItem{TREE, MODEL, "qrc:/resources/tree.png", true};
    QObject::connect(treeToolboxItem, &ToolboxItem::itemChecked, this, &model::onTreeItemCheck);
    toolbox()->addItem(treeToolboxItem);

    auto carToolboxItem =  new ToolboxItem{CAR, MODEL, "qrc:/resources/car.png", true};
    QObject::connect(carToolboxItem, &ToolboxItem::itemChecked, this, &model::onCarItemCheck);
    toolbox()->addItem(carToolboxItem);

    auto airplaneToolboxItem =  new ToolboxItem{AIRPLANE, MODEL, "qrc:/resources/airplane.png", true};
    QObject::connect(airplaneToolboxItem, &ToolboxItem::itemChecked, this, &model::onAirplanItemCheck);
    toolbox()->addItem(airplaneToolboxItem);

    mTreelLayer = new osgEarth::Annotation::AnnotationLayer();
    mTreelLayer->setName(TREE);

    mCarlLayer = new osgEarth::Annotation::AnnotationLayer();
    mCarlLayer->setName(CAR);

    mAirplanelLayer = new osgEarth::Annotation::AnnotationLayer();
    mAirplanelLayer->setName(AIRPLANE);
    return true;
}

void model::makeIconNode(const QString &fileName)
{
    osg::Image* icon = osgDB::readImageFile(fileName.toStdString());
    if(icon){
        icon->scaleImage(24, 24, icon->r());
        mIconNode = new osgEarth::Annotation::PlaceNode();
        mIconNode->setIconImage(icon);
    }
}

osgEarth::Annotation::PlaceNode *model::iconNode() const
{
    return mIconNode.get();
}

model::State model::state() const
{
    return mState;
}

void model::setState(State newState)
{
    mState = newState;
}

osgEarth::Annotation::AnnotationLayer *model::modelLayer()
{
    if(!mModelLayer)
        mModelLayer = dynamic_cast<osgEarth::Annotation::AnnotationLayer*>
            (mapItem()->getMapObject()->getLayerByName(MODEL));
    return mModelLayer;
}

bool model::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
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
        confirm();
        return false;
    }
    return false;
}

bool model::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
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

osgEarth::Symbology::Style &model::getDefaultStyle()
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

void model::onTreeItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/tree.png");
        mModelNode = new simpleModelNode(mapItem(),"../data/models/tree_I.osgb", "../data/images/model/tree.png");
        if(mTreelLayer->getGroup()->getNumChildren() <= 0){
            auto sModelLayer = modelLayer();
            mapItem()->getMapObject()->addLayer(mTreelLayer, sModelLayer);
        }
        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        if(mTreelLayer->getGroup()->getNumChildren() <= 0){
            auto sModelLayer = modelLayer();
            mapItem()->getMapObject()->removeLayer(mTreelLayer, sModelLayer);
        }
        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}

void model::onCarItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/car.png");
        mModelNode = new MoveableModel(mapItem(),"../data/models/truck.osgb", "../data/images/model/car.png");

        if(mCarlLayer->getGroup()->getNumChildren() <= 0){
            auto sModelLayer = modelLayer();
            mapItem()->getMapObject()->addLayer(mCarlLayer, sModelLayer);
        }
        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        if(mCarlLayer->getGroup()->getNumChildren() <= 0){
            auto sModelLayer = modelLayer();
            mapItem()->getMapObject()->removeLayer(mCarlLayer, sModelLayer);
        }
        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}

void model::onAirplanItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/images/model/airplane.png");
        mModelNode = new flyableModel(mapItem(),"../data/models/aircraft/boeing-747.osgb", "../data/images/model/airplane.png");
        if(mAirplanelLayer->getGroup()->getNumChildren() <= 0){
            auto sModelLayer = modelLayer();
            mapItem()->getMapObject()->addLayer(mAirplanelLayer, sModelLayer);
        }
        setState(State::READY);
        mapItem()->addNode(iconNode());

    }
    else {
        if(state() == State::MOVING)
            cancel();

        if(mAirplanelLayer->getGroup()->getNumChildren() <= 0){
            auto sModelLayer = modelLayer();
            mapItem()->getMapObject()->removeLayer(mAirplanelLayer, sModelLayer);
        }
        setState(State::NONE);
        mapItem()->removeNode(iconNode());
    }
}

void model::initModel(const osgEarth::GeoPoint &geoPos){
    //QString name = "box" + QString::number(mCount);
    //mBox->setName(name.toStdString());
    mCurrentModel = mModelNode->getNewModel();
    mCurrentModel->setPosition(geoPos);
//    mapItem()->getMapObject()->addNodeToLayer(mCurrentModel, mModelLayer);
    mModelLayer->addChild(mCurrentModel);
    //    mBoxProperties->setBox(mBox);

    setState(State::MOVING);
    mCount++;

}

void model::moving(const osgEarth::GeoPoint &geoPos){

    //mCurrentModel->setPosition(geoPos);

    auto moveableModell = dynamic_cast<MoveableModel*>(mCurrentModel.get());
    if (moveableModell){
        moveableModell->moveTo(geoPos,5);
    }
}

void model::confirm()
{
    if (state() == State::MOVING) {
        setState(State::READY);
    }
}

void model::cancel(){

    if(state() == State::MOVING){
//        mapItem()->getMapObject()->removeNodeFromLayer(mCurrentModel, mModelLayer);
//        mCurrentModel.release();
        setState(State::READY);
        mCount--;
    }
}
