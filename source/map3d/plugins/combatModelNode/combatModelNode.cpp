#include "combatModelNode.h"
#include "mapItem.h"
#include <osgEarth/GLUtils>
#include <moveableModelNode.h>
#include <flyableModelNode.h>
#include "combatManager.h"

using osgMouseButton = osgGA::GUIEventAdapter::MouseButtonMask;

CombatModelNode::CombatModelNode(QObject *parent)
    :PluginInterface(parent)
{
    Q_INIT_RESOURCE(combatModelNode);
}

CombatModelNode::~CombatModelNode()
{
    mIconNode.release();
}

bool CombatModelNode::setup()
{
    auto modelPlugin = getPlugin("libmodel.so");
    mCombatManager = new CombatManager(mapItem());
    mCombatMenu = new CombatMenu(mCombatManager, mapItem());

    osgEarth::GLUtils::setGlobalDefaults(mapItem()->getViewer()->getCamera()->getOrCreateStateSet());
    connect(mapItem(), &MapItem::modeChanged, this, &CombatModelNode::onModeChanged);
    mIs3D = mapItem()->getMode();

    mCombatModelNodeLayer = new CompositeAnnotationLayer();
    mCombatModelNodeLayer->setName(COMBATMODELNODE);
    mapItem()->getMapObject()->addLayer(mCombatModelNodeLayer);

    mAttackNodeLayer = new ParenticAnnotationLayer();
    mAttackNodeLayer->setName("Attacker");
    mCombatManager->setCombatLayer(mAttackNodeLayer);
    mCombatModelNodeLayer->addLayer(mAttackNodeLayer);

    QObject::connect(mCombatMenu->assignmentListModel(), &AssignmentListModel::addAssignmentChecked, this, &CombatModelNode::onAddAssignmentChecked);
    QObject::connect(mCombatMenu->assignmentListModel(), &AssignmentListModel::removeAssignmentChecked, this, &CombatModelNode::onRemoveAssignmentChecked);
    QObject::connect(mCombatMenu->assignmentListModel(), &AssignmentListModel::onCloseMenuClicked, this, &CombatModelNode::onCloseMenuClicked);

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


bool CombatModelNode::mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{

    SimpleModelNode* modelNode = pick(ea.getX(), ea.getY());
    if(modelNode){
        if (ea.getButton() == osgMouseButton::LEFT_MOUSE_BUTTON) {
            if(mState == State::ADD_ASSIGNMENT){
                if(mOperatorIsAttacker){
                    mCombatManager->assign(mOperatorNode, modelNode);
                    return true;
                }else{
                    mCombatManager->assign(modelNode, mOperatorNode);
                    return true;
                }
            }
            if(mState == State::REMOVE_ASSIGNMENT){
                modelNode->highlight(false);
                if(mOperatorIsAttacker){
                    mCombatManager->removeAssignment(mOperatorNode, modelNode);
                    return true;
                }else{
                    mCombatManager->removeAssignment(modelNode, mOperatorNode);
                    return true;
                }
            }
        }
        else if (ea.getButton() == osgMouseButton::RIGHT_MOUSE_BUTTON ) {
            QObject::disconnect(modelNode, &SimpleModelNode::onAttackChecked, this, &CombatModelNode::onAttackMenuChecked);
            QObject::connect(modelNode, &SimpleModelNode::onAttackChecked, this, &CombatModelNode::onAttackMenuChecked);

            QObject::disconnect(modelNode, &SimpleModelNode::onTargetChecked, this, &CombatModelNode::onTargetMenuChecked);
            QObject::connect(modelNode, &SimpleModelNode::onTargetChecked, this, &CombatModelNode::onTargetMenuChecked);
            return false;
        }
        else if (ea.getButton() == osgMouseButton::MIDDLE_MOUSE_BUTTON) {
            return false;
        }
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



void CombatModelNode::onModeChanged(bool is3DView)
{
    mIs3D = is3DView;
}

void CombatModelNode::onTargetMenuChecked()
{
    SimpleModelNode *currentObjectModel = dynamic_cast<SimpleModelNode*>(QObject::sender());
    if(currentObjectModel){
        mCombatMenu->selectTarget(currentObjectModel);
        mapItem()->setTopMenuVisible(true);
    }
}

void CombatModelNode::onAttackMenuChecked()
{
    SimpleModelNode *currentObjectModel = dynamic_cast<SimpleModelNode*>(QObject::sender());
    if(currentObjectModel){
        mCombatMenu->selectAttacker(currentObjectModel);
        mapItem()->setTopMenuVisible(true);
    }
}

void CombatModelNode::onAddAssignmentChecked(bool check, SimpleModelNode *node, bool isAttacker)
{
    if(check){
        mState = State::ADD_ASSIGNMENT;
        makeIconNode("../data/images/model/add.png");
        mapItem()->addNode(mIconNode);
    }
    else{
        mState = State::NONE;
        mapItem()->removeNode(mIconNode);
        mIconNode.release();
    }
    mOperatorNode = node;
    mOperatorIsAttacker = isAttacker;
}

void CombatModelNode::onRemoveAssignmentChecked(bool check, SimpleModelNode *node, bool isAttacker)
{
    if(check){
        mState = State::REMOVE_ASSIGNMENT;
        makeIconNode("../data/images/model/location-delete.png");
        mapItem()->addNode(mIconNode);
    }
    else{
        mState = State::NONE;
        mapItem()->removeNode(mIconNode);
        mIconNode.release();
    }
    mOperatorNode = node;
    mOperatorIsAttacker = isAttacker;
}

void CombatModelNode::onCloseMenuClicked()
{
    for (int var = 0; var < mCombatMenu->assignmentListModel()->getSelectedAssignmentList().count(); ++var) {
        mCombatMenu->assignmentListModel()->getSelectedAssignmentList().at(var)->attacker->highlight(false);
        mCombatMenu->assignmentListModel()->getSelectedAssignmentList().at(var)->target->highlight(false);
    }
    mapItem()->setTopMenuVisible(false);
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

