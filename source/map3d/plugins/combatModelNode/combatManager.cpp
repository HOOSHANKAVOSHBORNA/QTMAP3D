#include "combatManager.h"

CombatManager::CombatManager()
{

}

void CombatManager::setCombatLayer(ParenticAnnotationLayer *layer)
{
    mCombatLayer = layer;
}

ParenticAnnotationLayer *CombatManager::getCombatLayer()
{
    return mCombatLayer;
}

void CombatManager::assign(SimpleModelNode *attacker, SimpleModelNode *target)
{
    assignmentData *data;
    data->attacker = attacker;
    data->target = target;
    mEdgeDataList->append(data);
}

void CombatManager::removeAttackerFromTarget(SimpleModelNode *attacker, SimpleModelNode *target)
{
    for (int var = 0; var < mEdgeDataList->length() ; ++var) {
        if(mEdgeDataList->at(var)->attacker == attacker && mEdgeDataList->at(var)->target == target){
            mEdgeDataList->removeAt(var);
        }
    }
}

void CombatManager::removeTargetFromAttacker(SimpleModelNode *target, SimpleModelNode *attacker)
{
    for (int var = 0; var < mEdgeDataList->length() ; ++var) {
        if(mEdgeDataList->at(var)->target == target && mEdgeDataList->at(var)->attacker == attacker){
            mEdgeDataList->removeAt(var);
        }
    }
}

void CombatManager::deleteAttackerNode(SimpleModelNode *attacker)
{
    for (int var = 0; var < mEdgeDataList->length() ; ++var) {
        if(mEdgeDataList->at(var)->attacker == attacker){
            mEdgeDataList->removeAt(var);
        }
    }
}

void CombatManager::deleteTargetNode(SimpleModelNode *target)
{
    for (int var = 0; var < mEdgeDataList->length() ; ++var) {
        if(mEdgeDataList->at(var)->target == target){
            mEdgeDataList->removeAt(var);
        }
    }
}

QList<assignmentData *> *CombatManager::getAssignmentData()
{
    return mEdgeDataList;
}

int CombatManager::readyBulletFor(SimpleModelNode *attacker, const std::string url3D, const std::string url2D)
{
    osg::ref_ptr<BulletNode> bulletnode = new BulletNode(mMapItem, url3D, url2D);
    mCombatLayer->addChild(bulletnode);
    bulletnode->setPosition(attacker->getPosition());
    mBulletList.push_back(bulletnode);
    return mBulletList.indexOf(bulletnode);
}

void CombatManager::removeBullet(int bulletID)
{
    if(mBulletList.at(bulletID).valid()){
        mBulletList.removeAt(bulletID);
    }
}

osg::ref_ptr<BulletNode> CombatManager::getBulletNode(int bulletID)
{
    return mBulletList.at(bulletID);
}

void CombatManager::setBulletTargetModel(int bulletID, SimpleModelNode *target)
{
    if(mBulletList.at(bulletID).valid()){
        mBulletList.at(bulletID).get()->setTargetModel(target);
    }
}

SimpleModelNode *CombatManager::getBulletTargetModel(int bulletID)
{
    if(mBulletList.at(bulletID).valid()){
        return mBulletList.at(bulletID).get()->getTargetModel();
    }
    return nullptr;
}

void CombatManager::attackTo(int bulletID, osgEarth::GeoPoint geoPos)
{
    if(mBulletList.at(bulletID).valid()){
        mBulletList.at(bulletID).get()->attackTo(geoPos);
    }
}

osgEarth::GeoPoint CombatManager::getBulletPosition(int bulletID)
{
    if(mBulletList.at(bulletID).valid()){
        return mBulletList.at(bulletID).get()->getPosition();
    }
    return osgEarth::GeoPoint();
}

void CombatManager::attackResult(bool result, int bulletID)
{
    if(result && mBulletList.at(bulletID).valid()){
        Explosion *explode = mBulletList.at(bulletID)->explode();
        mCombatLayer->addChild(explode);
        explode->setPosition(mBulletList.at(bulletID)->getPosition());
        mBulletList.at(bulletID)->setNodeMask(0);
    }else{
        mBulletList.at(bulletID)->setNodeMask(0);
    }
}
