#include "attackManager.h"
#include "lineNode.h"

AttackManager::AttackManager(MapItem *mapControler, SimpleModelNode *parent)
    : mMapItem(mapControler),
    mParent(parent)
{

}

void AttackManager::attackResult(bool result, int bulletID)
{
    if(result && mBulletList.at(bulletID).valid()){
        Explosion *explode = mBulletList.at(bulletID)->explode();
        mAttackManagerLayer->addChild(explode);
        explode->setPosition(mBulletList.at(bulletID)->getPosition());
        mBulletList.at(bulletID)->setNodeMask(0);
    }else{
        mBulletList.at(bulletID)->setNodeMask(0);
    }
}

int AttackManager::readyBullet(const std::string url3D, const std::string url2D)
{
//    if(mBulletCount >= mBulletList.count()){
        osg::ref_ptr<BulletNode> bulletnode = new BulletNode(mMapItem, url3D, url2D);
        mAttackManagerLayer->addChild(bulletnode);
        bulletnode->setPosition(mParent->getPosition());
        mBulletList.push_back(bulletnode);
        return mBulletList.indexOf(bulletnode);
//    }
//    return 0;
}

void AttackManager::attackTo(int bulletID, osgEarth::GeoPoint geoPos)
{
    if(mBulletList.at(bulletID).valid()){
        mBulletList.at(bulletID).get()->attackTo(geoPos);
    }
}


void AttackManager::removeBullet(int bulletID)
{
    if(mBulletList.at(bulletID).valid()){
        mBulletList.removeAt(bulletID);
    }
}

void AttackManager::setBulletTargetModel(int bulletID ,SimpleModelNode *model)
{
    if(mBulletList.at(bulletID).valid()){
        mBulletList.at(bulletID).get()->setTargetModel(model);
    }
}

void AttackManager::setBulletCount(const int count)
{
    mBulletCount = count;
}

int AttackManager::getBulletCount()
{
    return mBulletCount;
}

osgEarth::GeoPoint AttackManager::getBulletPosition(int bulletID)
{
    if(mBulletList.at(bulletID).valid()){
        return mBulletList.at(bulletID).get()->getPosition();
    }
    return osgEarth::GeoPoint();
}

SimpleModelNode *AttackManager::getBulletTargetModel(int bulletID)
{
    if(mBulletList.at(bulletID).valid()){
        return mBulletList.at(bulletID).get()->getTargetModel();
    }
    return nullptr;
}

void AttackManager::setAttackLayer(ParenticAnnotationLayer *layer)
{
    mAttackManagerLayer = layer;
}

ParenticAnnotationLayer *AttackManager::getAttackLayer()
{
    return mAttackManagerLayer;
}

osg::ref_ptr<BulletNode> AttackManager::getBulletNode(int bulletID)
{
    return mBulletList.at(bulletID);
}

QList<SimpleModelNode *> AttackManager::getNearTargets()
{
    return mNearTargets;
}

void AttackManager::setNearTargets(SimpleModelNode *targetNode)
{
    mNearTargets.append(targetNode);
}

void AttackManager::showNearTargets()
{
    mHighlightGroup = new osg::Group;
    for (int var = 0; var < mNearTargets.length(); ++var) {
        osg::ref_ptr<LineNode> line = new LineNode(mMapItem);
        line->addPoint(mParent->getPosition());
        line->addPoint(mNearTargets.at(var)->getPosition());
        line->setFillColor(osgEarth::Color::Yellow);
        mHighlightGroup->addChild(line);
        mNearTargets.at(var)->select();
    }
    mParent->addChild(mHighlightGroup);
}

void AttackManager::hideNearTargets()
{
    if(mParent->containsNode(mHighlightGroup)){
        for (int var = 0; var < mNearTargets.length(); ++var) {
            mNearTargets.at(var)->select();
        }
        mParent->removeChild(mHighlightGroup);
    }

}
