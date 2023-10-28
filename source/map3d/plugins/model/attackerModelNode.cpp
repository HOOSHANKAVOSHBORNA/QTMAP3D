#include "attackerModelNode.h"

AttackerModelNode::AttackerModelNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QQmlEngine *engine, BookmarkProxyModel *bookmark):
    SimpleModelNode(mapControler, modelUrl, iconUrl, engine, bookmark)
{
    mMapItem = mapControler;
    mBulletModelURL = "../data/models/missile/missile.osgb";
    mBulletIconURL =  "../data/models/missile/missile.png";
    mEngine = engine;
    mBookmark = bookmark;

}

void AttackerModelNode::ready(int count)
{
    mBulletNode = new FlyableModelNode(mMapItem,mBulletModelURL,mBulletIconURL,mEngine,mBookmark);
    this->addChild(mBulletNode);
    mBulletNode->setPosition(this->getPosition());
}

void AttackerModelNode::attackTo(osgEarth::GeoPoint position)
{
    mBulletNode->flyTo(position,100);
    mLastPosition = position;
}

void AttackerModelNode::attackResult(bool result , osgEarth::GeoPoint position)
{
    if(result){
        mExplosion = new Explosion(mMapItem);
        mExplosion->setMapNode(mMapItem->getMapNode());
        this->addChild(mExplosion);
//        position.getSRS().
        mExplosion->setPosition(position);
//        mBulletNode->deleteLater();
    }else{
//        mBulletNode->deleteLater();
    }
}


