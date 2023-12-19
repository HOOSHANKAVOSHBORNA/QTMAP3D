#include "combatModelNode.h"
#include "mapItem.h"
#include <osgEarth/GLUtils>
#include <moveableModelNode.h>
#include <flyableModelNode.h>
#include "combatManager.h"
#include "combatListModel.h"

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
    mEngine = QQmlEngine::contextForObject(mapItem())->engine();
    mCombatManager = new CombatManager(mapItem());
    mCombatList = new CombatList(mEngine,mapItem());
    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(mapItem(), &MapItem::modeChanged, this, &CombatModelNode::onModeChanged);
    mIs3D = mapItem()->getMode();

    mCombatModelNodeLayer = new CompositeAnnotationLayer();
    mCombatModelNodeLayer->setName(COMBATMODELNODE);
    mapItem()->getMapObject()->addLayer(mCombatModelNodeLayer);

    mDataManager = new DataManager(mapItem(), mainWindow());

    auto tankToolboxItem =  new ToolboxItem{"Tank", COMBATMODELNODE, "qrc:/resources/tank.png", true};
    QObject::connect(tankToolboxItem, &ToolboxItem::itemChecked, this, &CombatModelNode::onTankItemCheck);
    toolbox()->addItem(tankToolboxItem);

    mAttackNodeLayer = new ParenticAnnotationLayer();
    mAttackNodeLayer->setName("Attacker");
    mCombatManager->setCombatLayer(mAttackNodeLayer);

    QObject::connect(mCombatList->getCombatModel(),&CombatListModel::addManuallyChecked,this,&CombatModelNode::onAddManuallyChecked);


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

bool CombatModelNode::mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    SimpleModelNode* modelNode = pick(ea.getX(), ea.getY());
    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        if(mState == State::NONE){
            return false;
        }
        if(mState == State::MANUAL){
            if(mCurrentModel->isAttacker()){
                mCombatManager->assign(mCurrentModel,modelNode);
            }else{
                mCombatManager->assign(modelNode,mCurrentModel);
            }
            mCombatList->getCombatModel()->addData(mCombatManager->getAssignmentData()->last());
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
        }else if(mState == State::NONE && !mCombatManager->getAssignmentData()->empty()){
            for (int var = 0; var < mCombatManager->getAssignmentData()->count(); ++var) {
                // int bulletID = mCombatManager->readyBulletFor(mCombatManager->getAssignmentData()->(var).attacker,"../data/models/missile/missile.osgb", "../data/models/missile/missile.png");
                // mCombatManager->attackTo(bulletID,mCombatManager->getAssignmentData()->at(var).target->getPosition());
                // mCombatManager->setBulletTargetModel(bulletID,mCombatManager->getAssignmentData()->at(var).target);
                // mBulletID.append(bulletID);
            }
        }
        QObject::connect(modelNode, &SimpleModelNode::onAttackChecked, this, &CombatModelNode::onAttackMenuChecked);
        QObject::connect(modelNode, &SimpleModelNode::onTargetChecked, this, &CombatModelNode::onTargetMenuChecked);
        return false;
    }
    else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON && (mState == State::MOVING)) {
        confirm();
        return false;
    }


    return false;
}

bool CombatModelNode::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    SimpleModelNode* modelNode = pick(ea.getX(), ea.getY());
    if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
        if(modelNode){
            if(modelNode->isAttacker()){
                mAttackerNode = modelNode;
                mDragModelNode = getDragModel();
                mapItem()->addNode(mDragModelNode);
                return true;
            }
        }
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
            mCombatManager->assign(mAttackerNode,targetModelNode,AssignState::PREASSIGN);
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
    if(!mBulletID.empty()){
        for (int var = 0; var < mBulletID.length(); ++var) {
            if(mCombatManager->getBulletPosition(mBulletID.at(var)) == mCombatManager->getBulletTargetModel(mBulletID.at(var))->getPosition()){
                mCombatManager->attackResult(true,mBulletID.at(var));
                mBulletID.removeAt(var);
            }
        }
    }
    return false;
}

bool CombatModelNode::mouseDragEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mDragModelNode)
    {
        osgEarth::GeoPoint mouseGeoPoint = mapItem()->screenToGeoPoint(ea.getX(), ea.getY());
        mDragModelNode->setPosition(mouseGeoPoint);
        return true;
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

void CombatModelNode::onTargetMenuChecked()
{
    SimpleModelNode *currentObjectModel = dynamic_cast<SimpleModelNode*>(QObject::sender());
    if(currentObjectModel){
        mCombatList->getCombatModel()->setTitle(QString::fromStdString(currentObjectModel->getName()));
        mCombatList->getCombatModel()->setIconUrl(QUrl::fromLocalFile(QString::fromStdString(currentObjectModel->url2D())));
        mCombatList->getCombatModel()->setIsAttacker(false);
        mCombatList->getCombatModel()->setBulletCount(0);
        mCombatList->getCombatModel()->setActorModel(currentObjectModel);
        //--update model------------------------------------------------------------------------------------------
        mCombatList->getCombatModel()->removeRows(0,mCombatList->getCombatModel()->rowCount()-1);
        for (int var = 0; var < mCombatManager->getAssignmentData()->count(); ++var) {
            if(mCombatManager->getAssignmentData()->values().takeAt(var).target == currentObjectModel){
                mCombatList->getCombatModel()->addData(mCombatManager->getAssignmentData()->values().at(var));
            }
        }
        mCombatList->setCombatMenuVisible(true);
    }
}

void CombatModelNode::onAttackMenuChecked()
{
    SimpleModelNode *currentObjectModel = dynamic_cast<SimpleModelNode*>(QObject::sender());
    if(currentObjectModel){
        mCombatList->getCombatModel()->setTitle(QString::fromStdString(currentObjectModel->getName()));
        mCombatList->getCombatModel()->setIconUrl(QUrl::fromLocalFile(QString::fromStdString(currentObjectModel->url2D())));
        mCombatList->getCombatModel()->setIsAttacker(true);
        mCombatList->getCombatModel()->setBulletCount(10);
        mCombatList->getCombatModel()->setActorModel(currentObjectModel);
        //--update model------------------------------------------------------------------------------------------
        mCombatList->getCombatModel()->removeRows(0,mCombatList->getCombatModel()->rowCount()-1);
        for (int var = 0; var < mCombatManager->getAssignmentData()->count(); ++var) {
            if(mCombatManager->getAssignmentData()->values().takeAt(var).attacker == currentObjectModel){
                mCombatList->getCombatModel()->addData(mCombatManager->getAssignmentData()->values().at(var));
            }
        }
        mCombatList->setCombatMenuVisible(true);
    }
}

void CombatModelNode::onAddManuallyChecked()
{
    CombatListModel *senderObject = dynamic_cast<CombatListModel*>(QObject::sender());
    SimpleModelNode *senderModel = senderObject->getActorModel();
    mCurrentModel = senderModel;
    setState(State::MANUAL);
}



void CombatModelNode::initModel(osgEarth::GeoPoint &geoPos){
    mNodeData = sampleNodeData("tank", "../data/models/tank/tank.png", "../data/models/tank/tank.osg",
                               "../data/models/tank/tank.png", "qrc:/resources/tank.png", geoPos);
    mNodeData->id = 500 + mCount;
    if(!mCombatModelNodeLayer->containsLayer(mAttackNodeLayer)){
        mAttackNodeLayer->clear();
        mCombatModelNodeLayer->addLayer(mAttackNodeLayer);
    }
    mNodeData->layers.push_back(mAttackNodeLayer);
    mCurrentModel = mDataManager->addUpdateNode(mNodeData);

    // mAttackerNode = new MoveableModelNode(mapItem(),"../data/models/tank/tank.osg","../data/models/tank/tank.png");
    // mAttackNodeLayer->addChild(mAttackerNode);
    // mCombatManager->setCombatLayer(mAttackNodeLayer);
    // mAttackerNode->setAttacker(true);
    // // mAttackerNode->setColor(osgEarth::Color(0,0,0,1));
    // mAttackerNode->setPosition(geoPos);
    // mCurrentModel = mAttackerNode;
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
    if (state() == State::MOVING) {
        setState(State::READY);
    }
}

void CombatModelNode::cancel(){

    if(state() == State::MOVING){
        mAttackNodeLayer->removeChild(mCurrentModel);
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
                    osg::BoundingBox bb = hit.drawable->getBoundingBox();
//                    qDebug()<<"radius: "<<bb.radius();
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


osgEarth::Annotation::ModelNode *CombatModelNode::getDragModel()
{
    osgEarth::Symbology::Style  style = mCurrentModel->getStyle();
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setDiffuse (osg::Material::FRONT_AND_BACK, osgEarth::Color::Gray);
    osg::ref_ptr<osgEarth::Annotation::ModelNode> dragModelNode = new osgEarth::Annotation::ModelNode(mapItem()->getMapNode() , style);
    dragModelNode->setCullingActive(false);
    dragModelNode->addCullCallback(mCurrentModel->getCullCallback());
    dragModelNode->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    return dragModelNode.release();
}


NodeData* CombatModelNode::sampleNodeData(std::string name, std::string url2d, std::string url3d, std::string imgSrc, std::string iconSrc,osgEarth::GeoPoint geoPos)
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
    nodeData->isAttacker = true;
    nodeData->fieldData.push_back(NodeFieldData{"name", "Tank" + QString::number(mCount), "Main Information","qrc:/Resources/exclamation-mark.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Id",QString::number(100 + mCount), "Main Information","qrc:/Resources/exclamation-mark.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Longitude",QString::number(nodeData->longitude), "Location Information","qrc:/Resources/location.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Latitude",QString::number(nodeData->latitude), "Location Information","qrc:/Resources/location.png"});
    nodeData->fieldData.push_back(NodeFieldData{"Altitude",QString::number(nodeData->altitude), "Location Information","qrc:/Resources/location.png"});
    nodeData->fieldData.push_back(NodeFieldData{"speed",QString::number(nodeData->speed), "Location Information","qrc:/Resources/location.png"});
    return nodeData;
}

