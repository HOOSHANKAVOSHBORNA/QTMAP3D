#include "combatModelNode.h"
#include "mapItem.h"
#include <osgEarth/GLUtils>

using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;
int CombatModelNode::mCount{0};
CombatModelNode::CombatModelNode(QObject *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(combatModelNode);

}

CombatModelNode::~CombatModelNode()
{
    mIconNode.release();
}

bool CombatModelNode::setup()
{

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(mapItem(), &MapItem::modeChanged, this, &CombatModelNode::onModeChanged);
    mIs3D = mapItem()->getMode();


    mCombatModelNodeLayer = new CompositeAnnotationLayer();
    mCombatModelNodeLayer->setName(COMBATMODELNODE);
    mapItem()->getMapObject()->addLayer(mCombatModelNodeLayer);


    auto tankToolboxItem =  new ToolboxItem{"Tank", COMBATMODELNODE, "qrc:/resources/tank.png", true};
    QObject::connect(tankToolboxItem, &ToolboxItem::itemChecked, this, &CombatModelNode::onTankItemCheck);
    toolbox()->addItem(tankToolboxItem);

    auto droneToolboxItem =  new ToolboxItem{"Drone", COMBATMODELNODE, "qrc:/resources/drone.png", true};
    QObject::connect(droneToolboxItem, &ToolboxItem::itemChecked, this, &CombatModelNode::onDroneItemCheck);
    toolbox()->addItem(droneToolboxItem);


    mAttackNodeLayer = new ParenticAnnotationLayer();
    mAttackNodeLayer->setName("Attacker");
    mTargetNodeLayer = new ParenticAnnotationLayer();
    mAttackNodeLayer->setName("Target");


    return true;
}

void CombatModelNode::makeIconNode(const QString &fileName)
{
    osg::Image* icon = osgDB::readImageFile(fileName.toStdString());
    if(icon){
        icon->scaleImage(24, 24, icon->r());
        mIconNode = new osgEarth::Annotation::PlaceNode();
        mIconNode->setIconImage(icon);
    }
}

osgEarth::Annotation::PlaceNode *CombatModelNode::iconNode() const
{
    return mIconNode.get();
}

CombatModelNode::State CombatModelNode::state() const
{
    return mState;
}

void CombatModelNode::setState(State newState)
{
    mState = newState;
}

bool CombatModelNode::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
//        SimpleModelNode* modelNode = pick(ea.getX(), ea.getY());
//        if(modelNode){
////            if(modelNode->isAttacker()){
////                mAttackerNode = modelNode;
////                mDragModelNode = modelNode->getAttackManager()->getBulletNode(mBulletID)->getDragModelNode();
////                mapItem()->addNode(mDragModelNode);
////            }else{
////                modelNode->getOutline();
////                if(modelNode->isAttacker()){
////                    modelNode->getAttackManager()->hideNearTargets();
////                }else{
////                    modelNode->getTargetManager()->hideNearAttackers();
////                }
////            }
//            return true;
//        }
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
        if(mState == State::FIRE){
            //            osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
            //            mCurrentModel->getAttackManager()->attackTo(mBulletID,geoPos);
            return true;
        }

    }
    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON ) {
        if(mState == State::MOVING){
            cancel();
        }/*else if(mState == State::NONE){
            SimpleModelNode* modelNode = pick(ea.getX(), ea.getY());
            if(modelNode){
                rightClickMenu(modelNode);
            }
        }*/
        return false;
    }
    //    else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON && (mState == State::ATTACKING)) {
    //        mCurrentModel->getAttackManager()->attackResult(true,mBulletID);
    //        mBulletID = mCurrentModel->getAttackManager()->readyBullet("../data/models/missile/missile.osgb", "../data/models/missile/missile.png");
    //        return false;
    //    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::MOVING)) {
        if(mType == Type::ATTACKER && !(mState == State::FIRE)){
//            mBulletID = mCurrentModel->getAttackManager()->readyBullet("../data/models/missile/missile.osgb", "../data/models/missile/missile.png");
            mState = State::FIRE;
        }else{
            confirm();
        }
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::FIRE)) {
        confirm();
        return false;
    }

    return false;
}


bool CombatModelNode::mouseReleaseEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(ea.getButton() == osgGA::GUIEventAdapter::MouseButtonMask::LEFT_MOUSE_BUTTON && mDragModelNode)
    {
        SimpleModelNode* targetModelNode = pick(ea.getX(), ea.getY());

        if(targetModelNode)
        {
            mTargetNode = targetModelNode;
            osgEarth::GeoPoint geoPos = targetModelNode->getPosition();
//            mAttackerNode->getAttackManager()->attackTo(mBulletID,geoPos);
        }
        mapItem()->removeNode(mDragModelNode);
        mDragModelNode = nullptr;
    }
    return false;
}

bool CombatModelNode::mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    //move icon-----------------------
    if(mIconNode){
        osgEarth::GeoPoint geoPos = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mIconNode->setPosition(geoPos);
    }

    return false;
}

bool CombatModelNode::frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mTargetNode){
//        if(mAttackerNode->getAttackManager()->getBulletPosition(mBulletID) == mTargetNode->getPosition()){
//            mAttackerNode->getAttackManager()->attackResult(true,mBulletID);
//            mTargetNode.release();
//        }
    }
    return false;
}

bool CombatModelNode::mouseDragEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mDragModelNode)
    {
        osgEarth::GeoPoint mouseGeoPoint = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mDragModelNode->setPosition(mouseGeoPoint);
    }
    return false;
}


osgEarth::Symbology::Style &CombatModelNode::getDefaultStyle()
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


void CombatModelNode::onTankItemCheck(bool check)
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

void CombatModelNode::onDroneItemCheck(bool check)
{
    if (check) {
        makeIconNode("../data/models/drone/drone.png");
        mType = Type::TARGET;
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


void CombatModelNode::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
}

void CombatModelNode::initModel(osgEarth::GeoPoint &geoPos){
    QString name;
    switch (mType) {
    case Type::ATTACKER:
        mAttackerNode = new MoveableModelNode(mapItem(),"../data/models/tank/tank.osg","../data/models/tank/tank.png");
        if(!mCombatModelNodeLayer->containsLayer(mAttackNodeLayer)){
            mAttackNodeLayer->clear();
            mCombatModelNodeLayer->addLayer(mAttackNodeLayer);
        }
        mAttackNodeLayer->addChild(mAttackerNode);
        mAttackerNode->setPosition(geoPos);
        mAttackerNode->setAttacker(true);
        mCurrentModel = mAttackerNode;
        break;
    case Type::TARGET:
        mTargetNode = new FlyableModelNode(mapItem(),"../data/models/drone/drone.osgb","../data/models/drone/drone.png");
        if(!mCombatModelNodeLayer->containsLayer(mTargetNodeLayer)){
            mTargetNodeLayer->clear();
            mCombatModelNodeLayer->addLayer(mTargetNodeLayer);
        }
        mTargetNodeLayer->addChild(mTargetNode);
        geoPos.z() = 500;
        mTargetNode->setPosition(geoPos);
        mTargetNode->setAttacker(false);
        mCurrentModel = mTargetNode;
        break;
    default:
        break;
    }
    setState(State::MOVING);
    mCount++;
}

void CombatModelNode::moving(osgEarth::GeoPoint &geoPos){

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


void CombatModelNode::confirm()
{
    if (state() == State::MOVING || state() == State::FIRE) {
        setState(State::READY);
    }
}

void CombatModelNode::cancel(){

    if(state() == State::MOVING){
        switch (mType) {
        case Type::ATTACKER:
            mAttackNodeLayer->removeChild(mCurrentModel);
            break;
        case Type::TARGET:
            mTargetNodeLayer->removeChild(mCurrentModel);
            break;
        default:
            break;
        }
        mCurrentModel.release();
        setState(State::READY);
        mCount--;
    }
}

SimpleModelNode *CombatModelNode::pick(float x, float y)
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
//                    ss->setAttributeAndModes(new osg::PolygonMode(
//                        osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE));

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

//NodeData *CombatModelNode::sampleNodeData(std::string name, std::string url2d, std::string url3d, std::string imgSrc,
//                                osgEarth::GeoPoint geoPos)
//{
//    NodeData* nodeData = new NodeData();
//    //    flyableNodeData->id = 100;
//    nodeData->name = name + std::to_string(mCount);
//    nodeData->longitude = geoPos.x();
//    nodeData->latitude = geoPos.y();
//    nodeData->altitude = geoPos.z();
//    nodeData->url2D = url2d;
//    nodeData->url3D = url3d;
//    nodeData->imgSrc = imgSrc;
//    nodeData->color = QColor("white").name().toStdString();
//    nodeData->speed = 100;
//    nodeData->fieldData.push_back(NodeFieldData{"name", "Aircraft" + QString::number(mCount), "Main Information","qrc:/Resources/exclamation-mark.png"});
//    nodeData->fieldData.push_back(NodeFieldData{"Id",QString::number(100 + mCount), "Main Information","qrc:/Resources/exclamation-mark.png"});
//    nodeData->fieldData.push_back(NodeFieldData{"Longitude",QString::number(nodeData->longitude), "Location Information","qrc:/Resources/location.png"});
//    nodeData->fieldData.push_back(NodeFieldData{"Latitude",QString::number(nodeData->latitude), "Location Information","qrc:/Resources/location.png"});
//    nodeData->fieldData.push_back(NodeFieldData{"Altitude",QString::number(nodeData->altitude), "Location Information","qrc:/Resources/location.png"});
//    nodeData->fieldData.push_back(NodeFieldData{"speed",QString::number(nodeData->speed), "Location Information","qrc:/Resources/location.png"});
//    return nodeData;
//}

void CombatModelNode::rightClickMenu(SimpleModelNode *selectedNode)
{

    osgEarth::GeoPoint baseModelPosition = selectedNode->getPosition();
//    if(selectedNode->isAttacker()){
//        for (int var = 0; var < mDataManager->nodeCount(); ++var) {
//            SimpleModelNode* nearModel = mDataManager->getNodeAtIndex(var);
//            if(nearModel->getPosition().distanceTo(baseModelPosition) < 15000 && nearModel !=selectedNode){
////                selectedNode->getAttackManager()->setNearTargets(nearModel);
////                nearModel->getTargetManager()->setNearAttacker(selectedNode);
////                selectedNode->getAttackManager()->showNearTargets();
//            }
//        }
//    }else{
//        for (int var = 0; var < mDataManager->nodeCount(); ++var) {
//            SimpleModelNode* nearModel = mDataManager->getNodeAtIndex(var);
//            if(nearModel->isAttacker() && nearModel->getPosition().distanceTo(baseModelPosition) < 15000 && nearModel !=selectedNode){
//                selectedNode->getTargetManager()->setNearAttacker(nearModel);
//                nearModel->getAttackManager()->setNearTargets(selectedNode);
//                selectedNode->getTargetManager()->showNearAttackers();
//            }
//        }
//    }
}



