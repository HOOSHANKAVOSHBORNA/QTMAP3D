#include "combatManager.h"

CombatManager::CombatManager(MapItem *map)
{
    // mEdgeDataList = new QList<assignmentData>;
    mAssignmentDataMap = new QMap<QString,assignmentData>;
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

void CombatManager::assign(SimpleModelNode *attacker, SimpleModelNode *target , State state)
{
    qDebug() << target->nodeData()->id;
    QString assignmentID = QString::number(attacker->nodeData()->id)+QString::number(target->nodeData()->id);
    if(mAssignmentDataMap->contains(assignmentID)){
        removeAssignment(attacker,target);
    }
    osg::ref_ptr<LineNode> line = new LineNode(mMapItem);
    line->addPoint(attacker->getPosition());
    line->addPoint(target->getPosition());
    line->setTessellation(50);
    line->setWidth(20);
    mCombatLayer->addChild(line);
    switch (state) {
    case PREASSIGN:
        line->setFillColor(osgEarth::Color(osg::Vec4f(1,1,1,1)));
        break;
    case HOVERED:
        line->setFillColor(osgEarth::Color(osg::Vec4f(0.2,0.2,0.5,1)));
        break;
    case SELECTED:
        line->setFillColor(osgEarth::Color(osg::Vec4f(0,1,0,1)));
        break;
    case ASSIGNED:
        line->setFillColor(osgEarth::Color(osg::Vec4f(1,0,0,1)));
        break;
    case SEARCH:
        line->setFillColor(osgEarth::Color(osg::Vec4f(0,0,0,1)));
        break;
    case LOCK:
        line->setFillColor(osgEarth::Color(osg::Vec4f(1,1,1,1)));
        break;
    case FIRE:
        line->setFillColor(osgEarth::Color(osg::Vec4f(1,1,1,1)));
        break;
    case SUCCEED:
        line->setNodeMask(false);
        break;
    case FAILED:
        line->setNodeMask(false);
        break;
    default:
        line->setFillColor(osgEarth::Color("white"));
        break;
    }
    attacker->highlight(true);
    target->highlight(true);
    assignmentData data = {attacker , target , line , state};
    mAssignmentDataMap->insert(QString::number(attacker->nodeData()->id)+QString::number(target->nodeData()->id),data);
}

void CombatManager::removeAssignment(SimpleModelNode *attacker, SimpleModelNode *target)
{
    QString assignmentID = QString::number(attacker->nodeData()->id)+QString::number(target->nodeData()->id);
    try {
        mAssignmentDataMap->remove(assignmentID);
    } catch (...) {
    }

}

void CombatManager::deleteAttackerNode(SimpleModelNode *attacker)
{
    QString attackerID = QString::number(attacker->nodeData()->id);
    for (auto i = mAssignmentDataMap->cbegin(), end = mAssignmentDataMap->cend(); i != end; ++i)
        if(i.key().startsWith(attackerID)){
            mAssignmentDataMap->remove(i.key());
        }
}

void CombatManager::deleteTargetNode(SimpleModelNode *target)
{
    QString targetID = QString::number(target->nodeData()->id);
    for (auto i = mAssignmentDataMap->cbegin(), end = mAssignmentDataMap->cend(); i != end; ++i)
        if(i.key().endsWith(targetID)){
            mAssignmentDataMap->remove(i.key());
        }
}

void CombatManager::setState(assignmentData data, State state)
{
    // data.
}

QMap<QString, assignmentData> *CombatManager::getAssignmentData()
{
    return mAssignmentDataMap;
}

int CombatManager::readyBulletFor(SimpleModelNode *attacker, const std::string &url3D, const std::string &url2D)
{
    osg::ref_ptr<BulletNode> bulletnode = new BulletNode(mMapItem, url3D, url2D);
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
