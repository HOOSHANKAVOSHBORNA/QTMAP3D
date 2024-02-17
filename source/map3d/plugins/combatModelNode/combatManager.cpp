#include "combatManager.h"
#include "assignLine.h"

#include <utility.h>

Assignment::Assignment(MapItem *mapItem, SimpleModelNode *attacker, SimpleModelNode *target)
{
    this->attacker = attacker;
    this->target = target;
    assignLine = new AssignLine(mapItem, attacker, target);

}

void Assignment::setState(AssignState state)
{
    this->state = state;
    assignLine->setFillColor(Utility::qColor2osgEarthColor(getColor()));
    assignLine->setPointColor(Utility::qColor2osgEarthColor(getColor()));
}

QColor Assignment::getColor()
{

    switch (state) {
    case PREASSIGN:
       return QColor(255, 255, 255, 150);
    case ASSIGNED:
        return  QColor(250, 140, 0, 150);
    case SEARCH:
        return QColor(250, 250, 0, 150);
    case LOCK:
        return QColor(250, 90, 0, 150);
    case FIRE:
        return QColor(255, 0, 0, 150);
    case BUSY:
        return QColor(150, 150, 150, 150);
    case SUCCEED:
        return QColor(0, 255, 0, 150);
    case FAILED:
        return QColor(120, 20, 20, 150);
    default:
        return QColor(50, 10, 255, 150);
        break;
    }

}
//---------------------------------------------------------------------------------
CombatManager::CombatManager(QQmlEngine *engine, MapItem *map)
{
    // mEdgeDataList = new QList<assignmentData>;
    mQmlEngine = engine;
    mBulletList = new QList<osg::ref_ptr<BulletNode>>;
    mMapItem = map;
}

void CombatManager::setCombatLayer(ParenticAnnotationLayer *layer)
{
    mCombatLayer = layer;
}

ParenticAnnotationLayer *CombatManager::getCombatLayer()
{
    return mCombatLayer;
}

void CombatManager::assign(SimpleModelNode *attacker, SimpleModelNode *target , AssignState state)
{
    if(!(attacker->nodeData().id == target->nodeData().id) && (attacker->isAttacker())){
        Assignment *assignment{nullptr};
        assignment = getAssignment(attacker, target);
        if(!assignment){
            assignment = new Assignment(mMapItem, attacker, target);
            mAssignmentList.append(assignment);
            mCombatLayer->addChild(assignment->assignLine);
        }
        assignment->setState(state);
        emit dataChanged();
    }
}

void CombatManager::removeAssignment(SimpleModelNode *attacker, SimpleModelNode *target)
{
    Assignment *assignment = getAssignment(attacker, target);
    if(assignment){
        mAssignmentList.removeOne(assignment);
        mCombatLayer->removeChild(assignment->assignLine);
        emit dataChanged();
    }
}

void CombatManager::removeAttackerAssignments(SimpleModelNode *attacker)
{
    if(!attacker)
        return;

    for(auto assignment: mAssignmentList)
        if(assignment->attacker->nodeData().id == attacker->nodeData().id){
            removeAssignment(attacker, assignment->target);
            emit dataChanged();
        }
}

void CombatManager::removeTargetAssignments(SimpleModelNode *target)
{
    if(!target)
        return;

    for(auto assignment: mAssignmentList)
        if(assignment->target->nodeData().id == target->nodeData().id){
            removeAssignment(assignment->attacker, target);
            emit dataChanged();
        }
}

int CombatManager::readyBulletFor(SimpleModelNode *attacker, const std::string &url3D, const std::string &url2D)
{
    osg::ref_ptr<BulletNode> bulletnode = new BulletNode(mQmlEngine, mMapItem, url3D, url2D);
    mCombatLayer->addChild(bulletnode);
    bulletnode->setPosition(attacker->getPosition());
    mBulletList->push_back(bulletnode);
    return mBulletList->indexOf(bulletnode);
}

void CombatManager::removeBullet(int bulletID)
{
    if(mBulletList->at(bulletID)){
        mBulletList->removeAt(bulletID);
    }
}

osg::ref_ptr<BulletNode> CombatManager::getBulletNode(int bulletID)
{
    return mBulletList->at(bulletID);
}

void CombatManager::setBulletTargetModel(int bulletID, SimpleModelNode *target)
{
    if(mBulletList->at(bulletID)){
        mBulletList->at(bulletID)->setTargetModel(target);
    }
}

SimpleModelNode *CombatManager::getBulletTargetModel(int bulletID)
{
    if(mBulletList->at(bulletID)){
        return mBulletList->at(bulletID)->getTargetModel();
    }
    return nullptr;
}

void CombatManager::attackTo(int bulletID, osgEarth::GeoPoint geoPos)
{
    if(mBulletList->at(bulletID)){
        mBulletList->at(bulletID)->attackTo(geoPos);
    }
}

osgEarth::GeoPoint CombatManager::getBulletPosition(int bulletID)
{
    if(mBulletList->at(bulletID)){
        return mBulletList->at(bulletID)->getPosition();
    }
    return osgEarth::GeoPoint();
}

void CombatManager::attackResult(bool result, int bulletID)
{
    if(result && mBulletList->at(bulletID)){
        Explosion *explode = mBulletList->at(bulletID)->explode();
        mCombatLayer->addChild(explode);
        explode->setPosition(mBulletList->at(bulletID)->getPosition());
        mBulletList->at(bulletID)->setNodeMask(0);
    }else{
        mBulletList->at(bulletID)->setNodeMask(0);
    }
}

Assignment *CombatManager::getAssignment(SimpleModelNode *attacker, SimpleModelNode *target)
{
    if(!attacker || !target)
        return nullptr;
    for(auto assignment: mAssignmentList)
        if(assignment->attacker->nodeData().id == attacker->nodeData().id
            && assignment->target->nodeData().id == target->nodeData().id)
            return assignment;
    return nullptr;
}

QList<Assignment *> CombatManager::getAttackerAssignments(SimpleModelNode *attacker)
{
    QList<Assignment *> assignments;
    if(!attacker)
        return assignments;

    for(auto assignment: mAssignmentList)
        if(assignment->attacker->nodeData().id == attacker->nodeData().id)
            assignments.append(assignment);

    return assignments;
}

QList<Assignment *> CombatManager::getTargetAssignments(SimpleModelNode *target)
{
    QList<Assignment *> assignments;
    if(!target)
        return assignments;

    for(auto assignment: mAssignmentList)
        if(assignment->target->nodeData().id == target->nodeData().id)
            assignments.append(assignment);

    return assignments;
}


