#include "attackManager.h"

AttackManager::AttackManager(MapItem *mapControler, QQmlEngine *engine, BookmarkManager *bookmark, SimpleModelNode *parent)
    : mMapItem(mapControler),
      mEngine(engine),
      mBookmark(bookmark),
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

void AttackManager::setTargetPosition(int bulletID, osgEarth::GeoPoint geoPos)
{
    if(mBulletList.at(bulletID).valid()){
        mBulletList.at(bulletID).get()->setTargetPosition(geoPos);
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

osgEarth::GeoPoint AttackManager::getTargetPosition(int bulletID)
{
    if(mBulletList.at(bulletID).valid()){
        return mBulletList.at(bulletID).get()->getTargetPosition();
    }
    return osgEarth::GeoPoint();
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
